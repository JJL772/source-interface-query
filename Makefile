
SRC=$(wildcard src/*.c)

LIBS= -ldl -lc 
FLAGS=-m32 -D_UNIX -DUNIX -DLINUX -D_LINUX
FLAGS64=-m64 -D_UNIX -DLINUX -D_LINUX -DUNIX
INSTALL_PREFIX ?= /usr/local/

all: dirs 
	$(CC) -o bin/valve-interface-query $(SRC) $(FLAGS) $(LIBS)
	$(CC) -o bin/valve-interface-query64 $(SRC) $(FLAGS64) $(LIBS)

clean:
	rm -rf bin

dirs:
	mkdir -p bin 

ifeq ($(INSTALL_PREFIX),)
	$(error You must specify an install prefix!)
endif 

install: all 
	/bin/cp src/wrapper.sh $(INSTALL_PREFIX)/bin/source-interface-query
	/bin/cp src/wrapper64.sh $(INSTALL_PREFIX)/bin/source-interface-query64
	chmod +rwx $(INSTALL_PREFIX)/bin/source-interface-query
	chmod +rwx $(INSTALL_PREFIX)/bin/source-interface-query64
	/bin/cp bin/valve-interface-query $(INSTALL_PREFIX)/bin/valve-interface-query
	/bin/cp bin/valve-interface-query64 $(INSTALL_PREFIX)/bin/valve-interface-query64 
