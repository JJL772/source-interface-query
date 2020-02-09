
SRC=$(wildcard src/*.c)

LIBS= -ldl -lc -m32 -g

all: dirs 
	$(CC) -o bin/valve-interface-query $(SRC) $(LIBS)

clean:
	rm -rf bin

dirs:
	mkdir -p bin 

install: all 
	/bin/cp bin/valve-interface-query /usr/local/bin/valve-interface-query
