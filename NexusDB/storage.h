#pragma once
#include <string>
#include <vector>
#include "table.h"

class Storage {
public:
    // Save a table to disk
    static void saveTable(const Table& table, const std::string& folder = "data");

    // Load a table from disk
    static Table loadTable(const std::string& tableName, const std::string& folder = "data");

    // Check if table file exists
    static bool tableExists(const std::string& tableName, const std::string& folder = "data");

private:
    // Helper: serialize a row to string (binary)
    static std::string serializeRow(const std::vector<std::string>& row);

    // Helper: deserialize string to row
    static std::vector<std::string> deserializeRow(const std::string& line);
};
