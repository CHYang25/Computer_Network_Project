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
#include <set>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream>
#include <tuple>
#include <filesystem>
#include <sys/stat.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
#include <cstring>
#define ERR_MSG(msg) fprintf(stderr, "%s\n", msg);

// #define 

class Client{
    public:
        struct sockaddr_in cliaddr;
        socklen_t clilen;
        int fd;
        void http_communication_procedure();
};

class Server{
    public:
        char server_host_name[512];
        uint16_t port;
        int fd;
        struct sockaddr_in servaddr;
        std::set<pid_t> child_pid;
        Server(int port_number);
        int accept_client(Client &cli);
        void add_child_pid(pid_t pid);
        void check_child_termination();
};

