C = gcc -std=c11
CPP = g++ -std=c++17
C_FLAGS = -Wall -Werror -Wextra
T_FLAGS = -lgtest -lgmock -lpthread
GCOV_FLAGS = -fprofile-arcs -ftest-coverage -lgcov

BUILD_PATH = ../build

TETRIS_TARGET = $(BUILD_PATH)/tetris
TETRIS_SRC = brick_game/tetris/*.c
TETRIS_LIB = tetris.a
TETRIS_CLI = gui/cli/tetris_interface.c

SNAKE_TARGET = $(BUILD_PATH)/snake_desktop
SNAKE_OBJ = $(BUILD_PATH)/brick_game/snake/snake.o
SNAKE_SRC = brick_game/snake/snake.cpp
CONTROLLER_OBJ = $(BUILD_PATH)/gui/cli/snake/controller.o
CONTROLLER_SRC = gui/cli/snake/controller.cpp
VIEW_OBJ = $(BUILD_PATH)/gui/cli/snake/view.o
VIEW_SRC = gui/cli/snake/view.cpp
SNAKE_CLI_OBJ = $(BUILD_PATH)/gui/cli/snake/interface.o
SNAKE_CLI_SRC = gui/cli/snake/interface.cpp
SNAKE_LIB = $(BUILD_PATH)/snake.a
SNAKE_CLI = $(BUILD_PATH)/snake_cli

TESTS_SRC = tests/*.cpp

HEADERS = brick_game/library_specification.h brick_game/tetris/*.h brick_game/snake/*.h gui/cli/snake/*.h gui/desktop/tetris/*.h gui/desktop/snake/*.h tests/*.h

SRC = $(TETRIS_SRC) $(TETRIS_CLI) gui/desktop/tetris/*.cpp brick_game/snake/*.cpp 
DESK = gui/desktop/tetris/*.cpp gui/desktop/snake/*.cpp

PROJECT = CPP3_BrickGame
VERSION = 2.0
PRJ_DIR = $(PROJECT)_v$(VERSION)
TAR = $(PRJ_DIR).tar.gz
PRJ_DIST = $(HEADERS) $(SRC) $(DESK) $(TESTS_SRC) Makefile

all: clean install

install: tetris_cli snake_cli tetris_desktop snake_desktop

# Сборка библиотеки Tetris
$(TETRIS_LIB):
	mkdir -p $(BUILD_PATH)
	$(C) $(C_FLAGS) -c $(TETRIS_SRC) -o $(TETRIS_TARGET)
	ar rcs $(BUILD_PATH)/$(TETRIS_LIB) $(TETRIS_TARGET)
	rm -rf $(BUILD_PATH)/tetris

# Компиляция Snake
$(SNAKE_OBJ): $(SNAKE_SRC)
	mkdir -p $(dir $@)
	$(CPP) $(C_FLAGS) -c $< -o $@

# Сборка библиотеки Snake
$(SNAKE_LIB): $(SNAKE_OBJ)
	mkdir -p $(dir $@)
	ar rcs $@ $<

# Компиляция Controller
$(CONTROLLER_OBJ): $(CONTROLLER_SRC)
	mkdir -p $(dir $@)
	$(CPP) $(C_FLAGS) -c $< -o $@

# Компиляция View
$(VIEW_OBJ): $(VIEW_SRC)
	mkdir -p $(dir $@)
	$(CPP) $(C_FLAGS) -c $< -o $@

# Компиляция CLI для Snake
$(SNAKE_CLI_OBJ): $(SNAKE_CLI_SRC)
	mkdir -p $(dir $@)
	$(CPP) $(C_FLAGS) -c $< -o $@

# Сборка CLI для Snake
$(SNAKE_CLI): $(SNAKE_CLI_OBJ) $(CONTROLLER_OBJ) $(VIEW_OBJ) $(SNAKE_LIB)
	$(CPP) $(C_FLAGS) -o $@ $(SNAKE_CLI_OBJ) $(CONTROLLER_OBJ) $(VIEW_OBJ) -lncurses -L. $(BUILD_PATH)/$(SNAKE_LIB)
	rm -rf $(BUILD_PATH)/brick_game
	rm -rf $(BUILD_PATH)/gui

tetris_cli: $(TETRIS_LIB)
	$(C) $(C_FLAGS) $(TETRIS_CLI) -o $(BUILD_PATH)/tetris.o -lncurses -L. $(BUILD_PATH)/$(TETRIS_LIB)

snake_cli: $(SNAKE_CLI)

tetris_desktop:
	rm -rf temp
	mkdir temp && cd temp && qmake ../gui/desktop/tetris
	cd temp && make
	mv temp/tetris_desktop.app/Contents/MacOS/tetris_desktop $(TETRIS_TARGET)
	rm -rf temp

snake_desktop:
	rm -rf temp
	mkdir temp && cd temp && qmake ../gui/desktop/snake
	cd temp && make
	mv temp/snake_desktop.app/Contents/MacOS/snake_desktop $(SNAKE_TARGET)
	rm -rf temp

uninstall:
	rm -rf $(BUILD_PATH)

dvi:
	rm -rf $ doxygen
	doxygen
	open doxygen/html/index.html

dist: $(PRJ_DIST)
	rm -rf $(PRJ_DIR)
	mkdir $(PRJ_DIR)
	cp $^ $(PRJ_DIR)
	tar -cvzf $(TAR) $(PRJ_DIR)
	rm -rf $(PRJ_DIR)

test: clean $(SNAKE_LIB)
	$(CPP) $(C_FLAGS) $(TESTS_SRC) $(SNAKE_SRC) $(SNAKE_LIB) $(T_FLAGS) -o test
	./test

gcov_report: clean $(SNAKE_LIB)
	$(CPP) $(C_FLAGS) $(TESTS_SRC) $(SNAKE_SRC) $(SNAKE_LIB) $(T_FLAGS) $(GCOV_FLAGS) -o snake
	./snake
	lcov -t "snake" -o s21_report.info -q --no-external -c -d . --ignore-errors usage,inconsistent
	genhtml -o report s21_report.info
	open ./report/index.html
	rm -rf *.gcno *.gcda *.info snake

clang:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i $(HEADERS) $(SRC) $(CLI) $(DESK) $(TESTS_SRC)
	rm -rf .clang-format

clang_review:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n $(HEADERS) $(SRC) $(CLI) $(DESK) $(TESTS_SRC)
	rm -rf .clang-format

leaks: test
	leaks -atExit -- ./test

clean:
	rm -rf *.a *.o $(BUILD_PATH) tetris_high_score.bin snake_high_score.bin doxygen $(TAR) test report