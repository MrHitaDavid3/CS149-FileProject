CC      = gcc
CFLAGS  = -Wall -pthread -I include
LDFLAGS = -lreadline
SRC     = src/main.c src/createThread.c src/closeThread.c \
          src/openThread.c src/searchThread.c
TARGET  = fileproject

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
