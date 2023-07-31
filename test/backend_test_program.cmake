
add_executable(backend_test)

include(GNUInstallDirs)

set_target_properties(backend_test PROPERTIES
    VERSION ${PROJECT_VERSION}
)

target_sources(backend_test PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/backend_test.cpp
)

target_link_libraries(backend_test Qbackend)

if (${CMAKE_LIBRARY_ARCHITECTURE} MATCHES "^aarch64-linux-android")
    set(ARCH_EXTRA_DIRS "/home/anicha1/tmp/android")
elseif(${CMAKE_LIBRARY_ARCHITECTURE} MATCHES "^x86_64-linux-gnu")
endif()



find_package(nlohmann_json PATHS ${ARCH_EXTRA_DIRS}/json/share/cmake/nlohmann_json)
target_link_libraries(backend_test fmt)
