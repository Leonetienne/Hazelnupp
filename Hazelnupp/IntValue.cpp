#include "IntValue.h"

IntValue::IntValue(const long long int& value)
	:
	Value(DATA_TYPE::INT),
	value { value }
{
	return;
}
