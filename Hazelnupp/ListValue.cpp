#include "ListValue.h"
#include "Hazelnupp.h"
#include <sstream>

ListValue::ListValue() :
	Value(DATA_TYPE::LIST)
{
	return;
}

ListValue::~ListValue()
{
	for (Value* val : value)
		delete val;

	value.clear();
	
	return;
}

Value* ListValue::Deepcopy() const
{
	ListValue* newList = new ListValue();

	for (const Value* val : value)
		newList->AddValue(val);

	return newList;
}

void ListValue::AddValue(const Value* value)
{
	this->value.emplace_back(value->Deepcopy());
	return;
}

std::string ListValue::GetAsOsString() const
{
	std::stringstream ss;

	ss << "ListValue: [";

	for (const Value* val : value)
	{
		ss << *val;
		if (val != value.back())
			ss << ", ";
	}

	ss << "]";

	return ss.str();
}