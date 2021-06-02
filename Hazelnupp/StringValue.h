#pragma once
#include "Value.h"
#include <string>

class StringValue : public Value
{
public:
	StringValue(const std::string& value);

	Value* Deepcopy() const override;

	friend std::ostream& operator<< (std::ostream& os, const StringValue& v)
	{
		return os << v.GetAsOsString();
	}

	std::string GetAsOsString() const override;

private:
	std::string value;
};
