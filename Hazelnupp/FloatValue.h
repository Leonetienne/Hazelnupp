#pragma once
#include "Value.h"

class FloatValue : public Value
{
public:
	FloatValue(const long double& value);

private:
	long double value;
};
