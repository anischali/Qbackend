target_sources(${QBACKEND_LIBRARY_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/lib/stun_client.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lib/peer_connection.cpp
)


target_precompile_headers(${QBACKEND_LIBRARY_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/lib/stun_client.hpp
    ${CMAKE_CURRENT_LIST_DIR}/lib/peer_connection.hpp
)