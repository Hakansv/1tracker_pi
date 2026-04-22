#pragma once

#include <filesystem>
#include <functional>
#include <string>

namespace tracker_pi {

// Per-thread: turn Windows structured exceptions (access violations, stack
// overflows, divide-by-zero, etc.) into a catchable C++ std::runtime_error
// so an existing `catch (...)` handler can swallow them. Must be called on
// every thread we want to protect. No-op on non-Windows. Requires /EHa on
// MSVC (see cmake/PluginCompiler.cmake).
void installSehTranslator();

// Process-wide: install an unhandled-exception filter that writes a Windows
// minidump (.dmp) file into `dumpDir` when any thread in the host process
// crashes uncaught. Safe to call many times; the filter is installed once.
// No-op on non-Windows.
void installCrashHandler(const std::filesystem::path& dumpDir);

// Runs `fn` inside both the SEH translator (if configured) and a C++
// try/catch. Any exception becomes `false` with an error string written to
// `errorOut` (if non-null). Never re-throws, never propagates. Use this to
// wrap every plugin entry point so nothing a plugin does can take OCPN down.
bool runGuarded(const char* tag, const std::function<void()>& fn,
                const std::function<void(const std::string&)>& logFn,
                std::string* errorOut = nullptr);

}  // namespace tracker_pi
