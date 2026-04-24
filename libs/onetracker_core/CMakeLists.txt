# Plugin-local static library with the 1tracker core logic.
# Exposed so both the plugin shared library and the test suite can link it.

if (TARGET onetracker_core)
  return ()
endif ()

set(_src_dir "${CMAKE_SOURCE_DIR}/src")

add_library(onetracker_core STATIC EXCLUDE_FROM_ALL
    "${_src_dir}/atomic_file_writer.cpp"
    "${_src_dir}/config_loader.cpp"
    "${_src_dir}/endpoint_error_summary.cpp"
    "${_src_dir}/endpoint_policy.cpp"
    "${_src_dir}/endpoint_sender.cpp"
    "${_src_dir}/endpoint_type_registry.cpp"
    "${_src_dir}/payload_builder.cpp"
    "${_src_dir}/scheduler.cpp"
    "${_src_dir}/state_store.cpp"
)

target_include_directories(onetracker_core PUBLIC
    "${CMAKE_SOURCE_DIR}/include"
)
target_compile_features(onetracker_core PUBLIC cxx_std_17)

if (QT_ANDROID)
  # On Android, libcurl / wxcurl / jsoncpp / wx live in the OpenCPN host
  # process and resolve at dlopen. We only need headers at compile time.
  # wx headers are provided by cmake/AndroidLibs.cmake via directory-scope
  # include_directories() (included from Plugin.cmake late_init()).
  target_include_directories(onetracker_core PUBLIC
      "${CMAKE_SOURCE_DIR}/opencpn-libs/jsoncpp/include"
      "${CMAKE_SOURCE_DIR}/opencpn-libs/wxcurl/include"
      "${CMAKE_SOURCE_DIR}/opencpn-libs/curl/${CMAKE_ANDROID_ARCH_ABI}/include"
  )
else ()
  target_link_libraries(onetracker_core PUBLIC
      ocpn::jsoncpp
      ocpn::libcurl
      ocpn::wxcurl
      ${wxWidgets_LIBRARIES}
  )
endif ()
