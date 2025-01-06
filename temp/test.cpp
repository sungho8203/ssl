#include "tcp.h"
#include "HTTP.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>

using namespace std;

int getMethod(HTTP& http){
    string target = http.getRequest().getStartLine().getTarget();
    
    // 웹 루트 디렉토리 설정 (예: "web" 폴더)
    string webRoot = "temp/web";
    string filePath = webRoot + target;
    
    // 파일 읽기 시도
    ifstream file(filePath, ios::binary);
    if (!file) {
        // 파일을 찾을 수 없는 경우 404 응답
        http.setResponse().setStatus(StatusCode::NOT_FOUND);
        http.setResponse().setBody("404 Not Found");
        return -1;
    }
    
    // 파일 내용을 문자열로 읽기
    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    
    // Content-Type 설정 (간단한 예시)
    string extension = target.substr(target.find_last_of(".") + 1);
    if (extension == "html") {
        http.setResponse().setHeader("Content-Type", "text/html; charset=utf-8");
    } else if (extension == "css") {
        http.setResponse().setHeader("Content-Type", "text/css; charset=utf-8");
    } else if (extension == "js") {
        http.setResponse().setHeader("Content-Type", "application/javascript; charset=utf-8");
    } else if (extension == "jpg" || extension == "jpeg") {
        http.setResponse().setHeader("Content-Type", "image/jpeg");
    } else if (extension == "png") {
        http.setResponse().setHeader("Content-Type", "image/png");
    }
    
    // 응답 본문 설정
    http.setResponse().setStatus(StatusCode::OK);
    http.setResponse().setBody(content);
    
    return 0;
}

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