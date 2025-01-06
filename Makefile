CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
LDFLAGS =
INCLUDES = -Ilib/tcp/inc

# 디렉토리 설정
BUILD_DIR = build
SRC_DIRS = temp lib/tcp/src
TARGET = $(BUILD_DIR)/server

# 소스 파일들
SRCS = $(wildcard temp/*.cpp) $(wildcard lib/tcp/src/*.cpp)
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# 헤더 파일들
HEADERS = $(wildcard lib/tcp/inc/*.h)

# 기본 타겟을 release로 설정
all: release

# 개발/디버그용 빌드
debug: CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer
debug: LDFLAGS += -fsanitize=address
debug: $(TARGET)

# 배포용 빌드 (성능 최적화)
release: CXXFLAGS += -O2
release: $(TARGET)

# 빌드 디렉토리 생성
mkdir_build:
	@mkdir -p $(BUILD_DIR)/temp
	@mkdir -p $(BUILD_DIR)/lib/tcp/src

# 실행 파일 생성
$(TARGET): mkdir_build $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# 오브젝트 파일 생성
$(BUILD_DIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 청소
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# 실행
run: release
	./$(TARGET)

# 디버깅
debug_run: debug
	gdb ./$(TARGET)

.PHONY: all clean run debug release debug_run mkdir_build