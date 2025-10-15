#include "storage.h"
#include <filesystem>
#include <fstream>
# define MAX_BUFFER 500
# define DB_CONST_LEN 15
# define MAX_STRING_LEN 60
# define INT_LEN 4

std::unordered_map<std::string, int> Storage::typeSizes = { {"int", INT_LEN}, {"string", MAX_STRING_LEN}};

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

std::vector<std::vector<Cell>> Storage::getRows(std::fstream& file, std::vector<Column>& columns)
{
    std::vector<std::vector<Cell>> rows = std::vector<std::vector<Cell>>();
    /*size_t rowLen = 0;
    for (auto& col : columns)
    {
        rowLen += Storage::typeSizes[col.type];
    }*/

    file.seekg(0);

    int beginLoc =  DB_CONST_LEN * (columns.size() * 2 + 1); // The name of the table included
    int currLoc = beginLoc;
    int colIndex = 0;

    std::string value;
    Cell* cellPtr = nullptr;
    std::vector<Cell> row = std::vector<Cell>();
    while (true)
    {
        value = read(file, currLoc, typeSizes[columns[colIndex].type]);

        if (!colIndex)
        {
            if (!row.empty()) rows.push_back(row);
            row = std::vector<Cell>();
        }

        if (value.empty()) break;


        Cell cell;
        cell.column = columns[colIndex].name;
        if (columns[colIndex].type == "string")
            cell.value = value;
        else if (columns[colIndex].type == "int")
        {
            int integer;
            std::memcpy(&integer, value.c_str(), sizeof(integer));
            cell.value = std::to_string(integer);
        }
        row.push_back(cell);


        currLoc += typeSizes[columns[colIndex].type];

        colIndex = (colIndex + 1) % columns.size();
    }

    file.seekg(0);

    return rows;
}



std::string Storage::read(std::fstream& file, int loc, int bytes)
{
    file.seekg(loc);
    char str[MAX_BUFFER] = { 0 };
    file.read(str, bytes);
    return std::string(str);
}

void Storage::InsertRow(const Table& table, const std::vector<Cell>& row)
{
    std::fstream file(table.name + ".bin", std::ios::in | std::ios::out | std::ios::binary);
    file.seekp(0, std::ios::end);
    if (!file)
        return; // handle file error properly

    // Lambda to write strings with zero-padding
    auto writeWithPadding = [&file](const std::string& str, size_t size) {
        file.write(str.c_str(), std::min(str.size(), size)); // write actual content

        char zero = 0;
        for (size_t i = str.size(); i < size; ++i)
            file.write(&zero, 1); // pad remaining bytes
        };

    for (size_t i = 0; i < table.columns.size(); i++)
    {
        std::string currType = table.columns[i].type;

        if (currType == "string")
        {
            writeWithPadding(row[i].value, typeSizes[currType]);
        }
        else if (currType == "int")
        {
            // Option A: store as raw binary (fast, but watch endianness)
            int32_t value = std::stoi(row[i].value);
            file.write(reinterpret_cast<char*>(&value), sizeof(int));

            // Option B: store as padded string (safer)
            // writeWithPadding(row[i].value, typeSizes[currType]);
        }
    }
}

void Storage::saveTable(const std::string& path, const Table& table, const std::string& folder)
{
    std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);

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
    Table table;
    table.name = tableName;

    std::fstream fileReader(tableName.find(".bin") == std::string::npos? tableName + ".bin" : tableName, std::ios::in | std::ios::out | std::ios::binary);
    table.columns = Storage::getColumns(fileReader);

    table.rows = Storage::getRows(fileReader, table.columns);

	return table;
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
