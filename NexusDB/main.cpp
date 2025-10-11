#include <iostream>
#include <string>
#include "parser.h"

int main()
{
	std::cout << "Welcome to Nexus database engine!" << std::endl;

	Database db = Database();
	Parser parser = Parser(&db);

	while (true)
	{
		std::string command;
		std::cout << ">> ";
		std::getline(std::cin, command);
		std::cout << parser.handleCommand(command) << std::endl;
	}
	return 0;
}