CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra -pedantic-errors -O0 -g
LIBS = -lm
OBJS = main.o parser.o stack.o convertions.o arithmetics.o manipulations.o logics.o variables.o inputoutput.o arrays.o blocks.o
TARGET = main

$(TARGET): $(OBJS)
	make format
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

run: $(TARGET)
	./main

clean:
	@rm -f $(TARGET) $(OBJS)

format:
	clang-format -i *.c *.h
	
spec: $(TARGET)
	./main < input