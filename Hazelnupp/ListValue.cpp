#include "ListValue.h"

ListValue::~ListValue()
{
	for (Value* val : value)
		delete val;

	value.clear();
	
	return;
}

void ListValue::AddValue(const Value* value)
{
	this->value.emplace_back(new Value(*value));
	return;
}
