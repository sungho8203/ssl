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
    Request_StartLine();
    ~Request_StartLine();
    
    bool setStartLine(string startLine);
    
    string getMethod() const;
    string getTarget() const;
    string getVersion() const;
};


class Response_StatusLine {
private:
    string version;
    StatusCode status;
    string reason;

public:
    Response_StatusLine() : version("HTTP/1.1") {}
    
    void setVersion(string ver){version = ver;}
    void setStatus(StatusCode code);
    
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
    Request_StartLine getStartLine() const;
};


class HTTP_Response {
private:
    Response_StatusLine statusLine;
    Headers headers;
    Body body;

public:
    HTTP_Response();
    void setStatus(StatusCode code);
    void setBody(const string& content);
    void setContentType(const string& type);
   
    void setHeader(const string& key, const string& value){
        this->headers.addHeader(key, value);
    };

    string buildResponse ();
};


class HTTP{
private:
    HTTP_Request request;
    HTTP_Response response;
public:
    static map<string, int (*)(HTTP &http)> methodMap;

    bool setRequest(string buffer);
    string getResponse();

    HTTP_Request& getRequest()  { return request; }
    HTTP_Response& setResponse() { return response; }
};

#endif