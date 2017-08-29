INCLUDE_DIR = include
SRC_DIR = src
BUILD_DIR = build
CPP_FLAGS = -std=c++11 -O3 -I$(INCLUDE_DIR) -Wall
OBJECTS = $(BUILD_DIR)/game.o $(BUILD_DIR)/blue_black_game.o \
	$(BUILD_DIR)/agent.o
TARGET = $(BUILD_DIR)/bbgame.bin

all: $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	g++ -c $(CPP_FLAGS) -o $@ $<

$(TARGET): $(SRC_DIR)/main.cpp $(OBJECTS)
	g++ $(CPP_FLAGS) -o $@ $^

clean:
	rm -f build/*
