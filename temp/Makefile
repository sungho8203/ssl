CXX = g++
CXXFLAGS = -std=c++11 -Wall
INCLUDES = -I../lib/tcp/inc -I.

# 디렉토리 설정
BUILD_DIR = build
SRC_DIR = .
LIB_SRC_DIR = ../lib/tcp/src

# 소스 파일들
SRCS = $(SRC_DIR)/test.cpp $(LIB_SRC_DIR)/HTTP.cpp $(LIB_SRC_DIR)/tcp.cpp
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
TARGET = $(BUILD_DIR)/test

# 헤더 파일들
HEADERS = ../lib/tcp/inc/HTTP.h ../lib/tcp/inc/type.h

# 기본 타겟
all: mkdir_build $(TARGET)

# 빌드 디렉토리 생성
mkdir_build:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(LIB_SRC_DIR)

# 실행 파일 생성
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# 오브젝트 파일 생성
$(BUILD_DIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 청소
clean:
	rm -rf $(BUILD_DIR)

# 빌드 및 실행
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run mkdir_build