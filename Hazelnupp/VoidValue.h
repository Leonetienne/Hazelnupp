#pragma once
#include "Value.h"

/** Specializations for void values. These house no value whatsoever, but only communicate information by merely existing.
*/
class VoidValue : public Value
{
public:
	VoidValue();
	~VoidValue() override {};

	//! Will return a deeopopy of this object
	Value* Deepcopy() const override;

	//! Will return a string suitable for an std::ostream;
	std::string GetAsOsString() const override;

	//! Throws HazelnuppValueNotConvertibleException
	long long int GetInt64() const override;
	//! Throws HazelnuppValueNotConvertibleException
	int GetInt32() const override;

	//! Throws HazelnuppValueNotConvertibleException
	long double GetFloat64() const override;
	//! Throws HazelnuppValueNotConvertibleException
	double GetFloat32() const override;

	//! Throws HazelnuppValueNotConvertibleException
	std::string GetString() const override;

	//! Throws HazelnuppValueNotConvertibleException
	const std::vector<Value*>& GetList() const;
};
