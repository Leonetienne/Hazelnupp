#include "Parameter.h"

Parameter::Parameter(const std::string& key, const ::Value* value)
	:
	key{ key }
{
	this->value = value->Deepcopy();
	return;
}

const std::string& Parameter::Key() const
{
	return key;
}

const ::Value* Parameter::Value() const
{
	return value;
}