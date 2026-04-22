#include "1tracker_pi/atomic_file_writer.h"

#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <system_error>

namespace tracker_pi {

namespace {

std::filesystem::path tempSiblingPath(const std::filesystem::path& path) {
  auto tmp = path;
  tmp += ".tmp";
  return tmp;
}

void removeIgnoreError(const std::filesystem::path& path) {
  std::error_code ec;
  std::filesystem::remove(path, ec);
}

}  // namespace

void writeFileAtomically(const std::filesystem::path& path,
                         const std::string& contents) {
  if (!path.parent_path().empty()) {
    std::error_code ec;
    std::filesystem::create_directories(path.parent_path(), ec);
    if (ec) {
      throw std::runtime_error("create_directories failed for " +
                               path.parent_path().string() + ": " + ec.message());
    }
  }

  const auto tmp = tempSiblingPath(path);
  // Scope: close the stream before the rename.
  {
    std::ofstream stream(tmp, std::ios::binary | std::ios::trunc);
    if (!stream.is_open()) {
      throw std::runtime_error("unable to open " + tmp.string() + " for writing");
    }
    stream.write(contents.data(), static_cast<std::streamsize>(contents.size()));
    stream.flush();
    if (!stream) {
      removeIgnoreError(tmp);
      throw std::runtime_error("failed while writing " + tmp.string());
    }
  }

  std::error_code ec;
  std::filesystem::rename(tmp, path, ec);
  if (ec) {
    removeIgnoreError(tmp);
    throw std::runtime_error("rename " + tmp.string() + " -> " +
                             path.string() + " failed: " + ec.message());
  }
}

}  // namespace tracker_pi
