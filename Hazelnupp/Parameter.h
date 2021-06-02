#pragma once
#include "Value.h"
#include <string>
#include <ostream>

class Parameter
{
public:
	explicit Parameter(const std::string& key, const Value* value);
	~Parameter();
	
	//! Will return the key of this parameter
	const std::string& Key() const;

	//! Will return the value of this parameter
	const Value* GetValue() const;

	friend std::ostream& operator<< (std::ostream& os, const Parameter& p)
	{
		return os << "{ Key: \"" << p.key << "\" -> " << *p.value << " }";
	}

private:
	std::string key;
	::Value* value;
};
