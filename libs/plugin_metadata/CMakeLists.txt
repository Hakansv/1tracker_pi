# Generates version.h and plugin_metadata.h from PKG_* / PROJECT_VERSION_*
# and exposes them via an INTERFACE library.

if (TARGET plugin_metadata_headers)
  return ()
endif ()

string(REPLACE "\n" " " _desc "${PKG_DESCRIPTION}")
string(STRIP "${_desc}" _desc)
set(ONETRACKER_PLUGIN_SUMMARY "${PKG_SUMMARY}")
set(ONETRACKER_PLUGIN_DESCRIPTION "${_desc}")
set(ONETRACKER_PLUGIN_COMMON_NAME "${DISPLAY_NAME}")

set(_gen_root "${CMAKE_BINARY_DIR}/generated")
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
  "${_gen_root}/${PKG_NAME}/version.h"
  @ONLY
)
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/plugin_metadata.h.in"
  "${_gen_root}/${PKG_NAME}/plugin_metadata.h"
  @ONLY
)

add_library(plugin_metadata_headers INTERFACE)
target_include_directories(plugin_metadata_headers INTERFACE "${_gen_root}")
