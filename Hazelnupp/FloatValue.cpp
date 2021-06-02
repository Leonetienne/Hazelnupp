#include "FloatValue.h"
#include "HazelnuppException.h"
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

const long double& FloatValue::GetValue() const
{
	return value;
}

FloatValue::operator long double() const
{
	return value;
}

FloatValue::operator double() const
{
	return (double)value;
}



long long int FloatValue::GetInt64() const
{
	return (long long int)value;
}

int FloatValue::GetInt32() const
{
	return (int)value;
}

long double FloatValue::GetFloat64() const
{
	return value;
}

double FloatValue::GetFloat32() const
{
	return (double)value;
}

std::string FloatValue::GetString() const
{
	std::stringstream ss;
	ss << value;

	return ss.str();
}

const std::vector<Value*>& FloatValue::GetList() const
{
	throw HazelnuppValueNotConvertibleException();
}
