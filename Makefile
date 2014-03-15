flags=--std=c++98

all: client server

client: clean
	g++ $(flags) client.cpp client.class.cpp -o client

server: clean
	g++ $(flags) server.cpp server.class.cpp -o server -lpthread

clean:
	@rm -f *.tar *.gz client server 2> /dev/null

pack: clean
	tar -cf xvokra00.tar *.cpp *.h Makefile
	gzip xvokra00.tar

