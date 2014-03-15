#include <iostream>

class ftpException: public std::exception
{
		int code;

	public:
		static const int REGEX_MATCH = 1;
		static const int REGEX_COMPILATION = 2;
		static const int SOCKET = 3;
		static const int HOSTNAME = 4;
		static const int CONNECT = 5;
		static const int NOT_FOUND = 6;
		static const int RESPONCE = 7;
		static const int ACCEPT = 8;
		static const int BIND = 9;
		static const int LISTEN = 10;
		static const int EXISTS = 11;

		ftpException(int code)
		{
			ftpException::code = code;
		}

		std::string getMessage()
		{
			std::string messages[] =
			{
				"",
				"Chybný formát parametru",
				"Regex se nepodařilo zkompilovat",
				"Nepodařilo se vytvořit socket",
				"Chyba hostname",
				"Chyba připojení",
				"Chybná odpověď serveru",
				"Neočekávaná odpověď serveru",
				"ACCEPT",
				"BIND",
				"LISTEN",
				"Soubor neexistuje",
			};

			return messages[code];
		}

		int getCode()
		{
			return code;
		}
};
