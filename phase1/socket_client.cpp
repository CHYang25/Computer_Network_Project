#include "socket_sc.hpp"

int main(int argc, char **argv)
{
    // check on the arguments
    if(argc != 3) ERR_MSG("there should be three arguments");

    Request req_cli;
    struct sockaddr_in serv_addr;
    if((req_cli.conn_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // AF_INET: IPv6, SOCK_STREAM: TCP
        ERR_MSG("socket initialization error");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2])); // argv[2] is the port number
    inet_aton(argv[1], (in_addr*)&serv_addr.sin_addr.s_addr); // argv[1] is the server's IP

    if(connect(req_cli.conn_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        ERR_MSG("connection error")

    // read message
    char msg[512];
    printf("Enter your message: ");
    fgets(msg, sizeof(msg), stdin);
    msg[sizeof(msg)-1] = '\0';
    // scanf("%s", msg);

    // write message
    write(req_cli.conn_fd, msg, strlen(msg));

    read(req_cli.conn_fd, msg, sizeof(msg));
    printf("Server reply: %s\n", msg);
    // close connection file descriptor
    close(req_cli.conn_fd);
    return 0;
}