#include "storage.h"
#include <filesystem>
#include <fstream>
# define MAX_BUFFER 100
# define DB_CONST_LEN 15



std::vector<Column> Storage::getColumns(std::fstream& file)
{
    std::vector<std::string> types = std::vector<std::string>();
    std::vector<Column> cols = std::vector<Column>();
    int i = DB_CONST_LEN;
    std::string str = Storage::read(file, i, i);
    while (Database::types.find(str) != Database::types.end())
    {
        str = Storage::read(file, i, DB_CONST_LEN);
        if (Database::types.find(str) != Database::types.end())
        {
            types.push_back(str);
            i += DB_CONST_LEN;
        }
    }

    for (int count = 0; count < types.size(); count++)
    {
        Column col;
        col.name = str;
        col.type = types[count];
        cols.push_back(col);
        i += DB_CONST_LEN;
        str = Storage::read(file, i, DB_CONST_LEN);
    }

    file.seekg(0);

    return cols;
}


std::string Storage::read(std::fstream& file, int loc, int bytes)
{
    file.seekg(loc);
    char str[MAX_BUFFER] = { 0 };
    file.read(str, bytes);
    return std::string(str);
}

void Storage::saveTable(const std::string& path, const Table& table, const std::string& folder)
{
    std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);

    // Create file if it doesn't exist
    if (!file.is_open()) {
        file.open(path, std::ios::out | std::ios::binary);
        file.close();
        file.open(path, std::ios::in | std::ios::out | std::ios::binary);
    }

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::fstream fileReader(path, std::ios::in | std::ios::out | std::ios::binary);
    std::vector<Column> cols = getColumns(fileReader);
    if (cols.size()) throw std::exception();

    fileReader.close();

    file.seekp(0);

    auto writeWithPadding = [&file](const std::string& str) {
        file.write(str.c_str(), str.size());

        char zero = 0;
        for (size_t i = str.size(); i < DB_CONST_LEN; ++i) {
            file.write(&zero, 1);
        }
    };

    writeWithPadding(table.name);

    for (auto& column : table.columns) {
        writeWithPadding(column.type);
    }

    for (auto& column : table.columns) {
        writeWithPadding(column.name);
    }

    file.close();
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
