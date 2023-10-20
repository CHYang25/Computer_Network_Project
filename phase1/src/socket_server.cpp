#include "socket_sc.hpp"

int main(int argc, char **argv)
{
    if(argc != 2) ERR_MSG("there should be 2 arguments");

    Server *srv = NULL;
    Request *requests = NULL; // this table is used for phase 2 project
    int maxfd;
    
    srv = new Server(atoi(argv[1])); // argv[1] is the port number
    maxfd = getdtablesize();
    requests = new Request[maxfd];
    requests[srv->fd].set_fd(srv->fd);
    strcpy(requests[srv->fd].host, srv->hostname);

    struct sockaddr_in cliaddr;
    int clilen;
    Request req_cli;

    // print startup message
    srv->startup_message(maxfd);
    // accept a client
    req_cli.accept_socket(srv->fd, (struct sockaddr*)&cliaddr, (socklen_t*)&clilen);
    // read the message from req_cli and respond
    req_cli.read_socket_respond();
    // close the client's fd
    close(req_cli.fd);
    // shutdown
    shutdown(srv->fd, SHUT_RDWR);
    return 0;
}