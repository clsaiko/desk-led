CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g

make: desk-led.c 
	$(CC) $(CFLAGS) -o desk-led desk-led.c dlserial.c

debug: desk-led.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o desk-led desk-led.c dlserial.c

clean: 
	rm desk-led