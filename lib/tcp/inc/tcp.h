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
    int server_fd;        // 서버 소켓
    int client_fd;        // 클라이언트 소켓
    int port;            // 포트 번호
    struct sockaddr_in server_addr;  // 서버 주소
    bool is_connected;   // 클라이언트 연결 상태

public:
    // 서버 생성 (포트만 필요)
    TCP(int port);
    ~TCP();

    // 서버 기능
    bool bind_and_listen();  // 서버 시작
    bool accept_client();    // 클라이언트 연결 수락
    bool disconnect();       // 클라이언트 연결 종료
    bool send(const string& data);  // 데이터 전송
    string receive(int buffer_size = 1024);  // 데이터 수신
    
    bool isConnected() const { return is_connected; }
};

#endif 