#pragma once

/*** StringTools.h ***/

#include <string>
#include <sstream>
#include <vector>
#include <cmath>

namespace Hazelnp
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

/*** HazelnuppException.h ***/

#include <stdexcept>

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
	};

	/** Gets thrown when a parameter constrained to be required is not provided, and has no default value set
	*/
	class HazelnuppConstraintMissingValue : public HazelnuppConstraintException
	{
	public:
		HazelnuppConstraintMissingValue() : HazelnuppConstraintException() {};
		HazelnuppConstraintMissingValue(const std::string& msg) : HazelnuppConstraintException(msg) {};
	};
}

/*** DataType.h ***/


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
}

/*** ParamConstraint.h ***/

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
		static ParamConstraint Require(const std::string& key, const std::vector<std::string>& defaultValue = {}, bool required = true)
		{
			ParamConstraint pc;
			pc.key = key;
			pc.defaultValue = defaultValue;
			pc.required = required;

			return pc;
		}

		//! Constructs a type-safety constraint
		static ParamConstraint TypeSafety(const std::string& key, DATA_TYPE wantedType, bool constrainType = true)
		{
			ParamConstraint pc;
			pc.key = key;
			pc.constrainType = constrainType;
			pc.wantedType = wantedType;

			return pc;
		}

		//! Whole constructor
		ParamConstraint(const std::string& key, bool constrainType, DATA_TYPE wantedType, const std::vector<std::string>& defaultValue, bool required)
			:
			key{ key },
			constrainType{ constrainType },
			wantedType{ wantedType },
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
		//! Think of this like a list of parameters. Like {"--width", "800"}
		std::vector<std::string> defaultValue;

		//! If set to true, and no default value set,
		//! an error will be produced if this parameter is not supplied by the user.
		bool required = false;
	};
}

/*** Value.h ***/

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

/*** ListValue.h ***/

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

/*** VoidValue.h ***/


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

		//! Throws HazelnuppValueNotConvertibleException
		std::string GetString() const override;

		//! Throws HazelnuppValueNotConvertibleException
		const std::vector<Value*>& GetList() const;
	};
}

/*** Parameter.h ***/

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

/*** StringValue.h ***/

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

/*** IntValue.h ***/


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

/*** Hazelnupp.h ***/

#include <unordered_map>
#include <vector>

namespace Hazelnp
{
	/** The main class to interface with
	*/
	class Hazelnupp
	{
	public:
		Hazelnupp();
		Hazelnupp(const int argc, const char* const* argv);

		~Hazelnupp();

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
		const std::string& GetAbbreviation(const std::string& abbrev) const;

		//! Will check wether or not an abbreviation is registered
		bool HasAbbreviation(const std::string& abbrev) const;

		//! Will delete all abbreviations
		void ClearAbbreviations();

		//! Will register parameter constraints
		void RegisterConstraints(const std::vector<ParamConstraint>& constraints);

		//! Will delete all constraints
		void ClearConstraints();

		//! Sets whether to crash the application, and print to stderr, when an exception is 
		//! raised whilst parsing, or not.
		void SetCrashOnFail(bool crashOnFail);

		//! Gets whether the application crashes on an exception whilst parsing, and prints to stderr.
		bool GetCrashOnFail() const;

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

		// These are abbreviations. Like, -f for --force.
		std::unordered_map<std::string, std::string> abbreviations;

		// Parameter constraints, mapped to keys
		std::unordered_map<std::string, ParamConstraint> constraints;

		std::vector<std::string> rawArgs;

		//! If set to true, Hazelnupp will crash the application with output to stderr when an exception is thrown whilst parsing.
		bool crashOnFail = true;
	};
}

/*** FloatValue.h ***/

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
