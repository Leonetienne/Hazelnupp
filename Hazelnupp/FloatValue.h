#pragma once
#include "Value.h"
#include <ostream>

class FloatValue : public Value
{
public:
	FloatValue(const long double& value);
	~FloatValue() override {};

	//! Will return a deeopopy of this object
	Value* Deepcopy() const override;

	//! Will return a string suitable for an std::ostream;
	std::string GetAsOsString() const override;

	//! Will return the raw value
	const long double& GetValue() const;

	operator long double () const;
	operator double() const;

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
	long double value;
};
