#include "FloatValue.h"

FloatValue::FloatValue(const long double& value)
	:
	Value(DATA_TYPE::FLOAT),
	value { value }
{
	return;
}
