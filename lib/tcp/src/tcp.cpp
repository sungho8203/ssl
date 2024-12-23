#include "tcp.h"
#include <iostream>
#include <cstring>

TCP::TCP(const string& ip, int port, bool server) 
    : ip(ip), port(port), sockfd(-1), client_sockfd(-1), is_connected(false), is_server(server) {
    
    // 소켓 생성
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cerr << "Error creating socket" << endl;
        return;
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (server) {
        // 서버 모드: 모든 인터페이스에서 연결 허용
        server_addr.sin_addr.s_addr = INADDR_ANY;
    } else {
        // 클라이언트 모드: 특정 IP로 연결
        if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
            cerr << "Invalid address" << endl;
            close(sockfd);
            sockfd = -1;
            return;
        }
    }
}

TCP::~TCP() {
    if (is_connected) {
        disconnect();
    }
    if (sockfd >= 0) {
        close(sockfd);
    }
}

bool TCP::connect() {
    if (sockfd < 0) return false;
    
    if (::connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Connection failed" << endl;
        return false;
    }
    
    is_connected = true;
    return true;
}

bool TCP::disconnect() {
    if (!is_connected) return false;
    
    if (is_server && client_sockfd >= 0) {
        close(client_sockfd);
        client_sockfd = -1;
    } else {
        close(sockfd);
        sockfd = -1;
    }
    
    is_connected = false;
    return true;
}

bool TCP::send(const string& data) {
    if (!is_connected) return false;
    
    int fd = is_server ? client_sockfd : sockfd;
    ssize_t bytes_sent = ::send(fd, data.c_str(), data.length(), 0);
    return bytes_sent == static_cast<ssize_t>(data.length());
}

string TCP::receive(int buffer_size) {
    if (!is_connected) return "";
    
    char* buffer = new char[buffer_size];
    string result;
    
    int fd = is_server ? client_sockfd : sockfd;
    ssize_t bytes_received = recv(fd, buffer, buffer_size - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        result = string(buffer);
    }
    
    delete[] buffer;
    return result;
} 

bool TCP::bind_and_listen() {
    if (!is_server || sockfd < 0) return false;

    // SO_REUSEADDR 옵션 설정
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        cerr << "setsockopt failed" << endl;
        return false;
    }

    // bind
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Bind failed" << endl;
        return false;
    }

    // listen
    if (listen(sockfd, 3) < 0) {
        cerr << "Listen failed" << endl;
        return false;
    }

    return true;
}

bool TCP::accept_client() {
    if (!is_server || sockfd < 0) return false;

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
    if (client_sockfd < 0) {
        cerr << "Accept failed" << endl;
        return false;
    }

    is_connected = true;
    return true;
} 