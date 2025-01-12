#include "Database.h"

Database::Database() {
    try {
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "board_user", "Board123!@#");
        con->setSchema("board");
    } catch (sql::SQLException &e) {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
    }
}

Database::~Database() {
    if (con) {
        delete con;
    }
}

bool Database::insertPost(const string& title, const string& author, 
                        const string& content, const string& imagePath) {
    try {
        sql::PreparedStatement* prep_stmt;
        prep_stmt = con->prepareStatement(
            "INSERT INTO posts(title, author, content, image_path) VALUES (?, ?, ?, ?)");
        
        prep_stmt->setString(1, title);
        prep_stmt->setString(2, author);
        prep_stmt->setString(3, content);
        prep_stmt->setString(4, imagePath);
        
        prep_stmt->execute();
        delete prep_stmt;
        return true;
    } catch (sql::SQLException &e) {
        cout << "Error inserting post: " << e.what() << endl;
        return false;
    }
}