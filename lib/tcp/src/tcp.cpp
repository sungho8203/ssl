#include "tcp.h"
#include <iostream>
#include <cstring>

// Client 클래스 메소드 정의
bool Client::disconnect() {
    if (!is_connected || fd < 0) return false;
    if (fd >= 0) {
        close(fd);
        fd = -1;
    }
    is_connected = false;
    delete this;
    return true;
}

bool Client::send(const string& data) {
    if (!is_connected || fd < 0) return false;
    ssize_t bytes_sent = ::send(fd, data.c_str(), data.length(), 0);
    return bytes_sent == static_cast<ssize_t>(data.length());
}

string Client::receive(int buffer_size) {
    if (!is_connected || fd < 0) return "";
    char* buffer = new char[buffer_size];
    string result;
    ssize_t bytes_received = recv(fd, buffer, buffer_size - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        result = string(buffer);
    }
    delete[] buffer;
    return result;
}

Client::Client(int socket_fd) : fd(socket_fd), is_connected(true) {
}

Client::~Client() {
    if (is_connected) disconnect();
}

// Server 클래스 메소드 정의
Server::Server(int port) : fd(-1), port(port) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        cerr << "Error creating socket" << endl;
        return;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
}

Server::~Server() {
    if (fd >= 0) close(fd);
}

bool Server::bind_and_listen() {
    if (fd < 0) return false;

    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        cerr << "setsockopt failed" << endl;
        return false;
    }

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        cerr << "Bind failed" << endl;
        return false;
    }

    if (listen(fd, 3) < 0) {
        cerr << "Listen failed" << endl;
        return false;
    }

    return true;
}

Client* Server::accept_client() {
    if (fd < 0) return nullptr;

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        cerr << "Accept failed: " << strerror(errno) << endl;
        return nullptr;
    }

    return new Client(client_fd);
}

// TCP 클래스 메소드 정의
TCP::TCP(int port) : server(new Server(port)), client(nullptr) {
}

TCP::~TCP() {
    delete server;
    delete client;
}

bool TCP::bind_and_listen() {
    return server->bind_and_listen();
}

bool TCP::accept_client() {
    if (client) {
        delete client;
        client = nullptr;
    }
    client = server->accept_client();
    return client != nullptr;
}

bool TCP::send(const string& data) {
    return client ? client->send(data) : false;
}

string TCP::receive(int buffer_size) {
    return client ? client->receive(buffer_size) : "";
}

bool TCP::disconnect() {
    return client ? client->disconnect() : false;
} 

Client* TCP::getClient() {
    Client* temp = client;
    client = nullptr;
    return temp;
}