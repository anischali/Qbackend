#include "stun_client.hpp"
#include <errno.h>
#include <time.h>
#include <string>
#include <thread>
#include <vector>

#define err_ret(msg, err) \
    printf("%d: %s\n", err, msg); \
    return err

#define min(x, y) x < y ? x : y

static inline bool c_array_cmp(uint8_t a1[], uint8_t a2[], int len) {
    
    while(len-- > 0 && *(a1++) != *(a2++));
    return len == 0;
}

stun_client::stun_client(int socket_fd) : 
    _socket{socket_fd}, ext_ip{0}
{}

stun_client::~stun_client()
{
}

int stun_client::stun_request(struct sockaddr_in stun_server) {
    struct sockaddr_in laddr;  
    struct stun_request_t request;
    struct stun_response_t response;
    uint16_t attr_len = 0, attr_type = 0;
    int ret, len, i;
    uint8_t *attrs;

    ret = sendto(_socket, &request, sizeof(request), 0, (struct sockaddr *)&stun_server, sizeof(stun_server));
    if (ret < 0) {
        err_ret("Failed to send data", ret);
    }

    ret = recvfrom(_socket, &response, sizeof(response), 0, NULL, 0);
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
            ext_ip.sin_port = ext_ip.sin_port;

            ext_ip.sin_addr.s_addr = (*(uint32_t *)&attrs[i + 8]);
            ext_ip.sin_addr.s_addr ^= response.magic_cookie;
            
            return 0;
        }
    }

    return -ENOENT;
}


int stun_client::stun_request(const char *stun_hostname, short stun_port) {
    struct sockaddr_in *addr;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_in *h;
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
    servinfo = NULL;

    stun_server.sin_port = htons(stun_port);
    stun_server.sin_family = AF_INET;

    return stun_request(stun_server);
}


class udp_peer_connection {

public:
    struct sockaddr_in local;
    stun_client *cstun;
    int sock_fd;
    struct sockaddr_in remote;

    udp_peer_connection(short port) {
        timeval tv = { .tv_sec = 5 };
        int enable = 1;
        
        sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock_fd > 0) {
            setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
            setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(enable));
            setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        }

        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_family = AF_INET;
        local.sin_port = htons(port);

        bind(sock_fd, (struct sockaddr *)&local, sizeof(local));

        cstun = new stun_client(sock_fd);
    };

    ~udp_peer_connection() {
        cstun->~stun_client();
        delete(cstun);
        close(sock_fd);
    };



    void set_remote(struct sockaddr_in remote_peer) {
        memcpy(&remote, &remote_peer, sizeof(remote));
    };

};

void visichat_listener(void *args) {
    int ret;
    static char buf[512];
    udp_peer_connection *conn = (udp_peer_connection *)args; 
    socklen_t len = sizeof(conn->remote);
    struct sockaddr_in s_addr;

    printf("receiver thread start [OK]\n");

    while(true) {
        ret = recvfrom(conn->sock_fd, buf, 512, 0, (struct sockaddr *)&s_addr, &len);
        if (ret < 0 || buf[0] == 0)
            continue;

        buf[ret] = 0;

        if (!strncmp("exit", &buf[0], 4))
            continue;

        fprintf(stdout, "[%s:%d]: %s\n\r> ", inet_ntoa(s_addr.sin_addr), ntohs(s_addr.sin_port), buf);
    }
}

void visichat_sender(void *args) {
    int cnt = 0;
    char c = 0;
    static char buf[512];
    udp_peer_connection *conn = (udp_peer_connection *)args;

    printf("sender thread start [OK]\n");

    while(true) {
        printf("\r> ");
        while((c = getc(stdin)) != '\n') {
            buf[cnt] = c;
            cnt = ((cnt + 1) % sizeof(buf));
        }

        if (cnt <= 0)
            continue;

        sendto(conn->sock_fd, buf, cnt, 0, (struct sockaddr *)&conn->remote, sizeof(conn->remote));
        cnt = 0;

        if (!strncmp("exit", &buf[0], 4)) {
            sleep(1);
            printf("sender thread stop [OK]\n");
            return;
        }
    }
}

//#if defined(STUN_CLIENT_EXEC)
//stun:stun.l.google.com 19302
int main(int argc, char *argv[]) {

    if (argc < 4) {
        printf("wrong arguments number !\n");
        return -1;
    }

    srand(time(NULL));
    udp_peer_connection conn(atoi(argv[3]));
    
    int ret = conn.cstun->stun_request(argv[1], atoi(argv[2]));
    if (ret < 0) {
        conn.~udp_peer_connection();
        return ret;
    }

    conn.remote.sin_addr.s_addr = conn.cstun->ext_ip.sin_addr.s_addr;/*htonl(inet_network("192.168.0.10"));*/
    conn.remote.sin_family = conn.cstun->ext_ip.sin_family;
    conn.remote.sin_port = htons(atoi(argv[4]));

    printf("bind: %s [%d]\n", inet_ntoa(conn.remote.sin_addr), ntohs(conn.local.sin_port));

    std::thread recver(visichat_listener, &conn);
    std::thread sender(visichat_sender, &conn);

    sender.join();
    recver.join();

    conn.~udp_peer_connection();

    return 0;
}
//#endif