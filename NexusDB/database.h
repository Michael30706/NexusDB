#pragma once
#include <string>
#include <map>
#include "table.h"
#include <unordered_set>

class Database {
    std::map<std::string, Table> tables;
    std::string _name;
public:
    static std::unordered_set<std::string> types;
    static bool checkTypes(const std::vector<Column>& columns);
    Database();
    void createTable(const std::string& name, const std::vector<Column>& columns);
    void insertInto(const std::string& table, const std::vector<std::string>& values);
    void selectAll(const std::string& table);
    void saveToDisk();
    void loadFromDisk();
};