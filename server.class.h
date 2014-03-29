#include <string>
#include <cstring>
#include <csignal>
#include <sys/time.h>
#include <sstream>
#include <fstream>
#include <pthread.h>
#include <regex.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stack>
#include <errno.h>
#include <unistd.h>
#include "ftpException.h"

#define CTRL_C 1

class Server
{
	int port;
	sockaddr_in address;
	std::stack<pthread_t*> threads;

	public:
		static const int bufferSize;
		static bool work;
		static int sck;
		static int speed;

		static int duration()
		{
			return (1000000/speed) - 50;
		}

		Server();
		~Server();
		void parseInput(char ** params);
		void listen();

	private:
		void getNumber(char * string, int * location);
		pthread_t * thread();

};
