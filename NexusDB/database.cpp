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

Table Database::getTable(const std::string& name)
{
	return tables.find(name)->second;
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

void Database::insertInto(const std::string& table, std::map<std::string, Cell>& values)
{
	if (tables.find(table) == tables.end()) throw std::exception();
	std::vector<Cell> paddedValues = std::vector<Cell>();
	for (auto& col : tables[table].columns)
	{
		Cell cell;
		cell.column = col.name;
		cell.value = (values.find(col.name) != values.end()? values[col.name].value : "");
		if (col.type != "string") cell.value = "0";
		paddedValues.push_back(cell);
	}
	tables[table].insertRow(paddedValues);
	Storage::InsertRow(tables[table], paddedValues);
}

void Database::selectAll(const std::string& table)
{
}

void Database::select(const std::string& table, std::vector<Column>& requestedColumns, std::vector<Cell> conditions)
{

}

void Database::saveToDisk()
{
}

void Database::loadFromDisk()
{
	std::vector<std::string> dbs = getBinFiles();
	for (auto& dbName : dbs)
	{
		std::string noExtensionName = split(dbName, ".")[0];
		Table table = Storage::loadTable(noExtensionName, "");
		tables[noExtensionName] = table;
	}


}
