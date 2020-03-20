CC = g++
SRCDIR = src
INCDIR = inc
CFLAGS = -g -Wall -Iinc -std=c++17 -O2 -I/usr/local/include/bsoncxx/v_noabi
CPPFLAGS = $(CFLAGS)
LDFLAGS = -lCAENDigitizer -lsqlite3 -lpthread -lbsoncxx
INSTALL = /usr/local/bin/obelix
TEST = test_exe

sources := $(wildcard src/*.cpp)
objects := $(sources:.cpp=.o)
VPATH = src:inc

exe : $(objects)
	$(CC) $(CPPFLAGS) -o $(TEST) $(objects) $(LDFLAGS)

install :
	$(CC) $(CPPFLAGS) -o $(INSTALL) $(objects) $(LDFLAGS)

$(L)%.o : %.cpp %.h %.d
	$(CC) $(CPPFLAGS) -c $< -o $@

$(L)%.d : %.cpp %.h
	$(CC) -MM $(CPPFLAGS) $< -o $@

.PHONY: clean

clean:
	-rm -f $(objects) $(TEST)
