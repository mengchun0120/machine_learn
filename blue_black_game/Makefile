INCLUDE_DIR = include
SRC_DIR = src
SRC_BUILD_DIR = build/src
TEST_DIR = test
TEST_BUILD_DIR = build/test
CPP_FLAGS = -std=c++11 -O3 -I$(INCLUDE_DIR) -Wall
OBJECTS = $(SRC_BUILD_DIR)/game.o $(SRC_BUILD_DIR)/blue_black_game.o \
          $(SRC_BUILD_DIR)/agent.o $(SRC_BUILD_DIR)/utils.o \
          $(SRC_BUILD_DIR)/conf_parser.o
TARGET = $(SRC_BUILD_DIR)/bbgame
TEST_TARGETS = $(TEST_BUILD_DIR)/test_utils $(TEST_BUILD_DIR)/test_conf_parser

all: $(TARGET) test

$(SRC_BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	g++ -c $(CPP_FLAGS) -o $@ $<

$(TARGET): $(SRC_DIR)/main.cpp $(OBJECTS)
	g++ $(CPP_FLAGS) -o $@ $^

$(TEST_BUILD_DIR)/%: $(TEST_DIR)/%.cpp $(OBJECTS)
	g++ $(CPP_FLAGS) -o $@ $^

test: $(TEST_TARGETS)
	./run_test.sh

clean:
	rm -f build/src/* build/test/*

