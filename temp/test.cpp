#include "HTTPMethod.h"

#include "tcp.h"
#include "HTTP.h"

#include <iostream>
#include <vector>
#include <thread>

using namespace std;

// 클라이언트 요청 처리 함수
void handleClient(Client *client) {
    HTTP http;

    string received = client->receive();
    cout << "Received request:" << endl << received << endl;

    bool isSet = http.setRequest(received);
    if(!isSet) cout << "error" << endl;
    
    string response = http.getResponse();
    cout << "Sending response..." << endl;
    client->send(response);
    
    client->disconnect();
}

int main(){
    TCP server(8080);
    Client *client = nullptr;
    HTTP::methodMap["GET"] = getMethod;
    HTTP::methodMap["POST"] = postMethod;
    server.bind_and_listen();

    while(true){
        // 새로운 클라이언트 연결 대기
        if (!server.accept_client()) {
            cout << "Failed to accept client" << endl;
            continue;
        }
        cout << "Client connected" << endl;
        client = server.getClient();

        // 새로운 쓰레드 생성하여 클라이언트 처리
        thread(handleClient, client).detach();
    }

    return 0;
}