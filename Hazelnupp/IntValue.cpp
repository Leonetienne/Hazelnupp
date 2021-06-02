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

const long long int& IntValue::GetValue() const
{
	return value;
}

IntValue::operator long long int() const
{
	return value;
}

IntValue::operator int() const
{
	return (int)value;
}



long long int IntValue::GetInt64() const
{
	return value;
}

int IntValue::GetInt32() const
{
	return (int)value;
}

long double IntValue::GetFloat64() const
{
	return (long double)value;
}

double IntValue::GetFloat32() const
{
	return (double)value;
}

std::string IntValue::GetString() const
{
	std::stringstream ss;
	ss << value;

	return ss.str();
}

const std::vector<Value*>& IntValue::GetList() const
{
	throw std::bad_cast();
}
