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
