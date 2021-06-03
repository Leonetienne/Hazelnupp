#pragma once
#include "Value.h"
#include <string>

namespace Hazelnp
{
	/** Specializations for string values (uses std::string)
	*/
	class StringValue : public Value
	{
	public:
		StringValue(const std::string& value);
		~StringValue() override {};

		//! Will return a deeopopy of this object
		Value* Deepcopy() const override;

		//! Will return a string suitable for an std::ostream;
		std::string GetAsOsString() const override;

		//! Will return the raw value
		const std::string& GetValue() const;

		operator std::string() const;

		//! Throws HazelnuppValueNotConvertibleException
		long long int GetInt64() const override;
		//! Throws HazelnuppValueNotConvertibleException
		int GetInt32() const override;

		//! Throws HazelnuppValueNotConvertibleException
		long double GetFloat64() const override;
		//! Throws HazelnuppValueNotConvertibleException
		double GetFloat32() const override;

		//! Will return this value as a string
		std::string GetString() const override;

		//! Throws HazelnuppValueNotConvertibleException
		const std::vector<Value*>& GetList() const override;

	private:
		std::string value;
	};
}
