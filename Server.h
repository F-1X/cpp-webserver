#pragma once
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstring>


using namespace std;


class Server {
    
    public:
        Server(){
            initializeServer();
            bindSocket();
            listenConnection();
            acceptConnection();
        }

	int recv_size = 0;
	char msg[1024];

        int socket_fd;
        int client_conn;
        int client_address_size;
        sockaddr_in server_address;
        sockaddr_in client_address;


        void initializeServer();
        void bindSocket();
        void listenConnection();
        void acceptConnection();
        void serverLoop();
        void reconnectClient();


};
