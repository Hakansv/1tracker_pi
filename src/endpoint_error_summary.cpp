#include "1tracker_pi/endpoint_error_summary.h"

#include <array>
#include <string_view>

#include "1tracker_pi/endpoint_policy.h"

namespace tracker_pi {

namespace {

bool containsText(const std::string& text, std::string_view needle) {
  return text.find(needle) != std::string::npos;
}

struct ErrorPattern {
  std::string_view needle;
  std::string_view message;
};

// Ordered: first match wins. Keep this list aligned with the user-facing copy
// expected by tracker_dialog.cpp's error panel.
constexpr std::array<ErrorPattern, 9> kPatterns{{
    {"Invalid or expired token", "The authorization token is invalid or expired."},
    {"Could not resolve host", "The server name could not be resolved."},
    {"Failed to connect", "Could not connect to the server."},
    {"Connection refused", "Could not connect to the server."},
    {"timed out", "The server took too long to respond."},
    {"HTTP 401", "The server rejected the credentials."},
    {"HTTP 403", "The server refused this request."},
    {"HTTP 404", "The tracking URL could not be found."},
    {"HTTP 200", "The server responded, but rejected the tracking data."},
}};

constexpr std::string_view kMissingApiKeyNeedle =
    "an active API key was not provided";
constexpr std::string_view kGenericHttpNeedle = "HTTP ";
constexpr std::string_view kMissingApiKeyNfl =
    "Your NFL boat key is missing or invalid.";
constexpr std::string_view kMissingApiKeyJson =
    "The API key was not accepted by the server.";
constexpr std::string_view kGenericHttpMessage =
    "The server returned an error while processing the tracking request.";
constexpr std::string_view kFallbackMessage =
    "Tracking could not be sent. Check the technical details below.";

}  // namespace

std::string summarizeEndpointError(const EndpointConfig& endpoint,
                                   const std::string& rawError) {
  if (rawError.empty()) {
    return {};
  }

  if (containsText(rawError, kMissingApiKeyNeedle)) {
    return std::string(isNoForeignLandType(endpoint.type) ? kMissingApiKeyNfl
                                                          : kMissingApiKeyJson);
  }

  for (const auto& pattern : kPatterns) {
    if (containsText(rawError, pattern.needle)) {
      return std::string(pattern.message);
    }
  }

  if (containsText(rawError, kGenericHttpNeedle)) {
    return std::string(kGenericHttpMessage);
  }

  return std::string(kFallbackMessage);
}

EndpointErrorUiState computeEndpointErrorUiState(
    const EndpointConfig* endpoint,
    const std::map<std::string, EndpointUiState>& statuses) {
  EndpointErrorUiState ui;
  if (endpoint == nullptr) {
    return ui;
  }

  const auto it = statuses.find(endpointStateKey(*endpoint));
  if (it == statuses.end() || it->second.lastErrorMessage.empty()) {
    return ui;
  }

  ui.visible = true;
  ui.details = it->second.lastErrorMessage;
  ui.summary = summarizeEndpointError(*endpoint, ui.details);
  if (!it->second.lastSentLocalTime.empty()) {
    ui.metaText = "Last call: " + it->second.lastSentLocalTime;
  }
  return ui;
}

}  // namespace tracker_pi
