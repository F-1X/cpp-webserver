#pragma once
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

using namespace std;


class Client {

    

    public:
        Client() {
            initiaziteSokets();
            connectToServer();
        }



        int socket_fd;
        struct sockaddr_in server_address;
        int err;
        int send_size;
        int recv_size;
        char buffer[1024];

        

        void initiaziteSokets();

        void connectToServer();

        void initializeBuffer(char *msg2);

        void sendingToServer();

        void recvFromServer();
    
};
