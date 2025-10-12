#include "database.h"
#include "storage.h"
#include <fstream>

std::unordered_set<std::string> Database::types = { "int", "string" };


bool Database::checkTypes(const std::vector<Column>& columns)
{
	for (Column col : columns)
	{
		if (Database::types.find(col.type) == Database::types.end() ||
			Database::types.find(col.name) != Database::types.end()) 
			return false;
	}
	return true;
}

Database::Database()
{
}

void Database::createTable(const std::string& name, const std::vector<Column>& columns)
{
	Table table = Table();
	table.name = name;
	table.columns = columns;
	tables[name] = table;
	std::string fullName = table.name;
	fullName += ".bin";
	if (!checkTypes(columns)) throw std::exception();
	tables[table.name] = table;
	Storage::saveTable(fullName, table);
}

void Database::insertInto(const std::string& table, const std::vector<std::string>& values)
{
}

void Database::selectAll(const std::string& table)
{
}

void Database::saveToDisk()
{
}

void Database::loadFromDisk()
{
}
