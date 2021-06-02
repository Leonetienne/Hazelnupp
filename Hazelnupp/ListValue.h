#pragma once
#include "Value.h"
#include <vector>

class ListValue : public Value
{
public:
	~ListValue();

	//! Will add this value to the list
	void AddValue(const Value* value);

private:
	std::vector<Value*> value;
};
