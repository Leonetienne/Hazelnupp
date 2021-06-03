#pragma once
#include "Value.h"

/** Specializations for integer values (uses long long int)
*/
class IntValue : public Value
{
public:
	IntValue(const long long int& value);
	~IntValue() override {};

	//! Will return a deeopopy of this object
	Value* Deepcopy() const override;

	//! Will return a string suitable for an std::ostream;
	std::string GetAsOsString() const override;

	//! Will return the raw value
	const long long int& GetValue() const;

	operator long long int() const;
	operator int() const;


	//! Will return the data as a long long int
	long long int GetInt64() const override;
	//! Will return the data as an int
	int GetInt32() const override;

	//! Will return the data as a long double
	long double GetFloat64() const override;
	//! Will return the data as a double
	double GetFloat32() const override;

	//! Will return the data as a string
	std::string GetString() const override;

	//! Throws HazelnuppValueNotConvertibleException
	const std::vector<Value*>& GetList() const override;

private:
	long long int value;
};
