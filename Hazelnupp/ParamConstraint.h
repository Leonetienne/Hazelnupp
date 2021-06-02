#pragma once
#include "DataType.h"
#include <string>
#include <vector>

struct ParamConstraint
{
public:
	//! The key of the parameter to constrain
	std::string key;
	
	//! Should this parameter be forced to be of a certain type?  
	//! Remember to set `constrainTo` to the wanted type
	bool constrainType = false;

	//! Constrain the parameter to this value. Requires `constrainType` to be set to true.
	DATA_TYPE wantedType = DATA_TYPE::VOID;

	//! The default value for this parameter.  
	//! Gets applied if this parameter was not given.
	std::vector<std::string> defaultValue;

	//! If set to true, and no default value set,
	//! an error will be produced if this parameter is not supplied by the user.
	bool required = false;
};
