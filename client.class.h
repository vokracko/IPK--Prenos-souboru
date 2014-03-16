#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <stack>
#include <regex.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include "ftpException.h"

struct Tconnection
{
	std::string host;
	std::string file;
	unsigned int port;
};

class Client
{
		static const int replyLength = 1024;

		int sck;

		enum
		{
			HOST = 1,
			PORT = 2,
			FILE = 3,
		};

	public:
		Tconnection connection;

		Client();
		~Client();

		void establishConnection();
		void parseInput(const char * input);
		void send(std::stringstream & command);
		void operator << (std::stringstream & command);

		bool getFile();


};
