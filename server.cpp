#include "server.class.h"

using namespace std;

int main(int argc, char *argv[])
{
	Server server;

	if(argc != 5)
	{
		cerr << "Zadán špatný počet parametrů" << endl;
		return -1; //chybí parametr
	}

	try
	{
		server.parseInput(argv);
		server.listen();
	}
	catch(ftpException & e)
	{
		cerr << e.getMessage() << endl;
		return e.getCode();
	}

	return 0;
}

//TODO regulace rychlosti - nefunguje přesně pro vyšší hodnoty (je to pomalejší)
