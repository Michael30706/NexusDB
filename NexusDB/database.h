#pragma once
#include <string>
#include <map>
#include "table.h"

class Database {
    std::map<std::string, Table> tables;

public:
    void createTable(const std::string& name, const std::vector<Column>& columns);
    void insertInto(const std::string& table, const std::vector<std::string>& values);
    void selectAll(const std::string& table);
    void saveToDisk();
    void loadFromDisk();
};