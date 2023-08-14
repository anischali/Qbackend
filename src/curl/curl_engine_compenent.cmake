


target_sources(${QBACKEND_LIBRARY_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/curl_engine.cpp
    ${CMAKE_CURRENT_LIST_DIR}/curl_engine.hpp
)


target_precompile_headers(${QBACKEND_LIBRARY_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/curl_engine.hpp
)


