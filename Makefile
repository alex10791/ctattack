
#g++ -g -Wall -o bin/main src/main.cpp

# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall
CLIBDIR = -L /usr/local/Cellar/cryptopp/5.6.3_1/lib/ 
CHEADER_DIR = -I src/include/
CLIBS   = -lcryptopp

# the build target executable:
TARGET = main

all: $(TARGET)

$(TARGET): src/$(TARGET).c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/$(TARGET) src/$(TARGET).c src/ctattack.c -O0 $(CHEADER_DIR)

#bin/main: src/$(TARGET).cpp 	# this is not working, how to make bin/main depend on src/$(TARGET)

debug: src/$(TARGET).c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/$(TARGET) $(TARGET).c src/ctattack.c -O0 -g $(CHEADER_DIR)

masm_intel: src/$(TARGET).c src/ctattack.c
	$(CC) $(CFLAGS) -o bin/$(TARGET) $(TARGET).c src/ctattack.c -O0 -masm=intel $(CHEADER_DIR)

run: bin/$(TARGET)
	./bin/$(TARGET) $(ARGS)

clean:
	$(RM) $(TARGET)

