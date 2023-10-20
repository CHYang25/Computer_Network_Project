#include "socket_sc.hpp"

Server srv;
Request *requests = NULL; // this table is used for phase 2 project
int maxfd;

static void init_request(Request* req)
{
    req->fd = -1;
    req->buf_len = 0;
    req->id = 0;
}

static void init_server(uint16_t port)
{
    struct sockaddr_in servaddr;
    int tmp;

    // initialize server socket
    gethostname(srv.hostname, sizeof(srv.hostname));
    srv.port = port;

    srv.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (srv.fd < 0) ERR_MSG("socket");

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // to accept any request
    servaddr.sin_port = htons(port);
    tmp = 1;
    if (setsockopt(srv.fd, SOL_SOCKET, SO_REUSEADDR, (void*)&tmp, sizeof(tmp)) < 0) 
        ERR_MSG("setsockopt");
    if (bind(srv.fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
        ERR_MSG("bind");
    if (listen(srv.fd, 1024) < 0) 
        ERR_MSG("listen");

    // Initialize the request table, for phase 2 project
    maxfd = getdtablesize();
    requests = (Request*) malloc(sizeof(Request) * maxfd);
    if (!requests)
        ERR_MSG("allocation error");
    
    for (int i = 0; i < maxfd; i++) 
        init_request(&requests[i]);
    
    requests[srv.fd].fd = srv.fd;
    strcpy(requests[srv.fd].host, srv.hostname);
    
    return;
}

int main(int argc, char **argv)
{
    if(argc != 2) ERR_MSG("there should be 2 arguments");
    
    struct sockaddr_in cliaddr;
    int clilen;

    Request req_cli;
    init_server(atoi(argv[1])); // argv[1] is the port number

    fprintf(stderr, "starting on %.80s, port %d, fd %d, maxconn %d...\n", srv.hostname, srv.port, srv.fd, maxfd);
    if((req_cli.fd = accept(srv.fd, (struct sockaddr*)&cliaddr, (socklen_t*)&clilen)) < 0){
        ERR_MSG("accepting client failure");
    }

    read(req_cli.fd, req_cli.buf, 512);
    printf("Client request: %s\n", req_cli.buf);

    strcat(req_cli.buf, " | message sent");
    write(req_cli.fd, req_cli.buf, sizeof(req_cli.buf));

    close(req_cli.fd);
    shutdown(srv.fd, SHUT_RDWR);
    return 0;
}