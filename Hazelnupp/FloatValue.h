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

	operator long double () const;
	operator double() const;

private:
	long double value;
};
