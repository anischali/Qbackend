
add_executable(backend_test)

include(FetchContent)

set_target_properties(backend_test PROPERTIES
    VERSION ${PROJECT_VERSION}
)

target_sources(backend_test PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/backend_test.cpp
)


if (${CMAKE_LIBRARY_ARCHITECTURE} MATCHES "^aarch64-linux-android")
    set(ARCH_EXTRA_DIRS "/home/anicha1/tmp/android")
elseif(${CMAKE_LIBRARY_ARCHITECTURE} MATCHES "^x86_64-linux-gnu")
endif()

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 9.0.0
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)



target_link_libraries(backend_test PRIVATE
    fmt::fmt
    nlohmann_json
    Qbackend
)

