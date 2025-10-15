#pragma once
#include <string>
#include <map>
#include "table.h"
#include "helpers.hpp"
#include <unordered_set>
class Database {
    std::map<std::string, Table> tables;
    std::string _name;
public:
    static std::unordered_set<std::string> types;
    static bool checkTypes(const std::vector<Column>& columns);
    Database();
    Table getTable(const std::string& name);
    void createTable(const std::string& name, const std::vector<Column>& columns);
    void insertInto(const std::string& table, std::map<std::string, Cell>& values);
    void selectAll(const std::string& table);
    void select(const std::string& table, std::vector<Column>& requestedColumns, std::vector<Cell> conditions);
    void saveToDisk();
    void loadFromDisk();
};