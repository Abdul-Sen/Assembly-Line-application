#include "Utilities.h"

char Utilities::delimiter;

void Utilities::setDelimiter(const char c)
{
	delimiter = c;
}

Utilities::Utilities()
{
	this->field_width = 1;
}

void Utilities::setFieldWidth(std::size_t fw)
{
	this->field_width = fw;
}

std::size_t Utilities::getFieldWidth() const
{
	return field_width;
}
const std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool & more)
{
	std::string token;

	//extracting token
	int indexofNextDelimit = str.find(this->delimiter, next_pos);
	token = str.substr(next_pos, indexofNextDelimit - next_pos);//this is finding from start. I want to say find starting from pos

	//checking if successfully extracted substring
	if (str.substr(next_pos).find(delimiter) == std::string::npos)
		more = false;
	else
		more = true;
	//checking if two delimiters without any data to extract
	if (more == true) {
		if (str.at(str.find(delimiter, next_pos)) == str.at(str.find(delimiter, next_pos) + 1)) //i.e CPU||5|Central Processing
			throw "Two delimiters with no data in between!!";
	}

	//updating field width
	if (this->field_width < token.length())
		this->field_width = token.length();

	next_pos = indexofNextDelimit + 1;
	return token;
}



const char Utilities::getDelimiter() const
{
	return this->delimiter;
}

