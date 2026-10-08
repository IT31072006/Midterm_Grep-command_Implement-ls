CC = gcc
CFLAGS = -Wall -D_GNU_SOURCE -g

TARGET = myls
OBJS = main.o ls.o print.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c ls.h
	$(CC) $(CFLAGS) -c main.c

ls.o: ls.c ls.h print.h
	$(CC) $(CFLAGS) -c ls.c

print.o: print.c print.h ls.h
	$(CC) $(CFLAGS) -c print.c

clean:
	rm -f $(OBJS) $(TARGET)