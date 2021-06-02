#pragma once
#include "DataType.h"
#include <string>
#include <vector>

struct ParamConstraint
{
public:
	//! Empty constructor
	ParamConstraint() = default;

	//! Whole constructor
	ParamConstraint(const std::string& key, bool constrainType, DATA_TYPE wantedType, const std::vector<std::string>& defaultValue, bool required)
		:
		key { key },
		constrainType { constrainType },
		wantedType { wantedType },
		defaultValue { defaultValue },
		required{ required }
	{
		return;
	}

	//! Type-Constraint constructor
	ParamConstraint(const std::string& key, bool constrainType, DATA_TYPE wantedType)
		:
		key{ key },
		constrainType{ constrainType },
		wantedType{ wantedType }
	{
		return;
	}

	//! Require-Constraint constructor
	ParamConstraint(const std::string& key, const std::vector<std::string>& defaultValue, bool required = false)
		:
		key{ key },
		defaultValue{ defaultValue },
		required{ required }
	{
		return;
	}

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
