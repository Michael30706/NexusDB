#include "parser.h"
#include "storage.h"
#include <algorithm>
#include <cctype>
#include <fstream>


Parser::Parser(Database* db) : _db(db)
{

}

std::string Parser::handleCommand(const std::string& command)
{
    try
    {
        std::vector<std::string> keyWords = split(command, " ");
        if (toUpperCopy(keyWords.at(0)) == "CREATE" && toUpperCopy(keyWords.at(1)) == "TABLE")
        {
            std::string name = keyWords.at(2);
            std::vector<std::string> types = std::vector<std::string>();
            std::vector<std::string> columnNames = std::vector<std::string>();
            for (int i = 3; i < keyWords.size(); i++)
            {
                if (isAlphaString(keyWords.at(i)))
                {
                    if (columnNames.size() == types.size()) columnNames.push_back(keyWords.at(i));
                    else types.push_back(keyWords.at(i));
                }
            }

            std::vector<Column> columns = std::vector<Column>();

            for (int i = 0; i < columnNames.size(); i++)
            {
                Column column;
                column.name = columnNames.at(i);
                column.type = types.at(i);
                columns.push_back(column);
            }

            _db->createTable(name, columns);
        }
        else if (toUpperCopy(keyWords.at(0)) == "INSERT" && toUpperCopy(keyWords.at(1)) == "INTO")
        {
            std::string table = keyWords.at(2);
            std::map<std::string, Cell> cells = std::map<std::string, Cell>();
            for (int i = 3; i < keyWords.size(); i++)
            {
                std::vector<std::string> kvps = split(keyWords[i], "=");

                std::string col = kvps[0];
                std::string val = kvps[1];

                Cell cell;
                cell.column = col;
                cell.value = val;

                cells.insert({cell.column, cell});
            }
            _db->insertInto(table, cells);

        }
        else
        {
            Table table = _db->getTable(command);
            table.display();
            throw std::exception();
        }
    }
    catch (...)
    {
        return "Syntax Error";
    }
    return "success!";
}
