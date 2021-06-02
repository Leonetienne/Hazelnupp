#pragma once
#include "Value.h"

class IntValue : public Value
{
public:
	IntValue(const long long int& value);

	Value* Deepcopy() const override;

	std::string GetAsOsString() const override;

private:
	long long int value;
};
