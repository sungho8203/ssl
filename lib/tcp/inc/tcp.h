#ifndef TCP_H
#define TCP_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

class Client {
private:
    int fd;
    bool is_connected;

public:
    Client(int socket_fd);
    ~Client();

    bool disconnect();
    bool send(const string& data);
    string receive(int buffer_size = 1024);
    bool isConnected() const;
};

class Server {
private:
    int fd;
    int port;
    struct sockaddr_in addr;

public:
    Server(int port);
    ~Server();

    bool bind_and_listen();
    Client* accept_client();
};

class TCP {
private:
    Server* server;
    Client* client;

public:
    TCP(int port);
    ~TCP();

    bool bind_and_listen();
    bool accept_client();
    bool send(const string& data);
    string receive(int buffer_size = 1024);
    bool disconnect();

    Client* getClient();
};

#endif