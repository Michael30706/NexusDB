#include "storage.h"

void Storage::saveTable(const Table& table, const std::string& folder)
{
	
}

Table Storage::loadTable(const std::string& tableName, const std::string& folder)
{
	return Table();
}

bool Storage::tableExists(const std::string& tableName, const std::string& folder)
{
	return false;
}

std::string Storage::serializeRow(const std::vector<std::string>& row)
{
	return std::string();
}

std::vector<std::string> Storage::deserializeRow(const std::string& line)
{
	return std::vector<std::string>();
}
