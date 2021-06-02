#pragma once
#include "DataType.h"
#include <ostream>

class Value
{
public:
	//! Will return a deeopopy of this object
	virtual Value* Deepcopy() const = 0;

	friend std::ostream& operator<< (std::ostream& os, const Value& v)
	{
		return os << v.GetAsOsString();
	}

	//! Will return a string suitable for an std::ostream;
	virtual std::string GetAsOsString() const = 0;

protected:
	Value(DATA_TYPE type);

	DATA_TYPE type;
};
