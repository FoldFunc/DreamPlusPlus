CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -O2

SRC := src
BUILD := build
TARGET := $(BUILD)/compiler

SRCS := $(shell find $(SRC) -name "*.cpp")
OBJS := $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(SRCS))

.PHONY: all clean

all : $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -rf $(BUILD)
