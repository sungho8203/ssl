CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
INCLUDES = -Ilib/tcp/inc

# 디렉토리 설정
BUILD_DIR = build
SRC_DIRS = temp lib/tcp/src
TARGET = server

# 소스 파일들
SRCS = $(wildcard temp/*.cpp) $(wildcard lib/tcp/src/*.cpp)
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# 헤더 파일들
HEADERS = $(wildcard lib/tcp/inc/*.h)

# 기본 타겟
all: mkdir_build $(TARGET)

# 빌드 디렉토리 생성
mkdir_build:
	@mkdir -p $(BUILD_DIR)/temp
	@mkdir -p $(BUILD_DIR)/lib/tcp/src

# 실행 파일 생성
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# 오브젝트 파일 생성
$(BUILD_DIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 청소
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# 실행
run: $(TARGET)
	./$(TARGET)

# 디버깅
debug: $(TARGET)
	gdb ./$(TARGET)

.PHONY: all clean run debug mkdir_build 