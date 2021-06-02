#pragma once
#include "Value.h"

class IntValue : public Value
{
public:
	IntValue(const long long int& value);

	//! Will return a deeopopy of this object
	Value* Deepcopy() const override;

	//! Will return a string suitable for an std::ostream;
	std::string GetAsOsString() const override;

	operator long long int() const;
	operator long int() const;
	operator int() const;

private:
	long long int value;
};
