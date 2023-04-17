CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS =

TARGET = main
SRCS = main.c fileio.c errhandler.c script.c vector.c hashtable.c drawer.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET)