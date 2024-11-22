#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <time.h>

int main(int argc, char *argv[]) {
     if (argc < 2)
        return -1;

    int fd, ret = 1;
    struct sockaddr_in r_addr;
    struct sockaddr_in s_addr;
    short port;
    uint8_t buf[512];

    srand(time(NULL));
    


    r_addr.sin_addr.s_addr = htonl(inet_network(argv[1]));
    r_addr.sin_port = htons(atoi(argv[2]));
    r_addr.sin_family = AF_INET;

    printf("ip addr: %s port: %d\n", inet_ntoa(r_addr.sin_addr), ntohs(r_addr.sin_port));

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
        return -1;


    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(ret));
    setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &ret, sizeof(ret));


    s_addr.sin_addr.s_addr = INADDR_ANY;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(rand() % 0xffff + 2049);

    ret = bind(fd, (struct sockaddr *)&s_addr, sizeof(s_addr));
    if (ret < 0)
        goto clean_fd;

    ret = sendto(fd, buf, 512, 0, (struct sockaddr *)&r_addr, sizeof(r_addr));
    if (ret < 0)
        goto clean_fd;

clean_fd:
    close(fd);
    return 0;
}