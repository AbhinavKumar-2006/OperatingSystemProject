CC = gcc
CFLAGS = -Wall -I. -lm
SRC = $(wildcard *.c)
TARGET = scheduler_sim

all: $(TARGET)

$(TARGET): $(SRC)
		$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
		rm -f $(TARGET) $(TARGET).exe
