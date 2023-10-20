# Computer Network Project Phase 1
written by Chih-Han Yang - B10902069

## Directory Structure
This is the directory structure of this pacakge:
```
phase1
├── client
├── demo.webm
├── Makefile
├── README.md
├── run.sh
├── server
└── src
    ├── socket_client.cpp
    ├── socket_sc.hpp
    └── socket_server.cpp
```
1. ```client```: The executable for client host process.
2. ```server```: The executable for server host process.
3. ```Makefile```: It compiles all the source files into executables.
4. ```run.sh```: The script file for execution.
5. ```README.md```: It describes the package in detail.
6. ```src```: The directory contains the source files:
    - ```socket_client.cpp```: client source file.
    - ```socket_server.cpp```: server source file.
    - ```socket_sc.hpp```: the header file.
7. ```demo.webm```: Demo video.

## Execution
### Compile
To compile the source files, use the ```Makefile``` to do so:
```
$ make
```
### Execution:
1. To execute the client:
```
$ ./client <server_IP_address> <port_number>
```
2. To execute the server:
```
$ ./server <port_number>
```
3. Note that the server should be deployed before client. Otherwise, client would get a connection error.
4. Or you can just deploy the shell script ```run.sh```. Modify the IP address and port number if needed. The current IP address used here is ```ws1.csie.ntu.edu.tw```, port number is ```8087```:
```
# For server
$ ./run.sh server

# For client
$ ./run.sh client
```