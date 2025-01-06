#include "HTTP.h"
#include <sstream>

using namespace std;

// static 멤버 변수 정의
map<string, int (*)(HTTP &http)> HTTP::methodMap;

#pragma region Request_StartLine
    Request_StartLine::Request_StartLine() {}
    Request_StartLine::~Request_StartLine() {}

    bool Request_StartLine::setStartLine(string startLine) {
        if(startLine.empty()) return false;

        istringstream iss(startLine);
        string method, target, version;
        
        iss >> method >> target >> version;

        if(method.empty() || target.empty() || version.empty()) return false;

        this->method = method;
        this->target = (target == "/") ? "/board.html" : target;
        this->version = version;

        return true;
    }

    string Request_StartLine::getMethod() const { return method; }
    string Request_StartLine::getTarget() const { return target; }
    string Request_StartLine::getVersion() const { return version; }

#pragma endregion

#pragma region Response_StatusLine
    void Response_StatusLine::setStatus(StatusCode code) {
        status = code;
        switch(code) {
            case StatusCode::OK: reason = "OK"; break;
            case StatusCode::CREATED: reason = "Created"; break;
            case StatusCode::BAD_REQUEST: reason = "Bad Request"; break;
            case StatusCode::UNAUTHORIZED: reason = "Unauthorized"; break;
            case StatusCode::FORBIDDEN: reason = "Forbidden"; break;
            case StatusCode::NOT_FOUND: reason = "Not Found"; break;
            case StatusCode::INTERNAL_SERVER_ERROR: reason = "Internal Server Error"; break;
        }
    }

    string Response_StatusLine::getStartLine() const {
        return version + " " + to_string(static_cast<int>(status)) + " " + reason;
    }
#pragma endregion

#pragma region Headers
    bool Headers::addHeader(const string& key, const string& value) {
        headers[key] = value;
        return true;
    }

    string Headers::getHeader(const string& key) const {
        auto it = headers.find(key);
        return (it != headers.end()) ? it->second : "";
    }

    bool Headers::hasHeader(const string& key) const {
        return headers.find(key) != headers.end();
    }

    const map<string, string>& Headers::getAllHeaders() const {
        return headers;
    }

    bool Headers::removeHeader(const string& key) {
        return headers.erase(key) > 0;
    }

    bool Headers::parseHeaders(string& headerLines) {
        istringstream iss(headerLines);
        string line;
        
        while (getline(iss, line)) {
            if (line.empty() || line == "\r") break;  // 헤더의 끝
            
            size_t colonPos = line.find(':');
            if (colonPos == string::npos) continue;  // 잘못된 형식
            
            string key = line.substr(0, colonPos);
            string value = line.substr(colonPos + 1);
            
            // 앞뒤 공백 제거
            value.erase(0, value.find_first_not_of(" "));
            value.erase(value.find_last_not_of("\r") + 1);
            
            addHeader(key, value);
        }
        return true;
    }
#pragma endregion

#pragma region Body
    void Body::setBody(string content) { body = content; }
    string Body::getBody() const { return body; }
#pragma endregion

#pragma region HTTP_Request
    bool HTTP_Request::setRequest(string & request) {
        if(request.empty()) return false;

        istringstream iss(request);
        string line;  
        string startLine;
        string headerSection;
        string bodyContent;
        
        // 시작줄 읽기
        getline(iss, startLine);
        if(!this->startLine.setStartLine(startLine)) return false;
        
        // 헤더 섹션 읽기
        while (getline(iss, line)) {
            if (line.empty() || line == "\r") break;  // 헤더의 끝
            headerSection += line + "\n";
        }
        
        // 헤더 파싱
        if(!this->headers.parseHeaders(headerSection)) return false;

        // Body 읽기
        while (getline(iss, line)) {
            bodyContent += line + "\n";
        }
        
        if (!bodyContent.empty()) {
            this->body.setBody(bodyContent);
        }

        return true;
    }

    Request_StartLine HTTP_Request::getStartLine() const { 
        return this->startLine; 
    }

#pragma endregion

#pragma region HTTP_Response
    HTTP_Response::HTTP_Response() {
        headers.addHeader("Server", "MyHTTPServer/1.0");
    }

    void HTTP_Response::setStatus(StatusCode code) {
        statusLine.setStatus(code);
        
        if(code >= StatusCode::BAD_REQUEST) {
            string errorBody = "<html><body><h1>" + 
                            to_string(static_cast<int>(code)) + " " +
                            "Error</h1></body></html>";
            setBody(errorBody);
            setContentType("text/html");
        }
    }

    void HTTP_Response::setBody(const string& content) {
        body.setBody(content);
        headers.addHeader("Content-Length", to_string(content.length()));
    }

    void HTTP_Response::setContentType(const string& type) {
        headers.addHeader("Content-Type", type);
    }

    string HTTP_Response::buildResponse () {
        string response = statusLine.getStartLine() + "\r\n";
        
        // 헤더 추가
        for(const auto& header : headers.getAllHeaders()) {
            response += header.first + ": " + header.second + "\r\n";
        }
        
        // 헤더와 바디 구분을 위한 빈 줄
        response += "\r\n";
        
        // 바디 추가
        response += body.getBody();
        
        return response;
    }
#pragma endregion

#pragma region HTTP
    bool HTTP::setRequest(string buffer) {
        if(buffer.empty()) return false;

        if(!this->request.setRequest(buffer)) {
            return false;
        }

        // 메서드 존재 여부 확인
        string method = request.getStartLine().getMethod();
        if(this->methodMap.find(method) == this->methodMap.end()) {
            response.setStatus(StatusCode::BAD_REQUEST);
            return false;
        }

        // 메서드 핸들러 실행
        if(this->methodMap[method](*this)) {
            response.setStatus(StatusCode::INTERNAL_SERVER_ERROR);
            return false;
        }

        response.setStatus(StatusCode::OK);  // 기본 상태 코드 설정
        return true;
    }

    string HTTP::getResponse() {
        return this->response.buildResponse();
    }
#pragma endregion