#include "parser.h"
#include <algorithm>
#include <cctype>

bool isAlphaString(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c) {
        return std::isalpha(c);
        });
}

std::string toUpperCopy(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::toupper(c); });
    return result;
}

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
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
        else
        {
            throw std::exception();
        }
    }
    catch (...)
    {
        return "Syntax Error";
    }
    return "success!";
}
