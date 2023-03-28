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


void checher(char *msg)
{

	int len = strlen(msg);
	int number = atoi(msg);

	if ((len > 2) && !(number%32)){
		cout << "Recivied data: " << msg << endl;
	} else {
		cout << "Error message strlen <= 2 or remainder of division is not 32" << endl;
	}
}

int server()
{
	string ipAddress = "127.0.0.1";
	int SERVER_PORT_NUM = 54321;

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		cout << "Socket creation failed.";
	}
	
	const int enable = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	    perror("setsockopt(SO_REUSEADDR) failed");
	    
	sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	inet_pton(AF_INET, ipAddress.c_str(), &server_address.sin_addr);
	server_address.sin_port = htons(SERVER_PORT_NUM);

	int err = bind(socket_fd, (sockaddr *)&server_address, sizeof(server_address));

	if (err != 0)
	{
		cout << "Error Socket binding to server info. " << errno << endl;
		close(socket_fd);
		return 1;
	}
	else
		cout << "Binding socket to Server info is OK" << endl;

	err = listen(socket_fd, SOMAXCONN);

	if (err != 0)
	{
		cout << "Can't start to listen to." << endl;
		close(socket_fd);
		return 1;
	}
	else
	{
		cout << "Listening..." << endl;
	}

	sockaddr_in client_address;
	memset(&client_address, 0, sizeof(client_address));

	int client_address_size = sizeof(client_address);

	int client_conn = accept(socket_fd, (sockaddr *)&client_address, (socklen_t *)&client_address_size);
	if (err != 0)
		{

			while ((err = connect(socket_fd, (sockaddr *)&server_address, sizeof(server_address))))
			{
				cout << "Trying to connect to client" << endl;
				usleep(1000*1000);

			}

		}

	else
	{
		cout << "Connection to a client established successfully" << endl;
		char clientIP[22];
		inet_ntop(AF_INET, &client_address.sin_addr, clientIP, INET_ADDRSTRLEN);
		cout << "Client connected with IP address " << clientIP << endl;
	}

	int recv_size = 0;

	char msg[1024];
	

	while (true)
	{
		memset(&msg, 0, 1024);
		recv_size = recv(client_conn, msg, sizeof(msg), 0);

		checher(msg);
		

		if (recv_size <= 0)
		{

			cout << "Client disconnected. Waiting for reconnect." << endl;

			close(socket_fd);
			close(client_conn);

			socket_fd = socket(AF_INET, SOCK_STREAM, 0);

			err = bind(socket_fd, (sockaddr *)&server_address, sizeof(server_address));

			if (err == -1)
			{
				cout << "Error Socket binding." << endl;
				close(socket_fd);
				break;
			}
			else
				cout << "Binding socket is OK" << endl;
			err = listen(socket_fd, SOMAXCONN);
			if (err == -1)
			{
				cout << "Can't start to listen to." << endl;
				close(socket_fd);
				return 1;
			}
			else
			{
				cout << "Listening..." << endl;
			}
			cout << "Waiting accept" << endl;
			client_conn = accept(socket_fd, (sockaddr *)&client_address, (socklen_t *)&client_address_size);
			if (client_conn < 0){
				cout << "Error accepting";
			}
			
			cout << "Client reconnected" << endl;
			continue;
		}


	}

	close(socket_fd);
	close(client_conn);

	return 0;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	server();

	return 0;
}

