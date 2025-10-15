#include <iostream>
#include <string>
#include "parser.h"


int main()
{

	Database db = Database();
	db.loadFromDisk();

	Parser parser = Parser(&db);


	std::cout << "Welcome to Nexus database engine!" << std::endl;

	while (true)
	{
		std::string command;
		std::cout << ">> ";
		std::getline(std::cin, command);
		std::cout << parser.handleCommand(command) << std::endl;
	}
	return 0;
}