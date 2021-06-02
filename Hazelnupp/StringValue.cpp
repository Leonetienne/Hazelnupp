#include "StringValue.h"
#include <sstream>

StringValue::StringValue(const std::string& value)
	:
	Value(DATA_TYPE::STRING),
	value { value }
{
	return;
}

Value* StringValue::Deepcopy() const
{
	return new StringValue(value);
}

std::string StringValue::GetAsOsString() const
{
	std::stringstream ss;
	ss << "StringValue: " << value;
	return ss.str();
}

const std::string& StringValue::GetValue() const
{
	return value;
}

StringValue::operator std::string() const
{
	return value;
}



long long int StringValue::GetInt64() const
{
	throw std::bad_cast();
}

int StringValue::GetInt32() const
{
	throw std::bad_cast();
}

long double StringValue::GetFloat64() const
{
	throw std::bad_cast();
}

double StringValue::GetFloat32() const
{
	throw std::bad_cast();
}

std::string StringValue::GetString() const
{
	return value;
}

const std::vector<Value*>& StringValue::GetList() const
{
	throw std::bad_cast();
}
