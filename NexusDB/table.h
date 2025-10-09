#include <iostream>
#include <vector>

struct Column {
    std::string name;
    std::string type; // "INT" or "STRING"
};

class Table {
public:
    std::string name;
    std::vector<Column> columns;
    std::vector<std::vector<std::string>> rows;

    void insertRow(const std::vector<std::string>& values);
    void display() const;
};