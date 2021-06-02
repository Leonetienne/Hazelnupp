#pragma once
#include "Value.h"

class IntValue : public Value
{
public:
	IntValue(const long long int& value);

private:
	long long int value;
};
