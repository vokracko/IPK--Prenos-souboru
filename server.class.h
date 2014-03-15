#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <pthread.h>
#include <regex.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "ftpException.h"

class Server
{
	int speed;
	int port;
	int threadCount;
	int sck;
	sockaddr_in address;
	pthread_t threads[SOMAXCONN];

	public:
		Server();
		void parseInput(char ** params);
		void listen();

	private:
		void getNumber(char * string, int * location);
};
