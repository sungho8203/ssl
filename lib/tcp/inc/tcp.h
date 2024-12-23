#ifndef TCP_H
#define TCP_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

class TCP {
private:
    int sockfd;
    int client_sockfd;
    string ip;
    int port;
    struct sockaddr_in server_addr;
    bool is_connected;
    bool is_server;

public:
    TCP(const string& ip, int port, bool server = false);
    ~TCP();

    bool bind_and_listen();
    bool accept_client();
    bool connect();
    bool disconnect();
    bool send(const string& data);
    string receive(int buffer_size = 1024);
    
    bool isConnected() const { return is_connected; }
    int getSocket() const { return is_server ? client_sockfd : sockfd; }
};

#endif 