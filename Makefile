CC = gcc
CFLAGS = -Wall -Wextra -O2 -D_CRT_SECURE_NO_WARNINGS
TARGET = csvreader

SRCS = main.c unit.c
OBJS = main.o unit.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

main.o: main.c unit.h
	$(CC) $(CFLAGS) -c main.c

unit.o: unit.c unit.h
	$(CC) $(CFLAGS) -c unit.c

clean:
	del -f $(OBJS) $(TARGET).exe 2>nul || rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET) example.csv

.PHONY: all clean run