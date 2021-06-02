#pragma once
#include "Value.h"
#include <string>

class StringValue : public Value
{
public:
	StringValue(const std::string& value);
	~StringValue() override {};

	//! Will return a deeopopy of this object
	Value* Deepcopy() const override;

	//! Will return a string suitable for an std::ostream;
	std::string GetAsOsString() const override;

	//! Will return the raw value
	const std::string& GetValue() const;

	operator std::string() const;

private:
	std::string value;
};
