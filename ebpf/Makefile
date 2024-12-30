# Makefile
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

TARGET = tcp_tracer
SRCS = main.cpp parse_value.cpp tcp_tracer.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

