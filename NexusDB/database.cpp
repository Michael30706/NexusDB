#include "database.h"
#include "storage.h"
#include <fstream>
#include <algorithm>


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
		if (col.type != "string" && cell.value == "") cell.value = "0";
		paddedValues.push_back(cell);
	}
	tables[table].insertRow(paddedValues);
	Storage::InsertRow(tables[table], paddedValues);
}

void Database::selectAll(const std::string& table)
{
}

void Database::select(const std::string& table, std::vector<std::string>& requestedColumns, std::vector<Condition>& conditions)
{
	Table tableObject = tables[table];
	std::vector<Column> orderedColumns = std::vector<Column>();

	std::unordered_map<std::string, Column> columnsMap;
	for (auto& colObj : tableObject.columns)
		columnsMap[colObj.name] = colObj;

	for (auto& colName : requestedColumns) {
		auto it = columnsMap.find(colName);
		if (it != columnsMap.end())
			orderedColumns.push_back(it->second);
		else
			throw std::exception();
	}

	std::vector<std::vector<Cell>> rowsSelected = std::vector<std::vector<Cell>>();
	std::vector<std::vector<Cell>> prev = std::vector<std::vector<Cell>>();
	bool andLogicDone = false;

	for (auto& condition : conditions)
	{
		Cell dummyCell;

		dummyCell.column = condition.column;

		dummyCell.value = condition.value;

		if (condition.logicGate == 'o' || prev.empty() && !andLogicDone)
		{
			for (auto& row : prev)
			{
				rowsSelected.push_back(row);
			}
			prev = std::vector<std::vector<Cell>>(); 

			for (auto& row : tableObject.rows) 
			{
				auto cellFound = std::find(row.begin(), row.end(), dummyCell); 
				if (cellFound != row.end())
				{
					if (condition.logicGate == 'o')
						rowsSelected.push_back(row);
					else if (!andLogicDone) prev.push_back(row);
				}
			}
			if (condition.logicGate == 'a') andLogicDone = prev.empty();
			else andLogicDone = false;
		}
		else if (!andLogicDone)
		{
			int index = 0;
			for (auto& row : prev)
			{
				auto cellFound = std::find(row.begin(), row.end(), dummyCell);
				if (cellFound == row.end()) prev.erase(prev.begin() + index);
				index++;
			}
			andLogicDone = prev.empty();
		}
	}

	if (!conditions.empty() && conditions[conditions.size() - 1].logicGate == 'a')
	{
		for (auto& row : prev)
		{
			rowsSelected.push_back(row);
		}
	}
	else if (conditions.empty()) rowsSelected = tableObject.rows;
	

	for (auto& col : orderedColumns) {
		std::cout << col.name << "\t";
	}
	std::cout << "\n";

	// Print separator
	for (size_t i = 0; i < orderedColumns.size(); i++) std::cout << "--------\t";
	std::cout << "\n";

	// Print each selected row
	for (auto& row : rowsSelected) {
		for (auto& col : orderedColumns) {
			// find the cell in the row that matches this column
			auto it = std::find_if(row.begin(), row.end(),
				[&col](const Cell& c) { return c.column == col.name; });

			if (it != row.end())
				std::cout << it->value << "\t";  // print cell value
			else
				std::cout << "NULL\t";           // column missing
		}
		std::cout << "\n";
	}
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
