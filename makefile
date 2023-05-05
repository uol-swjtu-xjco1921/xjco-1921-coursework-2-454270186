CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -lplplot
LDFLAGS = 

TARGET = main
SRCS = main.c fileio.c errhandler.c script.c vector.c hashtable.c drawer.c log.c pathfinder.c heap.c
OBJS = $(SRCS:.c=.o)
TEMPFILE = map.png shortest.png

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -lplplot -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET) $(TEMPFILE)