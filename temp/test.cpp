#include "tcp.h"
#include "HTTP.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// 파일 읽기 함수
string readFile(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        return "";
    }
    
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// HTTP 요청 처리 함수
HTTP_Response handleRequest(const string& request) {
    HTTP http;
    HTTP_Response response;
    
    // 요청 파싱 (const 제거를 위해 복사)
    string req_copy = request;
    if (!http.setRequest(req_copy)) {
        response.setStatus(StatusCode::BAD_REQUEST);
        return response;
    }
    
    // index.html 읽기
    string content = readFile("web/index.html");
    if (content.empty()) {
        response.setStatus(StatusCode::NOT_FOUND);
        response.setBody("<html><body><h1>404 Not Found</h1></body></html>");
    } else {
        response.setStatus(StatusCode::OK);
        response.setContentType("text/html");
        response.setBody(content);
    }
    
    return response;
}

int main() {
    TCP tcp("0.0.0.0", 8080, true);  // 서버 모드로 생성
    
    cout << "Server starting on port 8080" << endl;
    
    if (!tcp.bind_and_listen()) {
        cerr << "Failed to start server" << endl;
        return 1;
    }
    
    while (true) {
        // 클라이언트 연결 대기
        if (!tcp.accept_client()) {
            cerr << "Failed to accept connection" << endl;
            continue;
        }
        
        cout << "Client connected" << endl;
        
        // 요청 받기
        string request = tcp.receive();
        if (request.empty()) {
            tcp.disconnect();
            continue;
        }
        
        cout << "Received request:" << endl << request << endl;
        
        // 요청 처리 및 응답 생성
        HTTP_Response response = handleRequest(request);
        string responseStr = response.buildResponse();
        
        // 응답 전송
        tcp.send(responseStr);
        cout << "Sent response:" << endl << responseStr << endl;
        
        // 연결 종료
        tcp.disconnect();
        cout << "Connection closed" << endl << endl;
    }
    
    return 0;
}
