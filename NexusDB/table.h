#include <iostream>
#include <vector>

struct Column {
    std::string name;
    std::string type; // "INT" or "STRING"
};

struct Cell
{
    std::string column;
    std::string value;
};


class Table {
public:
    std::string name;
    std::vector<Column> columns;
    std::vector<std::vector<Cell>> rows;

    void insertRow(const std::vector<Cell>& values);
    void display() const;
};