#include "Hazelnupp.h"

/*** FloatValue.cpp ***/

#include <sstream>

using namespace Hazelnp;

FloatValue::FloatValue(const long double& value)
	:
	Value(DATA_TYPE::FLOAT),
	value { value }
{
	return;
}

Value* FloatValue::Deepcopy() const
{
	return new FloatValue(value);
}

std::string FloatValue::GetAsOsString() const
{
	std::stringstream ss;
	ss << "FloatValue: " << value;
	return ss.str();
}

const long double& FloatValue::GetValue() const
{
	return value;
}

FloatValue::operator long double() const
{
	return value;
}

FloatValue::operator double() const
{
	return (double)value;
}



long long int FloatValue::GetInt64() const
{
	return (long long int)value;
}

int FloatValue::GetInt32() const
{
	return (int)value;
}

long double FloatValue::GetFloat64() const
{
	return value;
}

double FloatValue::GetFloat32() const
{
	return (double)value;
}

std::string FloatValue::GetString() const
{
	std::stringstream ss;
	ss << value;

	return ss.str();
}

const std::vector<Value*>& FloatValue::GetList() const
{
	throw HazelnuppValueNotConvertibleException();
}


/*** Hazelnupp.cpp ***/

#include <iostream>
#include <cstdlib>

using namespace Hazelnp;

Hazelnupp::Hazelnupp()
{
	return;
}

Hazelnupp::Hazelnupp(const int argc, const char* const* argv)
{
	Parse(argc, argv);
	return;
}

Hazelnupp::~Hazelnupp()
{
	for (auto& it : parameters)
		delete it.second;

	parameters.clear();

	return;
}

void Hazelnupp::Parse(const int argc, const char* const* argv)
{
	try
	{
		// Populate raw arguments
		PopulateRawArgs(argc, argv);

		// Expand abbreviations
		ExpandAbbreviations();

		executableName = std::string(rawArgs[0]);

		std::size_t i = 1;
		while (i < rawArgs.size())
		{
			if ((rawArgs[i].length() > 2) && (rawArgs[i].substr(0, 2) == "--"))
			{
				Parameter* param = nullptr;
				i = ParseNextParameter(i, param);

				parameters.insert(std::pair<std::string, Parameter*>(param->Key(), param));
			}
			else
				i++;
		}

		// Apply constraints such as default values, and required parameters.
		// Types have already been enforced.
		ApplyConstraints();
	}
	catch (const HazelnuppConstraintTypeMissmatch& hctm)
	{
		if (crashOnFail)
		{
			std::cerr << "Fatal error: Command-line parameter value-type mismatch at \"" << hctm.What() << "\"!";
			quick_exit(-1009);
		}
		else
			throw hctm; // yeet
	}
	catch (const HazelnuppConstraintMissingValue& hctm)
	{
		if (crashOnFail)
		{
			std::cerr << "Fatal error: Missing required command-line parameter \"" << hctm.What() << "\"!";
			quick_exit(-1010);
		}
		else
			throw hctm; // yeet
	}

	return;
}

std::size_t Hazelnupp::ParseNextParameter(const std::size_t parIndex, Parameter*& out_Par)
{
	std::size_t i = parIndex;
	const std::string key = rawArgs[parIndex];
	std::vector<std::string> values;

	// Get values
	for (i++; i < rawArgs.size(); i++)
		// If not another parameter
		if ((rawArgs[i].length() < 2) || (rawArgs[i].substr(0, 2) != "--"))
			values.emplace_back(rawArgs[i]);
		else
		{
			break;
		}

	// Fetch constraint info
	const ParamConstraint* pcn = GetConstraintForKey(key);

	Value* parsedVal = ParseValue(values, pcn);
	if (parsedVal != nullptr)
	{
		out_Par = new Parameter(key, parsedVal);

		delete parsedVal;
		parsedVal = nullptr;
	}
	else
		throw std::runtime_error("Unable to parse parameter!");

	return i;
}

void Hazelnupp::PopulateRawArgs(const int argc, const char* const* argv)
{
	rawArgs.clear();
	rawArgs.reserve(argc);
	
	for (int i = 0; i < argc; i++)
		rawArgs.emplace_back(std::string(argv[i]));

	return;
}

void Hazelnupp::ExpandAbbreviations()
{
	// Abort if no abbreviations
	if (abbreviations.size() == 0)
		return;

	for (std::string& arg : rawArgs)
	{
		// Is arg registered as an abbreviation?
		auto abbr = abbreviations.find(arg);
		if (abbr != abbreviations.end())
		{
			// Yes: replace arg with the long form
			arg = abbr->second;
		}
	}

	return;
}

bool Hazelnupp::HasParam(const std::string& key) const
{
	return parameters.find(key) != parameters.end();
}

Value* Hazelnupp::ParseValue(const std::vector<std::string>& values, const ParamConstraint* constraint)
{
	// Constraint values
	const bool constrainType = (constraint != nullptr) && (constraint->constrainType);

	// Void-type
	if (values.size() == 0)
	{
		// Is a list forced via a constraint? If yes, return an empty list
		if ((constrainType) &&
			(constraint->wantedType == DATA_TYPE::LIST))
			return new ListValue();

		return new VoidValue;
	}

	// Force void type by constraint
	if ((constrainType) &&
		(constraint->wantedType == DATA_TYPE::VOID))
	{
		return new VoidValue;
	}

	// List-type
	else if (values.size() > 1)
	{
		// Should the type be something other than list?
		if ((constrainType) &&
			(constraint->wantedType != DATA_TYPE::LIST))
		{
			throw HazelnuppConstraintTypeMissmatch(values[0] + " " + values[1]);
		}

		ListValue* newList = new ListValue();
		for (const std::string& val : values)
		{
			Value* tmp = ParseValue({ val });
			newList->AddValue(tmp);
			delete tmp;
		}
		return newList;
	}

	// Now we're only dealing with a single value
	const std::string& val = values[0];
	
	// String
	if (!StringTools::IsNumeric(val, true))
	{
		// Is the type not supposed to be a string?
		// void and list are already sorted out
		if ((constrainType) &&
			(constraint->wantedType != DATA_TYPE::STRING))
		{
			// We can only force a list-value from here
			if (constraint->wantedType == DATA_TYPE::LIST)
			{
				ListValue* list = new ListValue();
				Value* tmp = ParseValue({ val });
				list->AddValue(tmp);
				delete tmp;
				tmp = nullptr;
				return list;
			}
			// Else it not possible to convert to a numeric
			else
				throw HazelnuppConstraintTypeMissmatch(val);
		}

		return new StringValue(val);
	}

	// In this case we have a numeric value.
	// We should still produce a string if requested
	if ((constrainType) &&
		(constraint->wantedType == DATA_TYPE::STRING))
		return new StringValue(val);

	// Numeric
	bool isInt;
	long double num;
	
	if (StringTools::ParseNumber(val, isInt, num))
	{
		// Is the type constrained?
		// (only int and float left)
		if (constrainType)
		{
			// Must it be an integer?
			if (constraint->wantedType == DATA_TYPE::INT)
				return new IntValue((long long int)num);
			// Must it be a floating point?
			else if (constraint->wantedType == DATA_TYPE::FLOAT)
				return new FloatValue(num);
			// Else it must be a List
			else
			{
				ListValue* list = new ListValue();
				Value* tmp = ParseValue({ val });
				list->AddValue(tmp);
				delete tmp;
				tmp = nullptr;
				return list;
			}
		}
		// Type is not constrained
		else
		{
			// Integer
			if (isInt)
				return new IntValue((long long int)num);

			// Double
			return new FloatValue(num);
		}
	}

	// Failed
	return nullptr;
}

bool Hazelnupp::GetCrashOnFail() const
{
	return crashOnFail;
}

void Hazelnupp::ApplyConstraints()
{
	// Enforce required parameters / default values
	for (const auto& pc : constraints)
		// Parameter in question is not supplied
		if (!HasParam(pc.second.key))
		{
			// Do we have a default value?
			if (pc.second.defaultValue.size() > 0)
			{
				// Then create it now, by its default value

				Value* tmp = ParseValue(pc.second.defaultValue, &pc.second);
				parameters.insert(std::pair<std::string, Parameter*>(
					pc.second.key,
					new Parameter(pc.second.key, tmp)
				));

				delete tmp;
				tmp = nullptr;
			}
			// So we do not have a default value...
			else
			{
				// Is it important to have the missing parameter?
				if (pc.second.required)
					// Throw an error message then
					throw HazelnuppConstraintMissingValue(pc.second.key);
			}
		}

	return;
}

const std::string& Hazelnupp::GetExecutableName() const
{
	return executableName;
}

const Value& Hazelnupp::operator[](const std::string& key) const
{
	// Throw exception if param is unknown
	if (!HasParam(key))
		throw HazelnuppInvalidKeyException();

	return *parameters.find(key)->second->GetValue();
}

void Hazelnupp::RegisterAbbreviation(const std::string& abbrev, const std::string& target)
{
	abbreviations.insert(std::pair<std::string, std::string>(abbrev, target));
	return;
}

const std::string& Hazelnupp::GetAbbreviation(const std::string& abbrev) const
{
	return abbreviations.find(abbrev)->second;
}

bool Hazelnupp::HasAbbreviation(const std::string& abbrev) const
{
	return abbreviations.find(abbrev) != abbreviations.end();
}

void Hazelnupp::ClearAbbreviations()
{
	abbreviations.clear();
	return;
}

void Hazelnupp::RegisterConstraints(const std::vector<ParamConstraint>& constraints)
{
	for (const ParamConstraint& pc : constraints)
	{
		// Does this constraint already exist?
		const auto constraint = this->constraints.find(pc.key);
		// If yes, replace it.
		if (constraint != this->constraints.end())
			constraint->second = pc;

		// Else, create a new pair
		else
			this->constraints.insert(std::pair<std::string, ParamConstraint>(
				pc.key,
				pc
			));
	}

	return;
}

void Hazelnupp::ClearConstraints()
{
	constraints.clear();
	return;
}

void Hazelnupp::SetCrashOnFail(bool crashOnFail)
{
	this->crashOnFail = crashOnFail;
	return;
}

const ParamConstraint* Hazelnupp::GetConstraintForKey(const std::string& key) const
{
	const auto constraint = constraints.find(key);

	if (constraint == constraints.end())
		return nullptr;

	return &constraint->second;
}


/*** IntValue.cpp ***/

#include <sstream>

using namespace Hazelnp;

IntValue::IntValue(const long long int& value)
	:
	Value(DATA_TYPE::INT),
	value { value }
{
	return;
}

Value* IntValue::Deepcopy() const
{
	return new IntValue(value);
}

std::string IntValue::GetAsOsString() const
{
	std::stringstream ss;
	ss << "IntValue: " << value;
	return ss.str();
}

const long long int& IntValue::GetValue() const
{
	return value;
}

IntValue::operator long long int() const
{
	return value;
}

IntValue::operator int() const
{
	return (int)value;
}



long long int IntValue::GetInt64() const
{
	return value;
}

int IntValue::GetInt32() const
{
	return (int)value;
}

long double IntValue::GetFloat64() const
{
	return (long double)value;
}

double IntValue::GetFloat32() const
{
	return (double)value;
}

std::string IntValue::GetString() const
{
	std::stringstream ss;
	ss << value;

	return ss.str();
}

const std::vector<Value*>& IntValue::GetList() const
{
	throw HazelnuppValueNotConvertibleException();
}


/*** ListValue.cpp ***/

#include <sstream>

using namespace Hazelnp;

ListValue::ListValue() :
	Value(DATA_TYPE::LIST)
{
	return;
}

ListValue::~ListValue()
{
	for (Value* val : value)
		delete val;

	value.clear();
	
	return;
}

Value* ListValue::Deepcopy() const
{
	ListValue* newList = new ListValue();

	for (const Value* val : value)
		newList->AddValue(val);

	return newList;
}

void ListValue::AddValue(const Value* value)
{
	this->value.emplace_back(value->Deepcopy());
	return;
}

const std::vector<Value*>& ListValue::GetValue() const
{
	return value;
}

std::string ListValue::GetAsOsString() const
{
	std::stringstream ss;

	ss << "ListValue: [";

	for (const Value* val : value)
	{
		ss << *val;
		if (val != value.back())
			ss << ", ";
	}

	ss << "]";

	return ss.str();
}

ListValue::operator std::vector<Value*>() const
{
	return value;
}



long long int ListValue::GetInt64() const
{
	throw HazelnuppValueNotConvertibleException();
}

int ListValue::GetInt32() const
{
	throw HazelnuppValueNotConvertibleException();
}

long double ListValue::GetFloat64() const
{
	throw HazelnuppValueNotConvertibleException();
}

double ListValue::GetFloat32() const
{
	throw HazelnuppValueNotConvertibleException();
}

std::string ListValue::GetString() const
{
	throw HazelnuppValueNotConvertibleException();
}

const std::vector<Value*>& ListValue::GetList() const
{
	return value;
}


/*** Parameter.cpp ***/


using namespace Hazelnp;

Parameter::Parameter(const std::string& key, const ::Value* value)
	:
	key{ key }
{
	this->value = value->Deepcopy();
	return;
}

Parameter::~Parameter()
{
	delete value;
	value = nullptr;

	return;
}

const std::string& Parameter::Key() const
{
	return key;
}

const ::Value* Parameter::GetValue() const
{
	return value;
}


/*** StringTools.cpp ***/


using namespace Hazelnp;

bool StringTools::Contains(const std::string& str, const char c)
{
    for (const char& i : str)
        if (i == c)
            return true;

    return false;
}

std::string StringTools::Replace(const std::string& str, const char find, const std::string& subst)
{
    std::stringstream ss;

    for (std::size_t i = 0; i < str.length(); i++)
    {
        if (str[i] != find) ss << str[i];
        else ss << subst;
    }

    return ss.str();
}

std::string StringTools::Replace(const std::string& str, const std::string& find, const std::string& subst)
{
    if (find.length() == 0) return str;

    std::stringstream ss;

    std::size_t posFound = 0;
    std::size_t lastFound = 0;

    while (posFound != std::string::npos)
    {
        lastFound = posFound;
        posFound = str.find(find, posFound);

        if (posFound != std::string::npos)
        {
            ss << str.substr(lastFound, posFound - lastFound) << subst;
            posFound += find.length();
        }
        else
        {
            ss << str.substr(lastFound, (str.length()) - lastFound);
        }
    }

    return ss.str();
}


bool StringTools::IsNumeric(const std::string& str, const bool allowDecimalPoint)
{
    if (str.length() == 0) return false;

    bool alreadyParsedDecimalPoint = false;
    std::size_t digitCount = 0;

    for (std::size_t i = 0; i < str.length(); i++)
    {
        if (!(
            ((str[i] >= '0') && (str[i] <= '9')) ||
            ((str[i] == '-') && (i == 0)) ||
            ((str[i] == '.') && (allowDecimalPoint) && (!alreadyParsedDecimalPoint) && (digitCount > 0))
            )) return false;


        // Here we just have to check for the character. Not for any other conditions.
        // Why? Because if these conditions failed, the function would have already returned false.
        if (((str[i] >= '0') && (str[i] <= '9'))) digitCount++;
        if (str[i] == '.') alreadyParsedDecimalPoint = true;
    }

    // Even if we did not find any invalid chars, we should still return false, if we found no digits at all.
    return digitCount > 0;
}

bool StringTools::ParseNumber(const std::string& str, bool& out_isInt, long double& out_number)
{
    bool isDecimal = false;

    if (str.length() == 0) return false;
    if (Contains(str, '.')) isDecimal = true;

    if (isDecimal)
    {
        try
        {
            out_number = std::stold(str);
            out_isInt = false;
        }
        catch (std::invalid_argument&)
        {
            return false;
        }
        catch (std::out_of_range&)
        {
            return false;
        }
    }
    else
    {
        try
        {
            out_number = (long double)std::stoll(str);
            out_isInt = true;
        }
        catch (std::invalid_argument&)
        {
            return false;
        }
        catch (std::out_of_range&)
        {
            return false;
        }
    }

    return true;
}

std::vector<std::string> StringTools::SplitString(const std::string& str, const char delimiter)
{
    if (str.length() == 0) return std::vector<std::string>();

    return SplitString(str, delimiter);
}

std::vector<std::string> StringTools::SplitString(const std::string& str, const std::string& delimiter)
{
    if (str.length() == 0) return std::vector<std::string>();

    std::vector<std::string> parts;

    if (delimiter.length() == 0) // If the delimiter is "" (empty), just split between every single char. Not useful, but logical
    {
        for (std::size_t i = 0; i < str.length(); i++)
        {
            parts.push_back(std::string({ str[i] }));
        }
        return parts;
    }

    std::size_t posFound = 0;
    std::size_t lastFound = 0;

    while (posFound != std::string::npos)
    {
        lastFound = posFound;
        posFound = str.find(delimiter, posFound);

        std::string found;

        if (posFound != std::string::npos)
        {
            found = str.substr(lastFound, posFound - lastFound);
            posFound += delimiter.length();
        }
        else
        {
            found = str.substr(lastFound, str.length() - lastFound);
        }

        parts.push_back(found);
    }

    return parts;
}

std::string StringTools::ToLower(const std::string& str)
{
    std::stringstream ss;
    for (std::size_t i = 0; i < str.length(); i++)
    {
        if ((str[i] >= 'A') && (str[i] <= 'Z')) ss << (char)(((int)str[i]) + 32);
        else if (str[i] == -60) ss << (char)-28; // AE => ae
        else if (str[i] == -42) ss << (char)-10; // OE => oe
        else if (str[i] == -36) ss << (char)-4;  // UE => ue
        else ss << str[i];
    }

    return ss.str();
}


/*** StringValue.cpp ***/

#include <sstream>

using namespace Hazelnp;

StringValue::StringValue(const std::string& value)
	:
	Value(DATA_TYPE::STRING),
	value { value }
{
	return;
}

Value* StringValue::Deepcopy() const
{
	return new StringValue(value);
}

std::string StringValue::GetAsOsString() const
{
	std::stringstream ss;
	ss << "StringValue: " << value;
	return ss.str();
}

const std::string& StringValue::GetValue() const
{
	return value;
}

StringValue::operator std::string() const
{
	return value;
}



long long int StringValue::GetInt64() const
{
	throw HazelnuppValueNotConvertibleException();
}

int StringValue::GetInt32() const
{
	throw HazelnuppValueNotConvertibleException();
}

long double StringValue::GetFloat64() const
{
	throw HazelnuppValueNotConvertibleException();
}

double StringValue::GetFloat32() const
{
	throw HazelnuppValueNotConvertibleException();
}

std::string StringValue::GetString() const
{
	return value;
}

const std::vector<Value*>& StringValue::GetList() const
{
	throw HazelnuppValueNotConvertibleException();
}


/*** Value.cpp ***/


using namespace Hazelnp;

Value::Value(DATA_TYPE type)
	:
	type{ type }
{
	return;
}

DATA_TYPE Value::GetDataType() const
{
	return type;
}


/*** VoidValue.cpp ***/


using namespace Hazelnp;

VoidValue::VoidValue()
	:
	Value(DATA_TYPE::VOID)
{
	return;
}

Value* VoidValue::Deepcopy() const
{
	return new VoidValue();
}

std::string VoidValue::GetAsOsString() const
{
	return "VoidValue";
}



long long int VoidValue::GetInt64() const
{
	throw HazelnuppValueNotConvertibleException();
}

int VoidValue::GetInt32() const
{
	throw HazelnuppValueNotConvertibleException();
}

long double VoidValue::GetFloat64() const
{
	throw HazelnuppValueNotConvertibleException();
}

double VoidValue::GetFloat32() const
{
	throw HazelnuppValueNotConvertibleException();
}

std::string VoidValue::GetString() const
{
	throw HazelnuppValueNotConvertibleException();
}

const std::vector<Value*>& VoidValue::GetList() const
{
	throw HazelnuppValueNotConvertibleException();
}

