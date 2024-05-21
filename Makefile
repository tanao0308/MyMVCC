# 定义编译器
CXX = g++

# 定义编译器标志
CXXFLAGS = -Wall -std=c++11

# 定义目标可执行文件
TARGET = main

# 定义源文件
SRCS = main.cpp

# 定义对象文件（将 .cpp 文件的扩展名替换为 .o）
OBJS = $(SRCS:.cpp=.o)

# 默认目标
all: $(TARGET)

# 规则：链接对象文件以创建可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# 规则：编译 .cpp 文件为 .o 文件
%.o: %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 规则：清理生成的文件
clean:
	rm -f $(OBJS) $(TARGET) *.gch *.out

# 显示构建内容的详细信息
.PHONY: all clean

