#pragma once
#include "DataType.h"
#include <ostream>
#include <vector>

class Value
{
public:
	//! Will return a deeopopy of this object
	virtual Value* Deepcopy() const = 0;

	//! Will return a string suitable for an std::ostream
	virtual std::string GetAsOsString() const = 0;

	//! Will return the data type of this value
	DATA_TYPE GetDataType() const;

	friend std::ostream& operator<< (std::ostream& os, const Value& v)
	{
		return os << v.GetAsOsString();
	}

	//! Will attempt to return the integer data (long long), if the type matches
	long long int GetInt64() const;
	//! Will attempt to return the integer data (int), if the type matches
	int GetInt32() const;

	//! Will attempt to return the floating-point data (long double), if the type matches
	long double GetFloat64() const;
	//! Will attempt to return the floating-point data (double), if the type matches
	double GetFloat32() const;

	//! Will attempt to return the string-data, if the type matches
	const std::string& GetString() const;

	//! Will attempt to return the list-data, if the type matches
	const std::vector<Value*>& GetList() const;

protected:
	Value(DATA_TYPE type);

	DATA_TYPE type;
};
