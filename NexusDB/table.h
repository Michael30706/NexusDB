#include <iostream>
#include <vector>

struct Column {
    std::string name;
    std::string type; // "INT" or "STRING"
    bool operator==(const Column& other) const;
};

struct Cell
{
    std::string column;
    std::string value;
    bool operator==(const Cell& other) const;
};

namespace std {
    template <>
    struct hash<Column> {
        std::size_t operator()(const Column& col) const {
            return std::hash<std::string>()(col.name) ^ (std::hash<std::string>()(col.type) << 1);
        }
    };
}


class Table {
public:
    std::string name;
    std::vector<Column> columns;
    std::vector<std::vector<Cell>> rows;

    void insertRow(const std::vector<Cell>& values);
    void display() const;
};