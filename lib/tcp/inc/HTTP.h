#ifndef HTTPS_H
#define HTTPS_H

#include <string>
#include <map>

#include "type.h"

using namespace std;

class Request_StartLine {
private:
    string method;
    string target;
    string version;

public:
    map<string, int (*)()> methodMap;

    Request_StartLine() {}
    ~Request_StartLine() {}

    bool setStartLine(string startLine);
};

class Response_StartLine {
private:
    string version;
    StatusCode status;
    string reason;
};

class Headers {
private:
    map<string, string> headers;

public:
    bool addHeader(const string& key, const string& value) {
        headers[key] = value;
        return true;
    }
    
    bool parseHeaders(string& headerLines);
};

class Body {
private:
    string body;

public:
    void setBody(string content) { body = content; }
};

class HTTP_Request {
private:
    Request_StartLine startLine;
    Headers headers;
    Body body;

public:
    bool setRequest(string & request);
};

class HTTP_Response {
private:
    Response_StartLine startLine;
    Headers headers;
    Body body;

public:
};

class HTTP{
private:
    HTTP_Request request;
    HTTP_Response response;
public:
    bool setRequest(string & buffer);
};

#endif