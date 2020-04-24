#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "err.h"

size_t is_status_200(char response[]) {
    return  strstr(buffer, "200 OK") != 0;
}

size_t print_status(char response[]) {
    print("%s", response);
}

void parse_request_resourse(char resource[], char target[], char host[]) {
    if (strncmp(resource, "http://", strlen("http://")) != 0) {
        fatal("not http");
    }
    if (strncmp(resource, "https://", strlen("https://")) == 0) {
        fatal("https");
    }
    char *target_start = strchr(resource + strlen("http://"), '/');
    if (!target_start) {
        strcpy(host, resource + strlen("http://"));
        strcpy(target, "/\0");
    }
    else {
        strncpy(host, resource + strlen("http://"), target_start - (resource + strlen("http://")));
        strcpy(target, target_start);
    }
}


size_t prepare_request_head(char *resource, char *request_head, char *request_head_len) {
    int request_head_len = 0;
    char host[HOST_SIZE];
    char target[HOST_SIZE];
    char get_header_template[] = "GET %s HTTP/1.1\n";
    char host_header_template[] = "Host: %s\n";
    char cookie_header_head[] = "Cookie: ";
    parse_request_resourse(resource, target, host);
    printf( "HOST %s\n", host);
    printf("TARGET %s\n", target);

    request_head_len = snprintf(request_head, REQUEST_HEAD_LEN, get_header_template, target);
    request_head_len += snprintf(request_head + request_head_len, REQUEST_HEAD_LEN, host_header_template, host);
    request_head_len += snprintf(request_head + request_head_len, REQUEST_HEAD_LEN, cookie_header_head);
//    request_head_len += snprintf(request_head + request_head_len, REQUEST_HEAD_LEN, connection_close_header);

    printf("%s", request_head);
    return request_head_len;
}

void prepare_close_header(char close_header[], size_t *close_header_len) {
    close_header = "\nConnection: close\n\n";
    *close_header_len = 0;
}
