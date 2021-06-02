#pragma once
#include "Value.h"
#include <string>

class StringValue : public Value
{
public:
	StringValue(const std::string& value);

private:
	std::string value;
};
