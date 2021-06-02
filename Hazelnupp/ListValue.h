#pragma once
#include "Value.h"
#include <vector>

class ListValue : public Value
{
public:
	ListValue();
	~ListValue();

	//! Will return a deeopopy of this object
	Value* Deepcopy() const override;

	//! Will return a string suitable for an std::ostream;
	std::string GetAsOsString() const override;

	//! Will add this value to the list
	void AddValue(const Value* value);

	//! Will return the raw value
	const std::vector<Value*>& GetValue() const;

	operator std::vector<Value*>() const;

private:
	std::vector<Value*> value;
};
