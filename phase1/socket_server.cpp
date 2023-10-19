#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>

typedef struct {
    char hostname[512];
    uint16_t port;
    int listen_fd;
} Server;

typedef struct {
    char host[512];  // client's host
    int conn_fd;  // fd to talk with client
    char buf[512];  // data sent by/to client
    size_t buf_len;  // bytes used by buf
    int id;
    int wait_for_write;  
} Request;

Server srv;
Request *requests = NULL; // this table is used for phase 2 project
int maxfd;

static void init_request(Request* req)
{
    req->conn_fd = -1;
    req->buf_len = 0;
    req->id = 0;
}

static void init_server(uint16_t port)
{
    struct sockaddr_in servaddr;
    int tmp;

    gethostname(srv.hostname, sizeof(srv.hostname));
    srv.port = port;

    srv.listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (srv.listen_fd < 0) fprintf(stderr, "socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    tmp = 1;
    if (setsockopt(srv.listen_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&tmp, sizeof(tmp)) < 0) {
        fprintf(stderr, "setsockopt");
    }
    if (bind(srv.listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr, "bind");
    }
    if (listen(srv.listen_fd, 1024) < 0) {
        fprintf(stderr, "listen");
    }

    // Get file descripter table size and initialize request table
    maxfd = getdtablesize();
    requests = (Request*) malloc(sizeof(Request) * maxfd);
    if (requests == NULL) {
        fprintf(stderr, "out of memory allocating all requests");
    }
    for (int i = 0; i < maxfd; i++) {
        init_request(&requests[i]);
    }
    requests[srv.listen_fd].conn_fd = srv.listen_fd;
    strcpy(requests[srv.listen_fd].host, srv.hostname);
    
    return;
}

int main(int argc, char **argv)
{
    struct sockaddr_in cliaddr;
    int clilen;

    Request req_cli;
    init_server(8087);

    fprintf(stderr, "\nstarting on %.80s, port %d, fd %d, maxconn %d...\n", srv.hostname, srv.port, srv.listen_fd, maxfd);
    if((req_cli.conn_fd = accept(srv.listen_fd, (struct sockaddr*)&cliaddr, (socklen_t*)&clilen)) < 0){
        fprintf(stderr, "accepting client failure");
    }

    read(req_cli.conn_fd, req_cli.buf, 512);
    printf("%s\n", req_cli.buf);

    close(req_cli.conn_fd);
    shutdown(srv.listen_fd, SHUT_RDWR);
    return 0;
}