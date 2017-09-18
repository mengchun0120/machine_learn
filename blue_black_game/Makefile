CPP=/usr/bin/g++
INCLUDE_DIR = include
SRC_DIR = src
SRC_BUILD_DIR = build/src
TEST_DIR = test
TEST_BUILD_DIR = build/test
COMMON_FLAGS = -std=c++11 -I$(INCLUDE_DIR) -Wall
DEBUG_FLAGS = -g $(COMMON_FLAGS)
RELEASE_FLAGS = -O3 -DDEBUG $(COMMON_FLAGS)
OBJECTS = $(SRC_BUILD_DIR)/game.o \
          $(SRC_BUILD_DIR)/blue_black_game.o \
          $(SRC_BUILD_DIR)/agent.o \
          $(SRC_BUILD_DIR)/utils.o \
          $(SRC_BUILD_DIR)/param_config.o \
          $(SRC_BUILD_DIR)/point.o \
          $(SRC_BUILD_DIR)/assign_func.o \
          $(SRC_BUILD_DIR)/conf_parser.o \
          $(SRC_BUILD_DIR)/record_game_func.o
TARGET = $(SRC_BUILD_DIR)/bbgame
TEST_TARGETS = $(TEST_BUILD_DIR)/test_utils \
               $(TEST_BUILD_DIR)/test_conf_parser

default: debug

debug: CPP_FLAGS = $(DEBUG_FLAGS)
debug: $(TARGET) test

release: CPP_FLAGS = $(RELEASE_FLAGS)
release: $(TARGET) test

$(SRC_BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	$(CPP) -c $(CPP_FLAGS) -o $@ $<

$(TARGET): $(SRC_DIR)/main.cpp $(OBJECTS)
	$(CPP) $(CPP_FLAGS) -o $@ $^

$(TEST_BUILD_DIR)/%: $(TEST_DIR)/%.cpp $(OBJECTS)
	$(CPP) $(CPP_FLAGS) -o $@ $^

test: $(TEST_TARGETS)
	sh ./run_test.sh

clean:
	rm -f build/src/* build/test/*

