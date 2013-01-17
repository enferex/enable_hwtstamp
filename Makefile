APP=enable_hwtstamp
CC=gcc
CFLAGS=-g3 -Wall -pedantic

$(APP): main.c
	$(CC) $^ $(CFLAGS) -o $@

clean:
	rm -f $(APP)
