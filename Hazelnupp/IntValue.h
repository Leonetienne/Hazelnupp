#pragma once
#include "Value.h"

class IntValue : public Value
{
public:
	IntValue(const long long int& value);
	~IntValue() override {};

	//! Will return a deeopopy of this object
	Value* Deepcopy() const override;

	//! Will return a string suitable for an std::ostream;
	std::string GetAsOsString() const override;

	//! Will return the raw value
	const long long int& GetValue() const;

	operator long long int() const;
	operator int() const;

private:
	long long int value;
};
