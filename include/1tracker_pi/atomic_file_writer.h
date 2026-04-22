#pragma once

#include <filesystem>
#include <string>

namespace tracker_pi {

// Writes `contents` to `path` atomically on POSIX filesystems: writes to a
// sibling `.tmp` file first, flushes it, and std::filesystem::rename()s it
// into place. A crash mid-write leaves the original file intact.
//
// Throws std::runtime_error on any failure. The temp file is best-effort
// cleaned up before the exception propagates.
void writeFileAtomically(const std::filesystem::path& path,
                         const std::string& contents);

}  // namespace tracker_pi
