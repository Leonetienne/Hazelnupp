#pragma once
#include "Value.h"
#include <vector>

namespace Hazelnp
{
	/** Specializations for list values (uses std::vector<Value*>)
	*/
	class ListValue : public Value
	{
	public:
		ListValue();
		~ListValue() override;

		//! Will return a deeopopy of this object
		Value* Deepcopy() const override;

		//! Will return a string suitable for an std::ostream;
		std::string GetAsOsString() const override;

		//! Will add this value to the list
		void AddValue(const Value* value);

		//! Will return the raw value
		const std::vector<Value*>& GetValue() const;

		operator std::vector<Value*>() const;

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

		//! Will return this values list
		const std::vector<Value*>& GetList() const override;

	private:
		std::vector<Value*> value;
	};
}
