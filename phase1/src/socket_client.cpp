#include "socket_sc.hpp"

int main(int argc, char **argv)
{
    // check on the arguments
    if(argc != 3) ERR_MSG("there should be three arguments");

    Request req_cli;
    struct sockaddr_in serv_addr;
    if((req_cli.fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // AF_INET: IPv6, SOCK_STREAM: TCP
        ERR_MSG("socket initialization error");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2])); // argv[2] is the port number
    inet_aton(argv[1], (in_addr*)&serv_addr.sin_addr.s_addr); // argv[1] is the server's IP

    req_cli.connection_socket((struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // read message from terminal
    char msg[512];
    req_cli.read_terminal_respond(msg);
    // close connection file descriptor
    close(req_cli.fd);
    return 0;
}