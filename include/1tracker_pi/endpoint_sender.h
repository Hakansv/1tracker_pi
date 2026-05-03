#pragma once

#include <string>

#include "1tracker_pi/endpoint_config.h"

namespace tracker_pi {

class EndpointSender {
public:
  virtual ~EndpointSender() = default;

  struct Result {
    bool success = false;
    long httpStatus = 0;
    std::string message;
  };

  virtual Result send(const EndpointConfig& endpoint,
                      const std::string& payload) const;

  static std::string MaskSecret(const std::string& secret);
  static std::string RedactSensitiveText(std::string text,
                                         const EndpointConfig& endpoint);

  // Force libcurl + OpenSSL initialisation on the calling thread. Call
  // once from the OpenCPN main thread during plugin Init(), before the
  // scheduler worker thread starts. Without this, the first send() ticks
  // on the worker thread and triggers TLS-slot allocation + OpenSSL
  // lazy init from a thread that didn't exist when the plugin .so was
  // dlopen'd — a known crash mode on Android.
  static void Prewarm();

  // Tell EndpointSender where the bundled Mozilla CA cert file lives, so
  // libcurl can verify HTTPS chains. Resolved at plugin Init() time on
  // the main thread (plugin_ui_utils.cpp lives in the plugin .so layer
  // and can hit OpenCPN's GetPluginDataDir API; this static lib cannot).
  // Empty string means "not found, fall back to libcurl default" — on
  // Android that means no verification root, every HTTPS endpoint will
  // fail cert validation. configureSSL() inside send() reads this once
  // per process via call_once.
  static void SetCaBundlePath(const std::string& path);
};

}  // namespace tracker_pi
