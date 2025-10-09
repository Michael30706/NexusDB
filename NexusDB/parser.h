#pragma once

#include <string>
#include <vector>
#include "database.h"

class Parser
{
private:
	Database* _db;
public:
	Parser(Database* db);
	std::string handleCommand(const std::string& command);
};