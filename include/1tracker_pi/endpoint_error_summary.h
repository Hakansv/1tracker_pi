#pragma once

#include <map>
#include <string>

#include "1tracker_pi/endpoint_config.h"
#include "1tracker_pi/endpoint_ui_state.h"

namespace tracker_pi {

std::string summarizeEndpointError(const EndpointConfig& endpoint,
                                   const std::string& rawError);

struct EndpointErrorUiState {
  bool visible = false;
  std::string summary;      // user-facing translation of the raw error
  std::string details;      // raw error text as reported by the sender
  std::string metaText;     // e.g. "Last call: 2026-04-22 18:42", empty if no timestamp
};

// Pure: decides what the error panel should show for a given endpoint.
// Returns visible=false when there is no endpoint, no recorded status, or no error.
EndpointErrorUiState computeEndpointErrorUiState(
    const EndpointConfig* endpoint,
    const std::map<std::string, EndpointUiState>& statuses);

}  // namespace tracker_pi
