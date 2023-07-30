


target_sources(backend_test PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/translation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lang_translation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/translations.cpp
)


target_precompile_headers(backend_test PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/translation.hpp
    ${CMAKE_CURRENT_LIST_DIR}/lang_translation.hpp
    ${CMAKE_CURRENT_LIST_DIR}/translations.hpp
)


