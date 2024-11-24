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


