#include "server.class.h"

void * handleClient(void * con)
{
	int x = *(int*)con;
	int res;
	int sended = 100;
	int tmpSize = 0;
	int size = 0;
	char buffer[100] = {0};
	std::string filename;
	std::stringstream msg;

	//načtení jména souboru
	do
	{
		memset(buffer, 0, 100);
		recv(x, buffer, 99, 0);
		filename += buffer;
	} while(res == 99);

	std::ifstream file(filename.c_str(), std::fstream::in | std::fstream::binary);

	if(!file.good()) msg << "KO";
	else
	{
		file.seekg(0, std::ifstream::end);
		size = file.tellg();
		msg << size << "\n";
		file.seekg(0, std::ifstream::beg);
	}

	send(x, msg.str().c_str(), msg.str().length(), 0);

	while(!file.eof() && file.good())
	{
		memset(buffer, 0, 100);
		file.read(buffer, 100);
		// std::cout << buffer << std::endl;
		if(file.eof()) sended = size - tmpSize;
		tmpSize += 100;
		send(x, buffer, sended, 0);
	}

	file.close();
}

Server::Server()
{
	threadCount = 0;
}

void Server::parseInput(char ** params)
{
	if(strcmp(params[1], "-d") == 0 && strcmp(params[3], "-p") == 0)
	{
		getNumber(params[2], &speed);
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

	sck = socket(PF_INET, SOCK_STREAM, 0);
	if(sck < 0) throw ftpException(ftpException::SOCKET);

	address.sin_family = PF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr  = INADDR_ANY;
	unsigned int  len = sizeof(address);

	res = bind(sck, (struct sockaddr *) &address, len);
	if(res < 0) throw ftpException(ftpException::BIND);

	res = ::listen(sck, SOMAXCONN);
	if(res < 0) throw ftpException(ftpException::LISTEN);

	while(true)
	{
		res = accept(sck, (struct sockaddr *) &address, &len);
		if(res < 0) throw ftpException(ftpException::ACCEPT);

		pthread_create(&threads[threadCount++], NULL, handleClient, &res);
	}
}



//TODO odchytávat ctrl+d, uzavřít socket
