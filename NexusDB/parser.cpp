#include "parser.h"
#include "storage.h"
#include <algorithm>
#include <cctype>
#include <fstream>

Condition toCondition(const std::string& expression, char logicGate)
{
    std::vector<std::string> columnValue = split(expression, "=");

    Condition condition;
    condition.column = columnValue[0];
    condition.value = columnValue[1];
    condition.logicGate = logicGate;

    return condition;
}


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
        else if (toUpperCopy(keyWords.at(0)) == "SELECT" && toUpperCopy(keyWords.at(2)) == "FROM")
        {
            std::string tableName = keyWords.at(3);
            std::vector<std::string> columns;

            if (keyWords.at(1) == "*")
            {
                Table table = _db->getTable(tableName);
                for (Column& col : table.columns)
                {
                    columns.push_back(col.name);
                }
            }
            else columns = split(keyWords.at(1), ",");

            std::string commandLower = toLowerCopy(command);
            std::vector<Condition> conditions = std::vector<Condition>();

            if (commandLower.find(" #where# ") != std::string::npos)
            {
                std::string condition = split(commandLower, " #where# ")[1];
                std::vector<std::string> orSplit = split(condition, " || ");
                for (auto& exp : orSplit)
                {
                    if (exp.find(" && ") != std::string::npos)
                    {
                        std::vector<std::string> expressions = split(exp, " && ");
                        for (auto& expression : expressions)
                        {
                            conditions.push_back(toCondition(expression, 'a'));
                        }
                    }
                    else
                    {
                        conditions.push_back(toCondition(exp, 'o'));
                    }
                }
            }
            _db->select(tableName, columns, conditions);

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
