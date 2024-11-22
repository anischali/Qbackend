cmake_minimum_required(VERSION 3.27)

if (DEFINED STUN_CLIENT_EXEC)
project(stun_client VERSION 1.0.1 DESCRIPTION "stun_client")
add_executable(stun_client)
set(QBACKEND_LIBRARY_NAME stun_client)
include(net_compenent.cmake)
add_compile_definitions(
    -DSTUN_CLIENT_EXEC=1
)
endif()


