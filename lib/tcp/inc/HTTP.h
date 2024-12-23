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

    Request_StartLine();
    ~Request_StartLine();
    bool setStartLine(string startLine);
    string getMethod() const;
    string getTarget() const;
    string getVersion() const;
};

class Response_StartLine {
private:
    string version;
    StatusCode status;
    string reason;

public:
    Response_StartLine();
    void setVersion(string ver);
    void setStatus(StatusCode code);
    void setReason(string msg);
    string getStartLine() const;
};

class Headers {
private:
    map<string, string> headers;

public:
    bool parseHeaders(string& headerLines);
    bool addHeader(const string& key, const string& value);
    string getHeader(const string& key) const;
    bool hasHeader(const string& key) const;
    string getContentType() const;
    const map<string, string>& getAllHeaders() const;
    bool removeHeader(const string& key);
};

class Body {
private:
    string body;

public:
    void setBody(string content);
    string getBody() const;
};

class HTTP_Request {
private:
    Request_StartLine startLine;
    Headers headers;
    Body body;

public:
    bool setRequest(string & request);
    string getPath() const {
        string target = startLine.getTarget();
        return target == "/" ? "/index.html" : target;
    }
};

class HTTP_Response {
private:
    Response_StartLine startLine;
    Headers headers;
    Body body;

public:
    HTTP_Response();
    void setStatus(StatusCode code);
    void setBody(const string& content);
    void setContentType(const string& type);
    string buildResponse();
};

class HTTP{
private:
    HTTP_Request request;
    HTTP_Response response;
public:
    bool setRequest(string & buffer);
    const HTTP_Request& getRequest() const { return request; }
};

#endif