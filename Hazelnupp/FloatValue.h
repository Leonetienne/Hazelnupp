#pragma once
#include "Value.h"
#include <ostream>

class FloatValue : public Value
{
public:
	FloatValue(const long double& value);

	//! Will return a deeopopy of this object
	Value* Deepcopy() const override;

	//! Will return a string suitable for an std::ostream;
	std::string GetAsOsString() const override;

	//! Will return the raw value
	const long double& GetValue() const;

	operator long double () const;
	operator double() const;

private:
	long double value;
};
