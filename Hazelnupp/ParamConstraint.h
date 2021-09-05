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
		static ParamConstraint Require(const std::initializer_list<std::string>& defaultValue = {}, bool required = true)
		{
			ParamConstraint pc;
			pc.defaultValue = defaultValue;
			pc.required = required;

			return pc;
		}

		//! Constructs a type-safety constraint
		static ParamConstraint TypeSafety(DATA_TYPE requiredType, bool constrainType = true)
		{
			ParamConstraint pc;
			pc.constrainType = constrainType;
			pc.requiredType = requiredType;

			return pc;
		}

		//! Constructs an incompatibility constraint. 
		//! This means, that the following parameters are NOT compatible with this one and will throw an error if passed together
		static ParamConstraint Incompatibility(const std::initializer_list<std::string>& incompatibleParameters)
		{
			ParamConstraint pc;
			pc.incompatibleParameters = incompatibleParameters;

			return pc;
		}

		//! Constructs an incompatibility constraint. 
		//! This means, that the following parameters are NOT compatible with this one and will throw an error if passed together.
		//! Syntactical-sugar proxy method that will convert the lonely string to an initializer list for you :3
		static ParamConstraint Incompatibility(const std::string& incompatibleParameters)
		{
			ParamConstraint pc;
			pc.incompatibleParameters = { incompatibleParameters };

			return pc;
		}

		//! Whole constructor
		ParamConstraint(bool constrainType, DATA_TYPE requiredType, const std::initializer_list<std::string>& defaultValue, bool required, const std::initializer_list<std::string>& incompatibleParameters)
			:
			constrainType{ constrainType },
			requiredType{ requiredType },
			defaultValue{ defaultValue },
			required{ required },
			incompatibleParameters{incompatibleParameters}
		{
			return;
		}

		//! Should this parameter be forced to be of a certain type?  
		//! Remember to set `constrainTo` to the wanted type
		bool constrainType = false;

		//! Constrain the parameter to this value. Requires `constrainType` to be set to true.
		DATA_TYPE requiredType = DATA_TYPE::VOID;

		//! The default value for this parameter.  
		//! Gets applied if this parameter was not given.  
		//! Think of this like a list of parameters. Like {"--width", "800"}
		std::vector<std::string> defaultValue;

		//! If set to true, and no default value set,
		//! an error will be produced if this parameter is not supplied by the user.
		bool required = false;

		//! Parameters that are incompatible with this parameter
		std::vector<std::string> incompatibleParameters;

	private:
		//! The parameter this constraint is for.
		//! This value is automatically set by Hazelnupp.
		std::string key;

		friend class CmdArgsInterface;
	};
}
