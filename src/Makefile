CXX = g++

CXXFLAGS = -std=c++17

INCLUDES = -I$(CONDA_PREFIX)/include/opencv4
LIBS = -L$(CONDA_PREFIX)/lib -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc

TARGET = exec

SRCS = quadtreenode.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)