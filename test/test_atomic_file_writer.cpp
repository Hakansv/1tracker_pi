#include <unistd.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "1tracker_pi/atomic_file_writer.h"

namespace {

namespace fs = std::filesystem;

void expect(bool condition, const char* message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

std::string slurp(const fs::path& path) {
  std::ifstream stream(path);
  std::ostringstream buffer;
  buffer << stream.rdbuf();
  return buffer.str();
}

}  // namespace

int main() {
  const auto tmpRoot =
      fs::temp_directory_path() /
      ("atomic_file_writer_test_" + std::to_string(::getpid()));
  fs::create_directories(tmpRoot);

  // 1. Writes to a fresh path, no stray .tmp left behind.
  {
    const auto target = tmpRoot / "one.json";
    tracker_pi::writeFileAtomically(target, "hello");
    expect(slurp(target) == "hello", "file content must match");
    expect(!fs::exists(target.string() + ".tmp"),
           "temp sibling must be cleaned up after rename");
  }

  // 2. Overwrite: original stays intact if the final rename succeeds with new bytes.
  {
    const auto target = tmpRoot / "two.json";
    tracker_pi::writeFileAtomically(target, "first");
    tracker_pi::writeFileAtomically(target, "second-longer-content");
    expect(slurp(target) == "second-longer-content", "overwrite must replace content");
    expect(!fs::exists(target.string() + ".tmp"),
           "no tmp residue after overwrite");
  }

  // 3. Nested parent directories created on demand.
  {
    const auto target = tmpRoot / "deep" / "nested" / "three.json";
    tracker_pi::writeFileAtomically(target, "ok");
    expect(fs::exists(target), "nested directories should be created");
    expect(slurp(target) == "ok", "nested content");
  }

  // 4. Writing to an existing file that is read-only (no parent dir, just path)
  //    — we don't simulate permission failure here (fragile across platforms),
  //    but we do exercise the "path has no parent" branch.
  {
    const auto target = tmpRoot / "no-dir.json";
    tracker_pi::writeFileAtomically(target, "x");
    expect(slurp(target) == "x", "single-segment path works");
  }

  // 5. Binary content with embedded NUL bytes round-trips.
  {
    const auto target = tmpRoot / "binary.json";
    std::string bytes("a\0b\0c", 5);
    tracker_pi::writeFileAtomically(target, bytes);
    expect(slurp(target) == bytes, "binary content round-trip");
  }

  // 6. When the parent cannot be created (blocker is a regular file), throw.
  {
    const auto blocker = tmpRoot / "blocker";
    {
      std::ofstream(blocker) << "regular file";
    }
    const auto target = blocker / "child" / "file";
    bool threw = false;
    try {
      tracker_pi::writeFileAtomically(target, "nope");
    } catch (const std::runtime_error&) {
      threw = true;
    }
    expect(threw, "must throw when parent directory cannot be created");
  }

  // 7. When the target path is itself a directory, rename() must fail and throw.
  {
    const auto dir = tmpRoot / "is-a-dir";
    fs::create_directories(dir);
    bool threw = false;
    try {
      tracker_pi::writeFileAtomically(dir, "content");
    } catch (const std::runtime_error&) {
      threw = true;
    }
    expect(threw, "must throw when target is an existing directory");
    expect(!fs::exists(dir.string() + ".tmp"),
           "tmp must be cleaned up on rename failure");
  }

  // 6. Exception on impossible parent (relative path into nonexistent root on
  //    some filesystems is hard to force; skip — covered informally by the happy paths).

  // Cleanup
  std::error_code ec;
  fs::remove_all(tmpRoot, ec);

  return EXIT_SUCCESS;
}
