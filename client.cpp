#include <iostream>
#include "client.class.h"

using namespace std;

int main(int argc, char* argv[])
{
	Client client;
	stringstream cmd;
	string responce;

	if(argc != 2)
	{
		cerr << "Musí být zadán právě jeden parametr" << endl;
		return -1;
	}

	try
	{
		client.parseInput(argv[1]);
		client.establishConnection();

		cmd << client.connection.file << endl;
		client << cmd;
		client.getFile();

	}
	catch(ftpException & e)
	{
		cerr << e.getMessage() << endl;
		return e.getCode();
	}

	return 0;
}


