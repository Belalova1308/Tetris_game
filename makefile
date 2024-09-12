CC = gcc
CFLAGS = -lncurses -Wall -Wextra -Werror -std=c11
TETRISDIR = brick_game/tetris/
DRAWDIR = gui/cli/
TARGET = game
OBJ = objects
RM = rm -rf
MK = mkdir -p
MV = mv
AR = ar rcs
OBJSDIR = OBJS/
TESTDIR=tests/
SRCS = *.c
OBJS = *.o
RAN = ranlib
RBIN = gcov_test gcov_test-test.gcda gcov_test-test.gcno gcov_test.info report
GCOV_FLAGS=-fprofile-arcs -ftest-coverage
G = *.gcno
GA = *.gcda

OS := $(shell uname -s)
ifeq ($(OS), Darwin)
	TESTFLAGS = -lcheck -lm -lpthread
else
	TESTFLAGS = -lcheck -lsubunit -pthread -lrt -lm
endif

.PHONY: all clean libgame.c test gcov_report

all: libgame.a $(OBJS) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(DRAWDIR)main.c -o $(TARGET) $(OBJSDIR)$(OBJS) $(CFLAGS)

libgame.a: $(OBJS)
	$(AR) libgame.a $(OBJSDIR)$(OBJS)

$(OBJS): $(DRAWDIR)$(SRCS) $(TETRISDIR)$(SRCS)
	$(MK) $(OBJSDIR)
	$(CC) -c $(DRAWDIR)screen.c $(TETRISDIR)$(SRCS)
	$(MV) $(OBJS) $(OBJSDIR)

test: $(TESTDIR)tests_main.c libgame.a
	$(CC)  $(TESTDIR)tests_main.c $(TESTDIR)tests.c libgame.a -o test $(TESTFLAGS) -lncurses
	./test

gcov_report: $(TESTDIR)tests_main.c
	$(CC) $(GCOV_FLAGS) $(TESTDIR)*.c $(TETRISDIR)$(SRCS) libgame.a $(TESTFLAGS) -lncurses -o gcov_test
	chmod +x *
	./gcov_test
	lcov -t "gcov_test" -o gcov_test.info -c -d .
	genhtml -o report gcov_test.info
	open ./report/index.html
	open report/gcov_report.html

clean:
	$(RM) $(OBJSDIR) $(TARGET)
	$(RM) $(OBJS) *.a $(RBIN) test
	$(RM) $(GA)
	$(RM) $(G)
	$(RM) dist/
	$(RM) build/

style :
	clang-format -n $(TETRISDIR)*.h $(TETRISDIR)$(SRCS) $(DRAWDIR)$(SRCS) $(DRAWDIR)*.h

install: all
	mkdir -p build
	cp $(TARGET) build/$(TARGET)

uninstall:
	rm -rf build

dist:
	mkdir -p dist
	tar -cvzf dist/game_zip.tar.gz $(OBJSDIR) libgame.a game

dvi:
ifeq ($(OS), Darwin)
	open ./doc/documentation.md
else
	cat ./doc/documentation.md
endif