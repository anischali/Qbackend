
set(QBACKEND_LIBRARY_NAME Qbackend)

include(FetchContent)

add_library(Qbackend)

file(GLOB Qbackend_HEADERS ${CMAKE_CURRENT_LIST_DIR}/*/*.hpp)

set_target_properties(Qbackend PROPERTIES
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER "${Qbackend_HEADERS}"
    SOVERSION 1
)

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

FetchContent_Declare(libcurl
  GIT_REPOSITORY https://github.com/curl/curl.git
  GIT_TAG tiny-curl-7_72_0
)
FetchContent_MakeAvailable(libcurl)


target_link_libraries(Qbackend PRIVATE
    fmt::fmt
    nlohmann_json
    libcurl
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Qbackend.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/Qbackend.pc
    @ONLY
)

install(TARGETS Qbackend
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Qbackend
)


install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/Qbackend.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)