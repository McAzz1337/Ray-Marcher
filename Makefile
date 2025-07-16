# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++20 -DGLM_FORCE_RADIANS -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -I./src/stb  
LDFLAGS = -lGL -lglfw -lGLEW 

# Project structure
SRC_DIR = src
BUILD_DIR = build
TARGET = main

# Find all .cpp files in src/
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Default rule
all: $(TARGET) clean_objs

# Link all object files
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile each .cpp file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@
# Create build directory if it does not exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Remove object files after build quietly
clean_objs:
	@rm -f $(OBJS)

# Clean all build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
