# 定义编译器
CXX = g++

# 定义编译器标志
CXXFLAGS = -std=c++11 -Iinclude

# 定义源文件目录和头文件目录
SOURCES = main.cpp
HEADERS = $(wildcard include/*.h)

# 生成目标文件列表
OBJECTS = $(SOURCES:.cpp=.o)

# 目标文件生成规则
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 生成可执行文件的规则
my_program: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 清理目标
clean:
	rm -f $(OBJECTS) my_program

