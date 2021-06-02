#pragma once
#include "Value.h"
#include <string>
#include <ostream>

class Parameter
{
public:
	explicit Parameter(const std::string& key, const Value* value);
	
	const std::string& Key() const;
	const Value* Value() const;

	friend std::ostream& operator<< (std::ostream& os, const Parameter& p)
	{
		return os << "{ Key: \"" << p.key << "\" -> " << *p.value << " }";
	}

private:
	std::string key;
	::Value* value;
};
