#include "IntValue.h"
#include <sstream>

IntValue::IntValue(const long long int& value)
	:
	Value(DATA_TYPE::INT),
	value { value }
{
	return;
}

Value* IntValue::Deepcopy() const
{
	return new IntValue(value);
}

std::string IntValue::GetAsOsString() const
{
	std::stringstream ss;
	ss << "IntValue: " << value;
	return ss.str();
}

IntValue::operator long long int() const
{
	return value;
}

IntValue::operator long int() const
{
	return (long int)value;
}

IntValue::operator int() const
{
	return (int)value;
}
