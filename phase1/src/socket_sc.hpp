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
#define ERR_MSG(msg) fprintf(stderr, "%s\n", msg);

class Server{
    public:
        char hostname[512];
        uint16_t port;
        int fd;
        Server(uint16_t port){
            struct sockaddr_in servaddr;
            int tmp;

            // initialize server socket
            gethostname(this->hostname, sizeof(this->hostname));
            this->port = port;

            this->fd = socket(AF_INET, SOCK_STREAM, 0);
            if(this->fd < 0) ERR_MSG("socket");

            servaddr.sin_family = AF_INET;
            servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // to accept any request
            servaddr.sin_port = htons(port);
            tmp = 1;
            if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, (void*)&tmp, sizeof(tmp)) < 0) 
                ERR_MSG("setsockopt");
            if (bind(this->fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
                ERR_MSG("bind");
            if (listen(this->fd, 1024) < 0) 
                ERR_MSG("listen");
        }
        void startup_message(int max_fd){
            // Show startup message
            fprintf(stderr, "\nstarting on host: %.80s; port: %d; fd: %d; maxconn %d...\n", this->hostname, this->port, this->fd, max_fd);
        }
    private:
};

class Request{
    public:
        char host[512];  // client's host
        int fd;  // fd to talk with client
        char buf[512];  // data sent by/to client
        size_t buf_len;  // bytes used by buf
        int id;
        int wait_for_write;  
        Request(){
            this->fd = -1;
            this->buf_len = 0;
            this->id = 0;
        }
        void set_fd(int fd){
            this->fd = fd;
        }

        // for server to accept a connection
        void accept_socket(int fd, struct sockaddr* cliaddr, socklen_t* clilen){
            if((this->fd = accept(fd, cliaddr, clilen)) < 0)
                ERR_MSG("accepting client failure");
        }
        // for server to read from socket and respond to socket
        void read_socket_respond(){
            read(this->fd, this->buf, 512);
            printf("Client request: %s\n", this->buf);

            strcat(this->buf, " | message sent");
            write(this->fd, this->buf, sizeof(this->buf));
        }

        // for client to connect to a remote socket
        void connection_socket(struct sockaddr* servaddr, socklen_t srvlen){
            if(connect(this->fd, servaddr, srvlen) < 0)
                ERR_MSG("connection error")
        }
        // for client to read from terminal and write to socket and show the respond
        void read_terminal_respond(char *msg){
            printf("Enter your message: ");
            fgets(msg, sizeof(msg), stdin);
            msg[strlen(msg)-1] = '\0';

            // write message to socket
            write(this->fd, msg, strlen(msg));

            read(this->fd, msg, 512);
            printf("Server reply: %s\n", msg);
        }
    private:
};