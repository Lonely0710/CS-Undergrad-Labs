# 编译器和编译选项
CXX = g++
CXXFLAGS = -Wall -g -std=c++11

# 目标文件
TARGET = CP_Lab5

# 源文件
SRCS = main.cpp SLR1.cpp

# 头文件
HEADERS = SLR1.h

# 对应的目标文件
OBJS = $(SRCS:.cpp=.o)

# 默认目标
all: $(TARGET)

# 链接目标文件生成最终可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# 编译 .cpp 文件为 .o 文件
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理目标文件和生成的可执行文件
clean:
	rm -f $(OBJS) $(TARGET)

# 伪目标，确保不会和文件名冲突
.PHONY: all clean
