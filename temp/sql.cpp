#include "Database.h"
#include "HTTP.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// 이미지 저장 함수 추가
string saveImage(const string& base64Image) {
    // base64 디코딩 및 파일 저장은 나중에 구현
    return "";  // 임시로 빈 문자열 반환
}

int postMethod_1(HTTP& http) {
    Database db;
    
    // POST 데이터 파싱
    string body = http.getRequest().getBody().getBody();
    json data = json::parse(body);
    
    // 이미지 처리 (파일 저장)
    string imagePath = "";
    if (data.contains("image")) {
        imagePath = saveImage(data["image"]);
    }
    
    // DB에 저장
    bool success = db.insertPost(
        data["title"], 
        data["author"], 
        data["content"], 
        imagePath
    );
    
    if (success) {
        http.setResponse().setStatus(StatusCode::CREATED);
        http.setResponse().setBody("{\"status\":\"success\"}");
    } else {
        http.setResponse().setStatus(StatusCode::INTERNAL_SERVER_ERROR);
    }
    
    return 0;
}