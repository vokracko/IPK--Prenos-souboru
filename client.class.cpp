#include "client.class.h"

Client::Client()
{
}

Client::~Client()
{
	close(sck);
}


void Client::parseInput(const char * input)
{
	bool compRes;
	bool match;
	regex_t regex;
	regmatch_t parts[FILE+1];

	compRes = regcomp(&regex, "^([a-zA-Z0-9.-]+):([0-9]+)/([a-zA-Z0-9_.-]+)$", REG_EXTENDED);

	if(compRes != 0)
	{
		throw ftpException(ftpException::REGEX_COMPILATION);
	}

	match = regexec(&regex, input, FILE+1, parts, 0);

	if(match != 0)
	{
		regfree(&regex);
		throw ftpException(ftpException::REGEX_MATCH);
	}

	connection.host.assign(input, parts[HOST].rm_so, parts[HOST].rm_eo - parts[HOST].rm_so);
	connection.port = atoi(input + parts[PORT].rm_so);
	connection.file.assign(input, parts[FILE].rm_so, parts[FILE].rm_eo - parts[FILE].rm_so);

	regfree(&regex);
}

void Client::establishConnection()
{
	int res;
	sockaddr_in address;
	hostent *host = gethostbyname(connection.host.c_str());
	sck = socket(PF_INET, SOCK_STREAM, 0);


	if(sck < 0) throw ftpException(ftpException::SOCKET);
	if(host == NULL) throw ftpException(ftpException::HOSTNAME);

	address.sin_family = PF_INET;
	address.sin_port = htons(connection.port);
	std::memcpy(&address.sin_addr, host->h_addr, host->h_length);

	res = connect(sck, (struct sockaddr *) &address, sizeof(address));

	if(res < 0) throw ftpException(ftpException::CONNECT);
}

bool Client::getFile()
{
	static char buffer[replyLength+1] = {0};

	std::ofstream file;
	long long size = 0, readSize = 0;
	int res;
	char * pos;

	//jako první se posílá velikost souboru
	//ale můžou tam být už data nebo chybová zpráva
	memset(buffer, 0, replyLength);
	res = recv(sck, buffer, replyLength, MSG_NOSIGNAL);
	if(res < 0)
	{
		throw ftpException(ftpException::RECV);
	}

	if(isdigit(buffer[0]))
	{
		size = atol(buffer);
		pos = strstr(buffer, "\n");

		if(pos && res == replyLength)
		{
			readSize = res - (int) (pos - buffer) - 1; //-1 čtu až za novým řádkem
			file.write(pos + 1, readSize); // +1 až za nový řádkem
		}
	}
	else
	{
		throw ftpException(ftpException::EXISTS);
	}

	file.open(connection.file.c_str(), std::fstream::out | std::fstream::binary);

	do
	{
		memset(buffer, 0, replyLength);
		res = recv(sck, buffer, replyLength, MSG_NOSIGNAL);
		if(res < 0)
		{
			file.close();
			throw ftpException(ftpException::RECV);
		}

		readSize += res;
		file.write(buffer, res);
	}
	while(readSize < size);

	file.close();

}

void Client::operator << (std::stringstream & command)
{
	Client::send(command);
}

void Client::send(std::stringstream & command)
{
	::send(sck, command.str().c_str(), command.str().size() - 1, MSG_NOSIGNAL); // -1 konec řádku
	command.str("");
}
