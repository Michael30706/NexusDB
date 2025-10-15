#include "table.h"

void Table::insertRow(const std::vector<Cell>& values)
{
	this->rows.push_back(values);
}

void Table::display() const
{
	std::cout << "|";

	for (auto& col : this->columns)
	{
		std::cout << col.name << "|";
	}

	std::cout << std::endl;


	for (auto& row : this->rows)
	{
		std::cout << "|";

		for (auto& cell : row)
		{
			std::cout << cell.value << "|";
		}
		std::cout << std::endl;
	}

}
