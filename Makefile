CC = gcc
CFLAGS = -std=c99 -g -Wall
OBJS = crawler.o indexPage.o webQueries.o

all: webSearch

webSearch: webSearch.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $< -o $@ -lm

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o webSearch