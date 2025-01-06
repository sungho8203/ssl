#include "tcp.h"
#include <iostream>
#include <cstring>

TCP::TCP(int port) 
    : server_fd(-1), client_fd(-1), port(port), is_connected(false) {
    
    // 서버 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        cerr << "Error creating socket" << endl;
        return;
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // 모든 인터페이스에서 연결 허용
    server_addr.sin_port = htons(port);
}

TCP::~TCP() {
    if (is_connected) {
        disconnect();
    }
    if (server_fd >= 0) {
        close(server_fd);
    }
}

bool TCP::bind_and_listen() {
    if (server_fd < 0) return false;

    // SO_REUSEADDR 옵션 설정
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        cerr << "setsockopt failed" << endl;
        return false;
    }

    // bind
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Bind failed" << endl;
        return false;
    }

    // listen
    if (listen(server_fd, 3) < 0) {
        cerr << "Listen failed" << endl;
        return false;
    }

    return true;
}

bool TCP::accept_client() {
    if (server_fd < 0) return false;

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        cerr << "Accept failed" << endl;
        return false;
    }

    is_connected = true;
    return true;
}

bool TCP::disconnect() {
    if (!is_connected) return false;
    
    if (client_fd >= 0) {
        close(client_fd);
        client_fd = -1;
    }
    
    is_connected = false;
    return true;
}

bool TCP::send(const string& data) {
    if (!is_connected || client_fd < 0) return false;
    
    ssize_t bytes_sent = ::send(client_fd, data.c_str(), data.length(), 0);
    return bytes_sent == static_cast<ssize_t>(data.length());
}

string TCP::receive(int buffer_size) {
    if (!is_connected || client_fd < 0) return "";
    
    char* buffer = new char[buffer_size];
    string result;
    
    ssize_t bytes_received = recv(client_fd, buffer, buffer_size - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        result = string(buffer);
    }
    
    delete[] buffer;
    return result;
} 