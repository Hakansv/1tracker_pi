#pragma once

#include <cstdint>
#include <shared_mutex>

#include "1tracker_pi/snapshot.h"

namespace tracker_pi {

class StateStore {
public:
  void updateTimevalue(std::int64_t timevalue);
  void updateLatLon(double lat, double lon);
  void updateAWA(double awa);
  void updateAWS(double aws);

  bool hasValidPosition() const;
  Snapshot getSnapshot() const;

private:
  // std::shared_mutex, not std::mutex — this is intentional.
  //
  // Microsoft changed the binary layout of std::mutex in VS 2022 17.10
  // (microsoft/STL PR #4233, May 2024). Binaries compiled with 17.10+ are
  // ABI-incompatible with older msvcp140.dll. OpenCPN 5.14 bundles its own
  // older msvcp140.dll in its install directory, which plugins load via
  // Windows' DLL search order — so every std::mutex::lock() in a plugin
  // built with a post-17.10 toolset crashes with AV 0xc0000005 reading 0x0
  // (_Mutex_t internal pointer is null).
  //
  // std::shared_mutex is implemented directly on top of Windows SRWLOCK
  // (kernel32, not msvcp140) with a stable layout across the whole MSVC
  // 14.x ABI. Using lock_guard<shared_mutex> takes exclusive locks, which
  // is semantically identical to lock_guard<mutex> for our usage — we
  // never use shared (reader) locking.
  //
  // Do not "simplify" this back to std::mutex unless OpenCPN has shipped a
  // current VC++ Redistributable, or Microsoft has removed the pre-17.10
  // codepath from msvcp140.dll entirely.
  mutable std::shared_mutex mutex_;
  Snapshot snapshot_;
};

}  // namespace tracker_pi
