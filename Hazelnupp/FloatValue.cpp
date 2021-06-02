#include "FloatValue.h"
#include <sstream>

FloatValue::FloatValue(const long double& value)
	:
	Value(DATA_TYPE::FLOAT),
	value { value }
{
	return;
}

Value* FloatValue::Deepcopy() const
{
	return new FloatValue(value);
}

std::string FloatValue::GetAsOsString() const
{
	std::stringstream ss;
	ss << "FloatValue: " << value;
	return ss.str();
}

FloatValue::operator long double() const
{
	return value;
}

FloatValue::operator double() const
{
	return (double)value;
}
