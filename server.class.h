#include <string>
#include <cstring>
#include <csignal>
#include <ctime>
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

		static time_t clocktime()
		{
			static const time_t kbsp = (1.0/speed)*CLOCKS_PER_SEC;
			return kbsp;
		}

		Server();
		~Server();
		void parseInput(char ** params);
		void listen();

	private:
		void getNumber(char * string, int * location);
		pthread_t * thread();

};
