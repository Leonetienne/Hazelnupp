#pragma once
#include "Value.h"
#include <string>

class StringValue : public Value
{
public:
	StringValue(const std::string& value);

	//! Will return a deeopopy of this object
	Value* Deepcopy() const override;

	//! Will return a string suitable for an std::ostream;
	std::string GetAsOsString() const override;

	operator std::string() const;

private:
	std::string value;
};
