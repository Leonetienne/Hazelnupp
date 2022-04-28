#include "Hazelnupp/ListValue.h"
#include "Hazelnupp/HazelnuppException.h"
#include <sstream>

using namespace Hazelnp;

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

const std::vector<Value*>& ListValue::GetValue() const
{
	return value;
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

ListValue::operator std::vector<Value*>() const
{
	return value;
}



long long int ListValue::GetInt64() const
{
	throw HazelnuppValueNotConvertibleException();
}

int ListValue::GetInt32() const
{
	throw HazelnuppValueNotConvertibleException();
}

long double ListValue::GetFloat64() const
{
	throw HazelnuppValueNotConvertibleException();
}

double ListValue::GetFloat32() const
{
	throw HazelnuppValueNotConvertibleException();
}

std::string ListValue::GetString() const
{
	throw HazelnuppValueNotConvertibleException();
}

const std::vector<Value*>& ListValue::GetList() const
{
	return value;
}
