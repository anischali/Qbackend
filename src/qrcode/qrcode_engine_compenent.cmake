


target_sources(${QBACKEND_LIBRARY_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/qrcode_engine.cpp
)

target_precompile_headers(${QBACKEND_LIBRARY_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/qrcode_engine.hpp
)

