#pragma once
#include "Value.h"

class VoidValue : public Value
{
public:
	VoidValue();

	Value* Deepcopy() const override;

	friend std::ostream& operator<< (std::ostream& os, const VoidValue& v)
	{
		return os << "VoidValue";
	}

	std::string GetAsOsString() const override;
};
