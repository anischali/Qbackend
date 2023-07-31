


target_sources(${QBACKEND_LIBRARY_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/translation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lang_translation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/translations.cpp
    ${CMAKE_CURRENT_LIST_DIR}/settings.cpp
)


target_precompile_headers(${QBACKEND_LIBRARY_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/translation.hpp
    ${CMAKE_CURRENT_LIST_DIR}/lang_translation.hpp
    ${CMAKE_CURRENT_LIST_DIR}/translations.hpp
    ${CMAKE_CURRENT_LIST_DIR}/settings.hpp
)


include(../json/json_engine_compenent)