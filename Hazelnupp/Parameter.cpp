#include "Parameter.h"

Parameter::Parameter(const std::string& key, const Value& value)
	:
	key { key },
	value { value }
{
	return;
}
