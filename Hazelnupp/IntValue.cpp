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
