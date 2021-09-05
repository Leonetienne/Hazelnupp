#pragma once

/*** ../Hazelnupp/StringTools.h ***/

#include <string>
#include <sstream>
#include <vector>
#include <cmath>

namespace Hazelnp
{
	namespace Internal
	{
		/** Internal helper class. Feel free to use it tho.
		*/
		class StringTools
		{
		public:
			//! Will return wether or not a given char is in a string
			static bool Contains(const std::string& str, const char c);

			//! Will replace a part of a string with another string
			static std::string Replace(const std::string& str, const char find, const std::string& subst);

			//! Will replace a part of a string with another string
			static std::string Replace(const std::string& str, const std::string& find, const std::string& subst);

			//! Will return true if the given string consists only of digits (including signage)
			static bool IsNumeric(const std::string& str, const bool allowDecimalPoint = false);

			//! Will convert the number in str to a number.  
			//! Returns wether or not the operation was successful.  
			//! Also returns wether the number is an integer, or floating point. If int, cast out_number to int.
			static bool ParseNumber(const std::string& str, bool& out_isInt, long double& out_number);

			//! Will split a string by a delimiter char. The delimiter will be excluded!
			static std::vector<std::string> SplitString(const std::string& str, const char delimiter);

			//! Will split a string by a delimiter string. The delimiter will be excluded!
			static std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter);

			//! Will make a string all lower-case
			static std::string ToLower(const std::string& str);
		};
	}
}


/*** ../Hazelnupp/Placeholders.h ***/

#include <string>

namespace Hazelnp
{
	namespace Placeholders
	{
		//! The only purpose of this is to provide the ability to return an empty string as an error for std::string& methods.
		static const std::string g_emptyString;
	}
}

/*** ../Hazelnupp/DataType.h ***/

#include <string>

namespace Hazelnp
{
	/** The different data types a paramater can be
	*/
	enum class DATA_TYPE
	{
		VOID,
		INT,
		FLOAT,
		STRING,
		LIST
	};

	static inline std::string DataTypeToString(DATA_TYPE type)
	{
		switch (type)
		{
		case DATA_TYPE::VOID:
			return "VOID";

		case DATA_TYPE::INT:
			return "INT";

		case DATA_TYPE::FLOAT:
			return "FLOAT";

		case DATA_TYPE::STRING:
			return "STRING";

		case DATA_TYPE::LIST:
			return "LIST";
		}

		return "";
	}
}

/*** ../Hazelnupp/HazelnuppException.h ***/

#include <stdexcept>
#include <string>
#include <sstream>

namespace Hazelnp
{
	/** Generic hazelnupp exception
	*/
	class HazelnuppException : public std::exception
	{
	public:
		HazelnuppException() {};
		HazelnuppException(const std::string& msg) : message{ msg } {};

		//! Will return an error message
		const std::string& What() const
		{
			return message;
		}

	protected:
		std::string message;
	};

	/** Gets thrown when an non-existent key gets dereferenced
	*/
	class HazelnuppInvalidKeyException : public HazelnuppException
	{
	public:
		HazelnuppInvalidKeyException() : HazelnuppException() {};
		HazelnuppInvalidKeyException(const std::string& msg) : HazelnuppException(msg) {};
	};

	/** Gets thrown when an attempt is made to retrieve the wrong data type from a value, when the value not convertible
	*/
	class HazelnuppValueNotConvertibleException : public HazelnuppException
	{
	public:
		HazelnuppValueNotConvertibleException() : HazelnuppException() {};
		HazelnuppValueNotConvertibleException(const std::string& msg) : HazelnuppException(msg) {};
	};

	/** Gets thrown something bad happens because of parameter constraints
	*/
	class HazelnuppConstraintException : public HazelnuppException
	{
	public:
		HazelnuppConstraintException() : HazelnuppException() {};
		HazelnuppConstraintException(const std::string& msg) : HazelnuppException(msg) {};
	};

	/** Gets thrown when a parameter is of a type that does not match the required type, and is not convertible to it
	*/
	class HazelnuppConstraintTypeMissmatch : public HazelnuppConstraintException
	{
	public:
		HazelnuppConstraintTypeMissmatch() : HazelnuppConstraintException() {};
		HazelnuppConstraintTypeMissmatch(const std::string& msg) : HazelnuppConstraintException(msg) {};
		
		HazelnuppConstraintTypeMissmatch(const std::string& key, const DATA_TYPE requiredType, const DATA_TYPE actualType, const std::string& paramDescription = "")
		{
			// Generate descriptive error message
			std::stringstream ss;
			ss << "Cannot convert parameter " << key << " to type " << DataTypeToString(requiredType)
				<< ". You supplied type: " << DataTypeToString(actualType) << ".";

			// Add the parameter description, if provided
			if (paramDescription.length() > 0)
				ss << std::endl << key << "   => " << paramDescription;

			message = ss.str();
			return;
		};
	};

	/** Gets thrown when a parameter constrained to be required is not provided, and has no default value set
	*/
	class HazelnuppConstraintMissingValue : public HazelnuppConstraintException
	{
	public:
		HazelnuppConstraintMissingValue() : HazelnuppConstraintException() {};
		HazelnuppConstraintMissingValue(const std::string& key, const std::string& paramDescription = "")
		{
			// Generate descriptive error message
			std::stringstream ss;
			ss << "Missing required parameter " << key << ".";

			// Add the parameter description, if provided
			if (paramDescription.length() > 0)
				ss << std::endl << key << "   => " << paramDescription;

			message = ss.str();
			return;
		};
	};

	/** Gets thrown when a parameter constrained to be incompatible with other parameters gets supplied alongside at least one of those incompatible ones
	*/
	class HazelnuppConstraintIncompatibleParameters : public HazelnuppConstraintException
	{
	public:
		HazelnuppConstraintIncompatibleParameters() : HazelnuppConstraintException() {};
		HazelnuppConstraintIncompatibleParameters(const std::string& key1, const std::string& key2)
		{
			// Generate descriptive error message
			std::stringstream ss;
			ss << "Parameter \"" << key1 << "\" is NOT compatible with parameter \"" << key2 << "\"!";

			message = ss.str();
			return;
		};
	};
}

/*** ../Hazelnupp/ParamConstraint.h ***/

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

		//! Daisychain-method. Will add a the "required-argument" aspect.  
		//! Think of the default value like of a list ofparameters. Like {"--width", "800"}
		ParamConstraint AddRequire(const std::initializer_list<std::string>& defaultValue = {}, bool required = true)
		{
			ParamConstraint pc = *this;
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

		//! Daisychain-method. Will add a the "type-safety" aspect.  
		//! Constructs a type-safety constraint
		ParamConstraint AddTypeSafety(DATA_TYPE requiredType, bool constrainType = true)
		{
			ParamConstraint pc = *this;
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

		//! Daisychain-method. Will add a the "incompatiblity" aspect.  
		//! This means, that the following parameters are NOT compatible with this one and will throw an error if passed together.
		//! Syntactical-sugar proxy method that will convert the lonely string to an initializer list for you :3
		ParamConstraint AddIncompatibilities(const std::string& incompatibleParameters)
		{
			ParamConstraint pc = *this;
			pc.incompatibleParameters = { incompatibleParameters };

			return pc;
		}

		//! Daisychain-method. Will add a the "incompatiblity" aspect.  
		//! This means, that the following parameters are NOT compatible with this one and will throw an error if passed together.
		ParamConstraint AddIncompatibilities(const std::initializer_list<std::string>& incompatibleParameters)
		{
			ParamConstraint pc = *this;
			pc.incompatibleParameters = incompatibleParameters;

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

/*** ../Hazelnupp/Version.h ***/

#define HAZELNUPP_VERSION (1.121)

/*** ../Hazelnupp/Value.h ***/

#include <ostream>
#include <vector>

namespace Hazelnp
{
	/** Abstract class for values
	*/
	class Value
	{
	public:
		virtual ~Value() {};

		//! Will return a deeopopy of this object
		virtual Value* Deepcopy() const = 0;

		//! Will return a string suitable for an std::ostream
		virtual std::string GetAsOsString() const = 0;

		//! Will return the data type of this value
		DATA_TYPE GetDataType() const;

		friend std::ostream& operator<< (std::ostream& os, const Value& v)
		{
			return os << v.GetAsOsString();
		}

		//! Will attempt to return the integer data (long long)
		virtual long long int GetInt64() const = 0;
		//! Will attempt to return the integer data (int)
		virtual int GetInt32() const = 0;

		//! Will attempt to return the floating-point data (long double)
		virtual long double GetFloat64() const = 0;
		//! Will attempt to return the floating-point data (double)
		virtual double GetFloat32() const = 0;

		//! Will attempt to return the string-data
		virtual std::string GetString() const = 0;

		//! Will attempt to return the list-data
		virtual const std::vector<Value*>& GetList() const = 0;

	protected:
		Value(DATA_TYPE type);

		DATA_TYPE type;
	};
}

/*** ../Hazelnupp/ListValue.h ***/

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

/*** ../Hazelnupp/FloatValue.h ***/

#include <ostream>

namespace Hazelnp
{
	/** Specializations for floating point values (uses long double)
	*/
	class FloatValue : public Value
	{
	public:
		FloatValue(const long double& value);
		~FloatValue() override {};

		//! Will return a deeopopy of this object
		Value* Deepcopy() const override;

		//! Will return a string suitable for an std::ostream;
		std::string GetAsOsString() const override;

		//! Will return the raw value
		const long double& GetValue() const;

		operator long double() const;
		operator double() const;

		//! Will return the data as a long long int
		long long int GetInt64() const override;
		//! Will return the data as an int
		int GetInt32() const override;

		//! Will return the data as a long double
		long double GetFloat64() const override;
		//! Will return the data as a double
		double GetFloat32() const override;

		//! Will return the data as a string
		std::string GetString() const override;

		//! Throws HazelnuppValueNotConvertibleException
		const std::vector<Value*>& GetList() const override;

	private:
		long double value;
	};
}

/*** ../Hazelnupp/IntValue.h ***/


namespace Hazelnp
{
	/** Specializations for integer values (uses long long int)
	*/
	class IntValue : public Value
	{
	public:
		IntValue(const long long int& value);
		~IntValue() override {};

		//! Will return a deeopopy of this object
		Value* Deepcopy() const override;

		//! Will return a string suitable for an std::ostream;
		std::string GetAsOsString() const override;

		//! Will return the raw value
		const long long int& GetValue() const;

		operator long long int() const;
		operator int() const;


		//! Will return the data as a long long int
		long long int GetInt64() const override;
		//! Will return the data as an int
		int GetInt32() const override;

		//! Will return the data as a long double
		long double GetFloat64() const override;
		//! Will return the data as a double
		double GetFloat32() const override;

		//! Will return the data as a string
		std::string GetString() const override;

		//! Throws HazelnuppValueNotConvertibleException
		const std::vector<Value*>& GetList() const override;

	private:
		long long int value;
	};
}

/*** ../Hazelnupp/VoidValue.h ***/


namespace Hazelnp
{
	/** Specializations for void values. These house no value whatsoever, but only communicate information by merely existing.
	*/
	class VoidValue : public Value
	{
	public:
		VoidValue();
		~VoidValue() override {};

		//! Will return a deeopopy of this object
		Value* Deepcopy() const override;

		//! Will return a string suitable for an std::ostream;
		std::string GetAsOsString() const override;

		//! Throws HazelnuppValueNotConvertibleException
		long long int GetInt64() const override;
		//! Throws HazelnuppValueNotConvertibleException
		int GetInt32() const override;

		//! Throws HazelnuppValueNotConvertibleException
		long double GetFloat64() const override;
		//! Throws HazelnuppValueNotConvertibleException
		double GetFloat32() const override;

		//! Returns an empty string
		std::string GetString() const override;

		//! Returns an empty list
		const std::vector<Value*>& GetList() const;
	};
}

/*** ../Hazelnupp/StringValue.h ***/

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

/*** ../Hazelnupp/Parameter.h ***/

#include <string>
#include <ostream>

namespace Hazelnp
{
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
		Hazelnp::Value* value;
	};
}

/*** ../Hazelnupp/CmdArgsInterface.h ***/

#include <unordered_map>
#include <vector>


namespace Hazelnp
{
	/** The main class to interface with
	*/
	class CmdArgsInterface
	{
	public:
		CmdArgsInterface();
		CmdArgsInterface(const int argc, const char* const* argv);

		~CmdArgsInterface();

		//! Will parse command line arguments
		void Parse(const int argc, const char* const* argv);

		//! Will return argv[0], the name of the executable.
		const std::string& GetExecutableName() const;

		//! Will return the value given a key
		const Value& operator[](const std::string& key) const;

		//! Will check wether a parameter exists given a key, or not
		bool HasParam(const std::string& key) const;

		// Abbreviations
		//! Will register an abbreviation (like -f for --force)
		void RegisterAbbreviation(const std::string& abbrev, const std::string& target);

		//! Will return the long form of an abbreviation (like --force for -f)  
		//! Returns "" if no match is found
		const std::string& GetAbbreviation(const std::string& abbrev) const;

		//! Will check wether or not an abbreviation is registered
		bool HasAbbreviation(const std::string& abbrev) const;

		//! Will delete the abbreviation for a given parameter.  
		//! IMPORTANT: This parameter is the abbreviation! Not the long form!
		void ClearAbbreviation(const std::string& abbrevation);

		//! Will delete all abbreviations
		void ClearAbbreviations();

		//! Will register a constraint for a parameter.
		//! IMPORTANT: Any parameter can only have ONE constraint. Applying a new one will overwrite the old one!
		//! Construct the ParamConstraint struct yourself to combine Require, TypeSafety and Incompatibilities! You can also use the ParamConstraint constructor!
		void RegisterConstraint(const std::string& key, const ParamConstraint& constraint);

		//! Will return the constraint information for a specific parameter
		ParamConstraint GetConstraint(const std::string& parameter) const;

		//! Will the constraint of a specific parameter
		void ClearConstraint(const std::string& parameter);

		//! Will delete all constraints
		void ClearConstraints();

		//! Sets whether to crash the application, and print to stderr, when an exception is 
		//! raised whilst parsing, or not.
		void SetCrashOnFail(bool crashOnFail);

		//! Gets whether the application crashes on an exception whilst parsing, and prints to stderr.
		bool GetCrashOnFail() const;

		//! Sets whether the CmdArgsInterface should automatically catch the --help parameter, print the parameter documentation to stdout, and exit or not.
		void SetCatchHelp(bool catchHelp);

		//! Retruns whether the CmdArgsInterface should automatically catch the --help parameter, print the parameter documentation to stdout, and exit or not.
		bool GetCatchHelp() const;

		//! Sets a brief description of the application to be automatically added to the documentation.
		void SetBriefDescription(const std::string& description);

		//! Returns the brief description of the application to be automatically added to the documentation.
		const std::string& GetBriefDescription();

		//! Willl register a short description for a parameter.  
		//! Will overwrite existing descriptions for that parameter.
		void RegisterDescription(const std::string& parameter, const std::string& description);

		//! Will return a short description for a parameter, if it exists.  
		//! Empty string if it does not exist.
		const std::string& GetDescription(const std::string& parameter) const;

		//! Returns whether or not a given parameter has a registered description
		bool HasDescription(const std::string& parameter) const;

		//! Will delete the description of a parameter if it exists.
		void ClearDescription(const std::string& parameter);

		//! Will delete all parameter descriptions
		void ClearDescriptions();

		//! Will generate a text-based documentation suited to show the user, for example on --help.
		std::string GenerateDocumentation() const;

	private:
		//! Will translate the c-like args to an std::vector
		void PopulateRawArgs(const int argc, const char* const* argv);

		//! Will replace all args matching an abbreviation with their long form (like -f for --force)
		void ExpandAbbreviations();

		//! Will parse the next parameter. Returns the index of the next parameter.
		std::size_t ParseNextParameter(const std::size_t parIndex, Parameter*& out_Par);

		//! Will convert a vector of string-values to an actual Value
		Value* ParseValue(const std::vector<std::string>& values, const ParamConstraint* constraint = nullptr);

		//! Will apply the loaded constraints on the loaded values, exluding types.
		void ApplyConstraints();

		//! Will return a pointer to a paramConstraint given a key. If there is no, it returns nullptr
		const ParamConstraint* GetConstraintForKey(const std::string& key) const;

		std::string executableName; //! The path of the executable. Always argv[0]
		std::unordered_map<std::string, Parameter*> parameters;

		//! These are abbreviations. Like, -f for --force.
		std::unordered_map<std::string, std::string> parameterAbreviations;

		//! Parameter constraints, mapped to keys
		std::unordered_map<std::string, ParamConstraint> parameterConstraints;

		//! Raw argv
		std::vector<std::string> rawArgs;

		//! Short descriptions for parameters
		//! First member is the abbreviation
		std::unordered_map<std::string, std::string> parameterDescriptions;

		//! A brief description of the application to be added to the generated documentation. Optional.
		std::string briefDescription;

		//! If set to true, CmdArgsInterface will automatically catch the --help parameter, print the parameter documentation to stdout and exit.
		bool catchHelp = true;

		//! If set to true, CmdArgsInterface will crash the application with output to stderr when an exception is thrown whilst parsing.
		bool crashOnFail = true;
	};
}
