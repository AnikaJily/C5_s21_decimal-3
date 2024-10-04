DATE = $(shell date -R)
CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11 -g
P = -pedantic
DB = -g -O0
OBJ = obj
SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c, $(OBJDIR)%.o, $(SOURCES))
TEST = s21_test
REPORT_TARGET = realpath src/*.c

OS := $(shell uname)
UBUNTU := $(shell hostnamectl | grep -o 'Ubuntu')
MAC_X86 := $(shell uname -a | grep -o _X86_64)
MAC_M1 := $(shell uname -a | grep -o _ARM64)

ifeq ($(OS), Linux)
	TEST_LIBS = -lcheck -pthread -lrt -lm -lsubunit
	MATH = -lm
	OPEN = xdg-open
else ifeq ($(UBUNTU), Ubuntu)
	TEST_FLAGS=-lpthread -lcheck  -Wno-nonnull -Wno-memset-transposed-args -lm -lsubunit
	OPEN = open
endif

ifeq ($(MAC_M1), _ARM64)
	TEST_LIBS := -I/opt/homebrew/opt/check/include -L/opt/homebrew/opt/check/lib -lcheck
	OPEN = open
endif
ifeq ($(MAC_X86), _X86_64)
	TEST_LIBS = $(shell pkg-config --libs --cflags check) -lm
	OPEN = open
endif

all: s21_decimal.a

build: all

repo:
	mkdir -p $(OBJ)
$(OBJDIR)%.o: %.c
	$(CC) $(FLAGS) -c $< -o obj/$@

s21_decimal.a: repo $(OBJECTS)
	ar rcs s21_decimal.a $(OBJ)/*.o
	ranlib s21_decimal.a
test: reset s21_decimal.a 
	$(CC) $(FLAGS) tests/*.c *.c s21_decimal.a $(TEST_LIBS) -o s21_test
	./$(TEST)
old: reset s21_decimal.a 
	$(CC) $(FLAGS) tests/*.c *.c s21_decimal.a $(TEST_LIBS) -o s21_test
	./$(TEST)
t2: reset s21_decimal.a 
	$(CC) $(FLAGS) tests/*.c *.c s21_decimal.a $(TEST_LIBS) -o s21_test
	./$(TEST)
brew:
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	reset
	brew install lcov
	brew install check
	sleep 1
	reset
gcov_flag:
	$(eval FLAGS += --coverage)
gcov_report: clean s21_decimal.a
	gcc -fprofile-arcs -ftest-coverage s21_comparison.c s21_convertor.c s21_another.c s21_arithmetic.c tests/test.c s21_decimal.a -o report.out -lm -lcheck
	./report.out
	gcovr -r . -f s21_comparison.c -f s21_convertor.c -f s21_another.c -f s21_arithmetic. --html --html-details -o ./report.html
	open report.html

fix:
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -n *.c *.h tests/*.c tests/*.h tests/*.c
	clang-format -i *.c *.h tests/*.c tests/*.h tests/*.c
	sleep 1
	reset
	clang-format -n *.c *.h tests/*.c tests/*.h tests/*.c

add_db_flag:
	$(eval FLAGS += $(DB))
mem: clean add_db_flag test
ifeq ($(OS),Linux)
	valgrind -s --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./$(TEST)
else
	leaks --atExit -- ./$(TEST)
endif

cpp: reset
	cppcheck --enable=all --suppress=missingIncludeSystem ./*.c *.h ./tests/*.c ./tests/*.h

rebuild: clean build
	
clean:
	rm -rf $(TEST) *.o *.a *.gcno *.gcda *.gcov *.info report a.out *.dSYM obj *.html *.css *.out
	reset
	
reset:
	reset | bash

