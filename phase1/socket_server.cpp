#include "socket_sc.hpp"

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
    if(argc != 2) ERR_MSG("there should be 2 arguments");
    
    struct sockaddr_in cliaddr;
    int clilen;

    Request req_cli;
    init_server(atoi(argv[1])); // argv[1] is the port number

    fprintf(stderr, "\nstarting on %.80s, port %d, fd %d, maxconn %d...\n", srv.hostname, srv.port, srv.listen_fd, maxfd);
    if((req_cli.conn_fd = accept(srv.listen_fd, (struct sockaddr*)&cliaddr, (socklen_t*)&clilen)) < 0){
        fprintf(stderr, "accepting client failure");
    }

    read(req_cli.conn_fd, req_cli.buf, 512);
    printf("Client request: %s\n", req_cli.buf);

    req_cli.buf[sizeof(req_cli.buf)-2] = '\0';
    strcat(req_cli.buf, "| message sent");
    write(req_cli.conn_fd, req_cli.buf, sizeof(req_cli.buf));

    close(req_cli.conn_fd);
    shutdown(srv.listen_fd, SHUT_RDWR);
    return 0;
}