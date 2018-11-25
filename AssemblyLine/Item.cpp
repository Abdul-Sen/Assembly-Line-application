#include <string>
#include "Item.h"
#include "Utilities.h"

unsigned int Item::field_width;

Item::Item(std::string & record)
{
	Utilities temp;
	bool mybool = true;
	size_t position = 0;
	try {
		this->name = temp.extractToken(record, position, mybool);
		this->serialNumber = std::stoi(temp.extractToken(record, position, mybool));
		this->Quantity = std::stoi(temp.extractToken(record, position, mybool));
		this->description = temp.extractToken(record, position, mybool);
		if (this->field_width < temp.getFieldWidth())
			this->field_width = temp.getFieldWidth();
	}
	catch (char const* error)
	{
		this->name = "";
		this->serialNumber = 0;
		this->Quantity = 0;
		this->description = "";
		std::cout << error;
		std::cin.get();

	}
}

const std::string & Item::getName() const
{
	return name;
}

const unsigned int Item::getSerialNumber()
{
	return this->serialNumber++;
}

const unsigned int Item::getQuantity()
{
	return this->Quantity;
}

void Item::updateQuantity()
{
	if(Quantity >1)
	this->Quantity = Quantity - 1;

}

void Item::display(std::ostream & os, bool full) const
{
	if (this->serialNumber != 0)
	{
		os << this->name;
		os.setf(std::ios::right);
		//os.width(this->field_width);//really trying to put a square through a cylinder
		os.width(5);
		os << " [" << this->serialNumber << "] ";
		if (full)
		{
			os << "Quantity " << this->Quantity << " Description: " << this->description << std::endl;
		}
		else
		{
			os << std::endl;
		}

	}
}