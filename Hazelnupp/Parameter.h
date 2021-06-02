#pragma once
#include "Value.h"
#include <string>

class Parameter
{
public:
	explicit Parameter(const std::string& key, const Value& value);
	
private:
	std::string key;
	Value value;
};
