#pragma once
#include "Value.h"
#include <vector>

class ListValue : public Value
{
public:
	ListValue();
	~ListValue();

	Value* Deepcopy() const override;

	//! Will add this value to the list
	void AddValue(const Value* value);

	std::string GetAsOsString() const override;

	friend std::ostream& operator<< (std::ostream& os, const ListValue& v)
	{
		return os << v.GetAsOsString();
	}

private:
	std::vector<Value*> value;
};
