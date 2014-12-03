CC = gcc
CFLAGS =
LIBS = -lpthread
DEPS = queue.h
OBJ = main.o queue.o
TARGET = queue

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -vf *.o $(TARGET)