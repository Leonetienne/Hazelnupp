#include "Value.h"

Value::Value(DATA_TYPE type)
	:
	type{ type }
{
	return;
}

DATA_TYPE Value::GetDataType() const
{
	return type;
}
