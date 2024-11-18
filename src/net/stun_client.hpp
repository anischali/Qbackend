#ifndef __STUN_CLIENT_HPP__
#define __STUN_CLIENT_HPP__
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>


struct __attribute__((packed)) stun_request_t {
    stun_request_t() {
        for (int i = 0; i < sizeof(transaction_id); ++i) {
            transaction_id[i] = rand() % 256;
        }
    }

    const uint16_t msg_type = htons(0x1);
    const uint16_t msg_len = htons(0x0);
    const uint32_t magic_cookie = htonl(0x2112A442);
    uint8_t transaction_id[12];
};


struct __attribute__((packed)) stun_response_t {
    uint16_t msg_type = htons(0x1);
    uint16_t msg_len = htons(0x0);
    uint32_t magic_cookie = htonl(0x2112A442);
    uint8_t transaction_id[12];
    uint8_t attributes[1000];
};

class stun_client
{
private:
    int fd;
    short port;

public:
    struct sockaddr_in ext_ip;
    stun_client(short port);
    stun_client();
    ~stun_client();

    int stun_request(struct sockaddr_in stun_server);
    int stun_request(const char *stun_hostname, short stun_port);

};

#endif