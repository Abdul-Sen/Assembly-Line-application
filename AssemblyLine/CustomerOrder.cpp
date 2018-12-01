#include <vector>
#include "Item.h"
#include "CustomerOrder.h"
#include "Utilities.h"
CustomerOrder::CustomerOrder()
{
	Name = '\0';
	Product = '\0';
	ItemCount = 0;
	ItemInfo** ItemList = nullptr;
	field_width = 0;
}

CustomerOrder::CustomerOrder(std::string & Order)
{
	Utilities temp;
	bool MoreString = true;
	size_t position = 0;

	Name = temp.extractToken(Order, position, MoreString);
	Product = temp.extractToken(Order, position, MoreString);

	std::vector<ItemInfo> ItemInformation;
	std::string currentItem;
	while (MoreString)
	{
		currentItem = temp.extractToken(Order, position, MoreString);
		ItemInformation.push_back(currentItem);
	}

	int size = ItemInformation.size();
	ItemList = new ItemInfo*[size]; //alloc to size of dyn array
	for (int i = 0; i<size; i++)
	{
		ItemList[i] = nullptr;
	}
	for (auto i : ItemInformation)
	{
		ItemList[ItemCount++] = new ItemInfo(i.ItemName);
	}
	field_width = temp.getFieldWidth();
}


CustomerOrder::~CustomerOrder()
{
	if (ItemCount != 0)
	{
		for (int i = 0; i < ItemCount; i++)
		{
			delete ItemList[i];
			ItemList[i] = nullptr;
		}
	}
	ItemList = nullptr;
	Name = '\0';
	Product = '\0';
	ItemCount = 0;
	field_width = 0;
}

CustomerOrder::CustomerOrder(CustomerOrder && src)
{
	*this = std::move(src); //calling copy assign
}

CustomerOrder & CustomerOrder::operator=(CustomerOrder && src)
{
	if (this != &src)
	{
		this->Name = src.Name;
		this->field_width = src.field_width;
		Product = src.Product;
		ItemCount = src.ItemCount;
		ItemList = src.ItemList;

		src.ItemList = nullptr;
		src.ItemCount = 0;
		src.Product = "";
		src.field_width = 0;
		src.Name = "";
	}
	else {
		throw "Self assignment Not allowed!";
	}
	return *this;
}

bool CustomerOrder::getOrderFillState()
{
	for (int i = 0; i < this->ItemCount; i++)
	{
		if (this->ItemList[i]->FillState == false)
			return false;
	}
	return true;
}

bool CustomerOrder::getItemFillState(std::string itemName)
{
	for (int i = 0; i < this->ItemCount; i++)
	{
		if (this->ItemList[i]->ItemName == itemName)
		{
			return this->ItemList[i]->FillState;
		}
	}
}

void CustomerOrder::fillItem(Item& item_, std::ostream& os)
{
	for (unsigned int i = 0; i < this->ItemCount; i++) {
		if (ItemList[i]->ItemName == item_.getName())
		{
			ItemList[i]->SerialNumber = item_.getSerialNumber();
			ItemList[i]->FillState = true;
			os << "Filled " << this->Name <<", " << this->Product <<" [" <<ItemList[i]->ItemName<<"] \n";
		}
	}

}

void CustomerOrder::Display(std::ostream & os)
{
	os << this->Name << std::endl;
	os << Product << std::endl;
	for (int i = 0; i < this->ItemCount; i++)
	{
		os << "["<<this->ItemList[i]->SerialNumber<<"] " << this->ItemList[i]->ItemName;
		if (this->ItemList[i]->FillState == false)
		{
			os << " - MISSING" << std::endl;
		}
		else
		{
			os <<" - FILLED" << std::endl;
		}
	}
}
