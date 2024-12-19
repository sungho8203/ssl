#include "HTTP.h"
#include <sstream>

using namespace std;

#pragma region Request_StartLine

    bool Request_StartLine::setStartLine(string startLine) {
        if(startLine.empty()) return false;

        istringstream iss(startLine);
        string method, target, version;
        
        iss >> method >> target >> version;

        if(method.empty() || target.empty() || version.empty()) return false;

        this->method = method;
        this->target = target;
        this->version = version;

        return true;
    }

#pragma endregion

#pragma region Response_StartLine

#pragma endregion

#pragma region Headers

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
#pragma endregion

#pragma region HTTP_Request

    bool HTTP_Request::setRequest(string & request) {
        if(request.empty()) return false;

        istringstream iss(request);
        string startLine;
        string headerSection;
        string line;
        
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

        return true;
    }

#pragma endregion

#pragma region HTTP_Response

#pragma endregion

#pragma region HTTP
    bool HTTP::setRequest(string & buffer) {
        if(buffer.empty()) return false;

        this->request.setRequest(buffer);
        return true;
    }
#pragma endregion