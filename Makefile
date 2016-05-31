CFLAGS=-Wall
all: envthis paththis

envthis:
	gcc envthis.c -o envthis
paththis:
	gcc paththis.c -o paththis
