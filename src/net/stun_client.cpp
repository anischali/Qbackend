#include "stun_client.hpp"
#include <errno.h>
#include <time.h>
#include <string>


#define err_ret(msg, err) \
    printf("%d: %s\n", err, msg); \
    return err

#define min(x, y) x < y ? x : y

static inline bool c_array_cmp(uint8_t a1[], uint8_t a2[], int len) {
    
    while(len-- > 0 && *(a1++) != *(a2++));
    return len == 0;
}

stun_client::stun_client(short port) : 
    port{port}, ext_ip{0}
{
    fd = socket(AF_INET, SOCK_DGRAM, 0);
}

stun_client::stun_client() : 
    port{ (short)((rand() % 0xffff) + 2048) }, ext_ip{0}
{
    fd = socket(AF_INET, SOCK_DGRAM, 0);
}

stun_client::~stun_client()
{
    close(fd);
}

int stun_client::stun_request(struct sockaddr_in stun_server) {
    struct sockaddr_in laddr;  
    struct stun_request_t request;
    struct stun_response_t response;
    int ret, len, i;
    struct timeval tv;
    uint8_t *attrs;
    uint16_t attr_len = 0, attr_type = 0;

    bzero(&laddr, sizeof(laddr));
    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(port);

    if (fd < 0) {
        err_ret("Invalid socket fd", fd);
    }
    ret = bind(fd, (struct sockaddr *)&laddr, sizeof(laddr));
    if (ret < 0) {
        err_ret("Failed to bind", ret);
    }
    
    ret = sendto(fd, &request, sizeof(request), 0, (struct sockaddr *)&stun_server, sizeof(stun_server));
    if (ret < 0) {
        err_ret("Failed to send data", ret);
    }

    tv.tv_sec = 5;
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
    ret = recvfrom(fd, &response, sizeof(response), 0, NULL, 0);
    if (ret < 0) {
        err_ret("Failed to recv data", ret);
    }

    if (response.magic_cookie != request.magic_cookie)
        return -EINVAL;

    if (c_array_cmp(response.transaction_id, request.transaction_id, sizeof(request.transaction_id)))
        return -EINVAL;

    if (response.msg_type != htons(0x0101))
        return -EINVAL;
    
    attrs = response.attributes;
    len = min(response.msg_len, sizeof(response.attributes));

    for (i = 0; i < len; i += (4 + attr_len)) {
        attr_type = ntohs(*(int16_t*)(&attrs[i]));
        attr_len = ntohs(*(int16_t*)(&attrs[i + 2]));

        if (attr_type == 0x020) {
            ext_ip.sin_port = (*(int16_t *)(&attrs[i + 6]));
            ext_ip.sin_port ^= ((uint16_t)response.magic_cookie);
            ext_ip.sin_port = htons(ext_ip.sin_port);

            ext_ip.sin_addr.s_addr = (*(uint32_t *)&attrs[i + 8]);
            ext_ip.sin_addr.s_addr ^= response.magic_cookie;
            
            return 0;
        }
    }

    return -ENOENT;
}


int stun_client::stun_request(const char *stun_hostname, short stun_port) {
    struct sockaddr_in stun_server, *addr;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_in* h;
    char *hostname, *service, hst[512];
    int ret;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    memset(hst, 0, sizeof(hst));
    memcpy(hst, stun_hostname, min(512, strlen(stun_hostname)));
    service = strtok_r(hst, ":/", &hostname);

    if (hostname[0] == '/' && hostname[1] == '/')
        hostname = &hostname[2];

    ret = getaddrinfo(hostname, service, &hints, &servinfo);
    if (ret != 0) {
        return ret;
    }
    
    for (p = servinfo; p != NULL; p = p->ai_next) {
        addr = (struct sockaddr_in*)p->ai_addr;
        memcpy(&stun_server, addr, sizeof(stun_server));
    }
    
    freeaddrinfo(servinfo);

    stun_server.sin_port = htons(stun_port);
    stun_server.sin_family = AF_INET;

    return stun_request(stun_server);
}

#if defined(STUN_CLIENT_EXEC)
//stun:stun.l.google.com 19302
int main(int argc, char *argv[]) {

    if (argc < 2)
        return -1;

    srand(time(NULL));
    stun_client stun;
    int ret = stun.stun_request(argv[1], atoi(argv[2]));

    printf("ret: %d ip addr: %s port: %d\n", ret, inet_ntoa(stun.ext_ip.sin_addr), stun.ext_ip.sin_port);

    stun.~stun_client();

    return 0;
}
#endif