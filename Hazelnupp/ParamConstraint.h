#pragma once
#include "DataType.h"
#include <string>
#include <vector>

namespace Hazelnp
{
	struct ParamConstraint
	{
	public:
		//! Empty constructor
		ParamConstraint() = default;

		//! Constructs a require constraint.  
		//! Think of the default value like of a list ofparameters. Like {"--width", "800"}
		static ParamConstraint Require(const std::vector<std::string>& defaultValue = {}, bool required = true)
		{
			ParamConstraint pc;
			pc.defaultValue = defaultValue;
			pc.required = required;

			return pc;
		}

		//! Constructs a type-safety constraint
		static ParamConstraint TypeSafety(DATA_TYPE wantedType, bool constrainType = true)
		{
			ParamConstraint pc;
			pc.constrainType = constrainType;
			pc.wantedType = wantedType;

			return pc;
		}

		//! Whole constructor
		ParamConstraint(bool constrainType, DATA_TYPE wantedType, const std::vector<std::string>& defaultValue, bool required)
			:
			constrainType{ constrainType },
			wantedType{ wantedType },
			defaultValue{ defaultValue },
			required{ required }
		{
			return;
		}

		//! Should this parameter be forced to be of a certain type?  
		//! Remember to set `constrainTo` to the wanted type
		bool constrainType = false;

		//! Constrain the parameter to this value. Requires `constrainType` to be set to true.
		DATA_TYPE wantedType = DATA_TYPE::VOID;

		//! The default value for this parameter.  
		//! Gets applied if this parameter was not given.  
		//! Think of this like a list of parameters. Like {"--width", "800"}
		std::vector<std::string> defaultValue;

		//! If set to true, and no default value set,
		//! an error will be produced if this parameter is not supplied by the user.
		bool required = false;

	private:
		//! The parameter this constraint is for.
		//! This value is automatically set by Hazelnupp.
		std::string key;

		friend class Hazelnupp;
	};
}
