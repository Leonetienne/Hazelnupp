#pragma once
#include "DataType.h"
#include <ostream>
#include <vector>

class Value
{
public:
	virtual ~Value() {};

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

	//! Will attempt to return the integer data (long long)
	virtual long long int GetInt64() const = 0;
	//! Will attempt to return the integer data (int)
	virtual int GetInt32() const = 0;

	//! Will attempt to return the floating-point data (long double)
	virtual long double GetFloat64() const = 0;
	//! Will attempt to return the floating-point data (double)
	virtual double GetFloat32() const = 0;

	//! Will attempt to return the string-data
	virtual std::string GetString() const = 0;

	//! Will attempt to return the list-data
	virtual const std::vector<Value*>& GetList() const = 0;

protected:
	Value(DATA_TYPE type);

	DATA_TYPE type;
};
