#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "err.h"


#define HOST_SIZE      256
#define PORT_SIZE      16

void parse_host_port(char *host, char *port, char *host_port){
    char *delimiter = strchr(host_port,':');
    int host_len = delimiter - host_port;
    if (host_len < 1) {
        fatal("host");
    }
    strncpy(host, host_port, host_len);
    host[host_len] = '\0';
    strcpy(port, host_port + host_len + 1);
    if (strlen(port) < 1) {
        fatal("port");
    }
}

void set_addr_hints(struct addrinfo *addr_hints) {
    memset(addr_hints, 0, sizeof(struct addrinfo));
    addr_hints->ai_flags = 0;
    addr_hints->ai_family = AF_INET;
    addr_hints->ai_socktype = SOCK_STREAM;
    addr_hints->ai_protocol = IPPROTO_TCP;
}

int set_connection(char host_port[]) {
    char host[HOST_SIZE];
    char port[PORT_SIZE];
    int rc;
    int sock;
    struct addrinfo addr_hints, *addr_result;

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        syserr("socket");
    }

    set_addr_hints(&addr_hints);

    parse_host_port(host, port, host_port);

    rc =  getaddrinfo(host, port, &addr_hints, &addr_result);
    if (rc != 0) {
        fprintf(stderr, "rc=%d\n", rc);
        syserr("getaddrinfo: %s", gai_strerror(rc));
    }

    if (connect(sock, addr_result->ai_addr, addr_result->ai_addrlen) != 0) {
        syserr("connect");
    }
    freeaddrinfo(addr_result);
    return sock;
}

size_t send_data(size_t sock, char buffer[], size_t buffer_size) {
    size_t nbytes_last, nbytes_total = 0;
    nbytes_total = 0;
    while (nbytes_total < buffer_size) {
        nbytes_last = write(sock, buffer + nbytes_total, buffer_size - nbytes_total);
        if (nbytes_last == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        nbytes_total += nbytes_last;
    }
}