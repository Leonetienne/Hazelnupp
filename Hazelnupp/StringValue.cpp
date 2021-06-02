#include "StringValue.h"

StringValue::StringValue(const std::string& value)
	:
	Value(DATA_TYPE::STRING),
	value { value }
{
	return;
}
