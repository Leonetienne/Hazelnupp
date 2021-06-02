#pragma once
#include "Value.h"

class VoidValue : public Value
{
public:
	VoidValue();

	//! Will return a deeopopy of this object
	Value* Deepcopy() const override;

	//! Will return a string suitable for an std::ostream;
	std::string GetAsOsString() const override;
};
