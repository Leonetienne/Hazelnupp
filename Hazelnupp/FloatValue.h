#pragma once
#include "Value.h"
#include <ostream>

class FloatValue : public Value
{
public:
	FloatValue(const long double& value);

	Value* Deepcopy() const override;

	friend std::ostream& operator<< (std::ostream& os, const FloatValue& v)
	{
		return os << v.GetAsOsString();
	}

	std::string GetAsOsString() const override;

private:
	long double value;
};
