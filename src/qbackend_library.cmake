
set(QBACKEND_LIBRARY_NAME Qbackend)

include(GNUInstallDirs)

add_library(Qbackend SHARED)

set_target_properties(Qbackend PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

install(TARGETS Qbackend
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Qbackend/
)

file(GLOB Qbackend_HEADERS ${CMAKE_CURRENT_LIST_DIR}/*/*.hpp)

install(FILES ${Qbackend_HEADERS} 
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Qbackend)

target_include_directories(Qbackend
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/audio
        ${CMAKE_CURRENT_LIST_DIR}/curl
        ${CMAKE_CURRENT_LIST_DIR}/json
        ${CMAKE_CURRENT_LIST_DIR}/storage
        ${CMAKE_CURRENT_LIST_DIR}/xml
        ${CMAKE_CURRENT_LIST_DIR}/model
)

set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/audio
    ${CMAKE_CURRENT_LIST_DIR}/curl
    ${CMAKE_CURRENT_LIST_DIR}/json
    ${CMAKE_CURRENT_LIST_DIR}/storage
    ${CMAKE_CURRENT_LIST_DIR}/xml
    ${CMAKE_CURRENT_LIST_DIR}/model
)

include(audio_engine_compenent)
include(curl_engine_compenent)
include(json_engine_compenent)
include(storage_engine_compenent)
include(xml_engine_compenent)
include(model_compenent)


if (${CMAKE_LIBRARY_ARCHITECTURE} MATCHES "^aarch64-linux-android")
    set(ARCH_EXTRA_DIRS "/home/anicha1/tmp/android")
elseif(${CMAKE_LIBRARY_ARCHITECTURE} MATCHES "^x86_64-linux-gnu")
endif()



find_package(nlohmann_json PATHS ${ARCH_EXTRA_DIRS}/json/share/cmake/nlohmann_json)
find_library(NAME fmt PATHS ${ARCH_EXTRA_DIRS}/fmt/lib)


