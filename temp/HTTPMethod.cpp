#include "HTTP.h"
#include <fstream>
#include <sstream>

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
    } 
    else if (extension == "css") {
        http.setResponse().setHeader("Content-Type", "text/css; charset=utf-8");
    } 
    else if (extension == "js") {
        http.setResponse().setHeader("Content-Type", "application/javascript; charset=utf-8");
    } 
    else if (extension == "jpg" || extension == "jpeg") {
        http.setResponse().setHeader("Content-Type", "image/jpeg");
    } 
    else if (extension == "png") {
        http.setResponse().setHeader("Content-Type", "image/png");
    }
    
    // 응답 본문 설정
    http.setResponse().setStatus(StatusCode::OK);
    http.setResponse().setBody(content);
    
    return 0;
}

int postMethod(HTTP& http) {
    // POST 요청의 Content-Type 확인
    string contentType = http.getRequest().getHeaders().getHeader("Content-Type");
    if (contentType.find("application/json") == string::npos) {
        http.setResponse().setStatus(StatusCode::BAD_REQUEST);
        return 1;
    }

    // POST 데이터 파싱
    string body = http.getRequest().getBody().getBody();
    // JSON 파싱 및 처리
    // 여기서는 간단히 성공 응답만 반환
    
    http.setResponse().setStatus(StatusCode::CREATED);
    http.setResponse().setHeader("Content-Type", "application/json");
    http.setResponse().setBody("{\"status\":\"success\"}");
    
    return 0;
}