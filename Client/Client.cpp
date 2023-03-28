#include "Client.h"
#include "Threads.h"


using namespace std;


void sigpipe_handler(int signal)
{
    cout << "Received SIGPIPE signal" << endl;
}

void Client::initializeBuffer(char *msg2)
{
    memset(buffer, '\0', sizeof(buffer));
    memcpy(buffer, msg2, sizeof(msg2));
}

void Client::initiaziteSokets()
{
    string ipAddress = "127.0.0.1";
    int SERVER_PORT_NUM = 54321;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        cout << "Socket creation failed.";
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT_NUM);
    inet_pton(AF_INET, ipAddress.c_str(), &server_address.sin_addr);
}

void Client::connectToServer()
{
    int err = connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));

    if (err != 0)
    {
        cout << "Cant connect to the server. Attempts to reconnecting..." << endl;

        while (err != 0)
        {
            usleep(1000 * 1000);

            cout << "Try to reconnect " << endl;
            close(socket_fd);
            socket_fd = socket(AF_INET, SOCK_STREAM, 0);
            err = connect(socket_fd, (sockaddr *)&server_address, sizeof(server_address));
        }
    }
    cout << "Connected." << endl;
}

void Client::sendingToServer()
{
    send_size = send(socket_fd, buffer, sizeof(buffer), 0);

    if (send_size < 0)
    {
        cout << "Send error, trying to reconnect and resend" << endl;
        while (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) != 0)
        {
            usleep(1000 * 1000);
            cout << "Trying to reconnect" << endl;
            close(socket_fd);
            socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        }

        cout << "Reconnected" << endl;
        sendingToServer();
        cout << "Resended" << endl;
    }
}

void Client::recvFromServer()
{
    recv_size = recv(socket_fd, buffer, sizeof(buffer), 0);

    if (recv_size <= 0)
    {
        cout << "Server disconnected." << endl;
        close(socket_fd);
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        while (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) != 0)
        {
            usleep(1000 * 1000);
            cout << "Trying to reconnect" << endl;
            close(socket_fd);
            socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        }
        cout << "Reconnected" << endl;
        cout << "Try to resend" << endl;
        sendingToServer();
    }
    cout << "Recvived successfuly." << endl;
}

int main()
{

    signal(SIGPIPE, sigpipe_handler);

    Client cli;
    Threads_worker worker;
    
    while(1){

        worker.runThreads();

        cli.initializeBuffer(worker.msg2);

        cli.sendingToServer();

        //cli.recvFromServer();

    }
    

    return 0;
}

