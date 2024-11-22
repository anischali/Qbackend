cmake_minimum_required(VERSION 3.27)


project(QbackendLib VERSION 1.0.1 DESCRIPTION "library for quran backend")



set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/src
)


include(qbackend_library)



if (${TEST_PROG})
set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/test
)
include(backend_test_program)
endif()

if (DEFINED STUN_CLIENT_EXEC)
enable_language(C CXX ASM)
project(litep2p VERSION 1.0.1 DESCRIPTION "stun_client")
add_executable(stun-client)
add_executable(udp-client)

set(QBACKEND_LIBRARY_NAME stun-client)
include(src/net/net_compenent.cmake)

target_sources(stun-client PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/net/stun-client.cpp
)

target_sources(udp-client PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/net/udp-client.c
)

add_compile_options(-g -ggdb -O0 -lpthread)
add_link_options(-g -ggdb -O0 -lpthread)

add_compile_definitions(
    -DSTUN_CLIENT_EXEC=1
)
endif()

