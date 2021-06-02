#pragma once
#include "Value.h"

class VoidValue : public Value
{
public:
	VoidValue();
	~VoidValue() override {};

	//! Will return a deeopopy of this object
	Value* Deepcopy() const override;

	//! Will return a string suitable for an std::ostream;
	std::string GetAsOsString() const override;

	//! Throws std::bad_cast
	long long int GetInt64() const override;
	//! Throws std::bad_cast
	int GetInt32() const override;

	//! Throws std::bad_cast
	long double GetFloat64() const override;
	//! Throws std::bad_cast
	double GetFloat32() const override;

	//! Throws std::bad_cast
	std::string GetString() const override;

	//! Throws std::bad_cast
	const std::vector<Value*>& GetList() const;
};
