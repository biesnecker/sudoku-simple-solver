# Define the compiler
CXX := g++

# Define compile-time flags
CXXFLAGS := --std=c++20 -O3 -march=native

# Define the target executable
TARGET := simple_solver

# Define source files
SOURCES := main.cpp solve.cpp

# Define object files
OBJECTS := $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Rule for linking the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule for compiling source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for cleaning up
clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
