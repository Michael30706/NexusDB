#include "table.h"

void Table::insertRow(const std::vector<std::string>& values)
{
	this->rows.push_back(values);
}

void Table::display() const
{

}
