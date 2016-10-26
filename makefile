CC=mpicc
CFLAGS=-Wall -g --std=c99
TARGET=forestfire

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)

clean:
	rm $(TARGET)
