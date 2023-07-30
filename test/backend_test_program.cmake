
add_executable(backend_test)

include(GNUInstallDirs)

set_target_properties(backend_test PROPERTIES
    VERSION ${PROJECT_VERSION}
)

target_sources(backend_test PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/backend_test.cpp
)

target_include_directories(backend_test
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/model
)


set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/model
)

target_link_libraries(backend_test Qbackend)

include(qmodel_compenent)
