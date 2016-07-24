CFLAGS=-Wall -g
CFLAGS += $(shell pkg-config --cflags json-c)
LDFLAGS += $(shell pkg-config --libs json-c)
LDFLAGS += $(shell pkg-config --libs --cflags libcurl)
all: envthis paththis shortenthis

envthis:
	gcc envthis.c -o envthis $(CFLAGS)
paththis:
	gcc  paththis.c -o paththis $(CFLAGS)
shortenthis:
	gcc  shortenthis.c -o shortenthis $(LDFLAGS) $(CFLAGS)
clean:
	rm paththis envthis shortenthis
install:
	sudo mv paththis /usr/bin/
	sudo mv shortenthis /usr/bin/
	sudo mv envthis /usr/bin/
