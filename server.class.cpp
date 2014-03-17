#include "server.class.h"

const int Server::bufferSize = 1024;
bool Server::work = true;
int Server::sck;
int Server::speed;

void stop(int sig)
{
	Server::work = false;
	shutdown(Server::sck, 2);
	close(Server::sck);
	std::cerr << "Čekám na ukončení všech aktivních spojení" << std::endl;
}

void * handleClient(void * con)
{
	int x = *(int*)con;
	int res;
	int toSend = Server::bufferSize;
	int sended = 0;
	int size = 0;
	time_t start, sleepTime, timeDiff;
	char buffer[Server::bufferSize] = {0};
	std::string filename;
	std::stringstream msg;

	//načtení jména souboru
	do
	{
		memset(buffer, 0, Server::bufferSize);
		res = recv(x, buffer, Server::bufferSize, MSG_NOSIGNAL);
		if(res < 0) return NULL;

		filename += buffer;
	} while(res == Server::bufferSize);

	std::ifstream file(filename.c_str(), std::fstream::in | std::fstream::binary);

	if(!file.good()) msg << "KO";
	else
	{
		file.seekg(0, std::ifstream::end);
		size = file.tellg();
		msg << size << "\n";
		file.seekg(0, std::ifstream::beg);
	}

	res = send(x, msg.str().c_str(), msg.str().length(), MSG_NOSIGNAL);
	if(res < 0) return NULL;

	start = std::clock();

	while(!file.eof() && file.good())
	{
		memset(buffer, 0, Server::bufferSize);
		file.read(buffer, Server::bufferSize);

		if(file.eof()) toSend = size - sended;

		sended += Server::bufferSize;
		start = std::clock();
		res = send(x, buffer, toSend, MSG_NOSIGNAL);
		timeDiff = std::clock() - start;

		if(res < 0) return NULL;

		if(timeDiff < Server::clocktime())
		{
			sleepTime = ((Server::clocktime() - timeDiff)*1000000)/CLOCKS_PER_SEC;
			usleep(sleepTime);
		}
	}

	file.close();
	close(x);
}

Server::Server()
{
	#ifdef CTRL_C
	signal(SIGINT, stop);
	#endif
}

Server::~Server()
{
	pthread_t * thread_id;

	while(!threads.empty())
	{
		thread_id = threads.top();
		pthread_join(*thread_id, NULL);
		delete thread_id;
		threads.pop();
	}

}

void Server::parseInput(char ** params)
{
	if(strcmp(params[1], "-d") == 0 && strcmp(params[3], "-p") == 0)
	{
		getNumber(params[2], &(Server::speed));
		getNumber(params[4], &port);
	}
	else throw ftpException(ftpException::REGEX_MATCH);
}

void Server::getNumber(char * string, int * location)
{
	char * endptr;

	*location = strtol(string, &endptr, 10);

	if(*endptr != '\0') throw ftpException(ftpException::REGEX_MATCH);
}

void Server::listen()
{
	int res;
	pthread_t * thread_id;
	Server::sck = socket(PF_INET, SOCK_STREAM, 0);
	if(sck < 0) throw ftpException(ftpException::SOCKET);

	address.sin_family = PF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr  = INADDR_ANY;
	unsigned int  len = sizeof(address);

	res = bind(Server::sck, (struct sockaddr *) &address, len);
	if(res < 0) throw ftpException(ftpException::BIND);

	res = ::listen(Server::sck, SOMAXCONN);
	if(res < 0) throw ftpException(ftpException::LISTEN);

	while(Server::work)
	{
		res = accept(Server::sck, (struct sockaddr *) &address, &len);
		if(Server::work == false) break;
		if(res < 0) throw ftpException(ftpException::ACCEPT);

		thread_id = thread();
		pthread_create(thread_id, NULL, handleClient, &res);
	}
}

pthread_t * Server::thread()
{
	pthread_t * thread_id = new pthread_t;
	threads.push(thread_id);

	return thread_id;
}




