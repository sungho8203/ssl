#ifndef DATABASE_H
#define DATABASE_H

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <string>
#include <vector>
#include "Post.h"
using namespace std;

class Database {
private:
    sql::Driver* driver;
    sql::Connection* con;
    
public:
    Database();
    ~Database();
    
    bool insertPost(const string& title, const string& author, 
                   const string& content, const string& imagePath);
    vector<Post> getPosts();
};

#endif