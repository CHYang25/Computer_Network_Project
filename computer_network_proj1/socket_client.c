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
    char host[512];  // client's host
    int conn_fd;  // fd to talk with client
    char buf[512];  // data sent by/to client
    size_t buf_len;  // bytes used by buf
    int id;
    int wait_for_write;  
} Request;

int main(int argc, char **argv)
{
    Request req_cli;
    struct sockaddr_in serv_addr;
    char *msg = "I'm a handsome boy\n";
    if((req_cli.conn_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "socket err\n");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8087);

    if(connect(req_cli.conn_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        fprintf(stderr, "connect err\n");
    }
    write(req_cli.conn_fd, msg, strlen(msg));
    fprintf(stderr, "msg sent\n");

    close(req_cli.conn_fd);
    return 0;
}