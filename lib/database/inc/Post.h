#ifndef POST_H
#define POST_H

#include <string>

struct Post {
    int id;
    std::string title;
    std::string author;
    std::string content;
    std::string imagePath;
    std::string createdAt;
};

#endif 