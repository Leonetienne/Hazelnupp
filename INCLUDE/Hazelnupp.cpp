#include "Hazelnupp.h"

/*** ../Hazelnupp/FloatValue.cpp ***/

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


/*** ../Hazelnupp/Hazelnupp.cpp ***/

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
		// Dont apply constraints when we are just printind the param docs
		if ((!catchHelp) || (!HasParam("--help")))
			ApplyConstraints();
	}
	catch (const HazelnuppConstraintTypeMissmatch& exc)
	{
		if (crashOnFail)
		{
			std::cout << GenerateDocumentation() << std::endl << std::endl;
			std::cerr << "Parameter error: " << exc.What() << std::endl;
			quick_exit(-1009);
		}
		else
			throw exc; // yeet
	}
	catch (const HazelnuppConstraintMissingValue& exc)
	{
		if (crashOnFail)
		{
			std::cout << GenerateDocumentation() << std::endl << std::endl;
			std::cerr << "Parameter error: " << exc.What() << std::endl;
			quick_exit(-1010);
		}
		else
			throw exc; // yeet
	}

	// Catch --help parameter
	if ((catchHelp) && (HasParam("--help")))
	{
		std::cout << GenerateDocumentation() << std::endl;
		quick_exit(0);
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
	if (parameterAbreviations.size() == 0)
		return;

	for (std::string& arg : rawArgs)
	{
		// Is arg registered as an abbreviation?
		auto abbr = parameterAbreviations.find(arg);
		if (abbr != parameterAbreviations.end())
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
	// This is the raw (unconverted) data type the user provided
	DATA_TYPE rawInputType;

	// Constraint values
	const bool constrainType = (constraint != nullptr) && (constraint->constrainType);

	// Void-type
	if (values.size() == 0)
	{
		rawInputType = DATA_TYPE::VOID;

		// Is a list forced via a constraint? If yes, return an empty list
		if ((constrainType) &&
			(constraint->wantedType == DATA_TYPE::LIST))
			return new ListValue();

		// Is a string forced via a constraint? If yes, return an empty string
		else if ((constrainType) &&
			(constraint->wantedType == DATA_TYPE::STRING))
			return new StringValue("");

		// Is an int or float forced via constraint? If yes, throw an exception
		else if ((constrainType) &&
			((constraint->wantedType == DATA_TYPE::INT) ||
			 (constraint->wantedType == DATA_TYPE::FLOAT)))
			throw HazelnuppConstraintTypeMissmatch(
				constraint->key,
				constraint->wantedType,
				rawInputType,
				GetDescription(constraint->key)
			);

		// Else, just return the void type
		return new VoidValue;
	}

	// Force void type by constraint
	else if ((constrainType) &&
		(constraint->wantedType == DATA_TYPE::VOID))
	{
		return new VoidValue;
	}

	// List-type
	else if (values.size() > 1)
	{
		rawInputType = DATA_TYPE::LIST;

		// Should the type be something other than list?
		if ((constrainType) &&
			(constraint->wantedType != DATA_TYPE::LIST))
		{
			throw HazelnuppConstraintTypeMissmatch(
				constraint->key, 
				constraint->wantedType,
				rawInputType,
				GetDescription(constraint->key)
			);
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
		rawInputType = DATA_TYPE::STRING;

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
			// Else it is not possible to convert to a numeric
			else
				throw HazelnuppConstraintTypeMissmatch(
					constraint->key,
					constraint->wantedType,
					rawInputType,
					GetDescription(constraint->key)
				);
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
		rawInputType = isInt ? DATA_TYPE::INT : DATA_TYPE::FLOAT;

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

void Hazelnupp::SetCatchHelp(bool catchHelp)
{
	this->catchHelp = catchHelp;
	return;
}

bool Hazelnupp::GetCatchHelp() const
{
	return catchHelp;
}

void Hazelnupp::SetBriefDescription(const std::string& description)
{
	briefDescription = description;
	return;
}

const std::string& Hazelnupp::GetBriefDescription()
{
	return briefDescription;
}

void Hazelnp::Hazelnupp::RegisterDescription(const std::string& parameter, const std::string& description)
{
	parameterDescriptions[parameter] = description;
	return;
}

const std::string& Hazelnp::Hazelnupp::GetDescription(const std::string& parameter) const
{
	// Do we already have a description for this parameter?
	if (!HasDescription(parameter))
		// No? Then return ""
		return Placeholders::g_emptyString;

	// We do? Then return it
	return parameterDescriptions.find(parameter)->second;
}

bool Hazelnupp::HasDescription(const std::string& parameter) const
{
	return parameterDescriptions.find(parameter) != parameterDescriptions.end();
}

void Hazelnupp::ClearDescription(const std::string& parameter)
{
	// This will just do nothing if the entry does not exist
	parameterDescriptions.erase(parameter);
	return;
}

void Hazelnp::Hazelnupp::ClearDescriptions()
{
	parameterDescriptions.clear();
	return;
}

std::string Hazelnupp::GenerateDocumentation() const
{
	std::stringstream ss;

	// Add brief, if available
	if (briefDescription.length() > 0)
		ss << briefDescription << std::endl;

	// Collect parameter information
	struct ParamDocEntry
	{
		std::string abbreviation;
		std::string description;
		std::string type;
		bool required = false;
		bool typeIsForced = false;
		std::string defaultVal;
	};
	std::unordered_map<std::string, ParamDocEntry> paramInfos;

	// Collect descriptions
	for (const auto& it : parameterDescriptions)
	{
		// Do we already have that param in the paramInfo set?
		if (paramInfos.find(it.first) == paramInfos.end())
			// No? Create it.
			paramInfos[it.first] = ParamDocEntry();

		paramInfos[it.first].description = it.second;
	}

	// Collect abbreviations
	// first value is abbreviation, second is long form
	for (const auto& it : parameterAbreviations)
	{
		// Do we already have that param in the paramInfo set?
		if (paramInfos.find(it.second) == paramInfos.end())
			// No? Create it.
			paramInfos[it.second] = ParamDocEntry();

		paramInfos[it.second].abbreviation = it.first;
	}

	// Collect constraints
	for (const auto& it : parameterConstraints)
	{
		// Do we already have that param in the paramInfo set?
		if (paramInfos.find(it.first) == paramInfos.end())
			// No? Create it.
			paramInfos[it.first] = ParamDocEntry();

		ParamDocEntry& cached = paramInfos[it.first];
		cached.required = it.second.required;
		cached.typeIsForced = it.second.constrainType;
		cached.type = DataTypeToString(it.second.wantedType);
		
		std::stringstream defaultValueSs;
		for (const std::string& s : it.second.defaultValue)
		{
			defaultValueSs << '\'' << s << '\'';

			// Add a space if we are not at the last entry
			if ((void*)&s != (void*)&it.second.defaultValue.back())
				defaultValueSs << " ";
		}
		cached.defaultVal = defaultValueSs.str();
	}

	// Now generate the documentatino body
	if (paramInfos.size() > 0)
	{
		ss << std::endl 
			<< "==== AVAILABLE PARAMETERS ====" 
			<< std::endl << std::endl;

		std::size_t counter = 0;
		for (const auto& it : paramInfos)
		{
			const ParamDocEntry& pde = it.second;

			// Put name
			ss << it.first << "   ";

			// Put abbreviation
			if (pde.abbreviation.length() > 0)
				ss << pde.abbreviation << "   ";

			// Put type
			if (pde.typeIsForced)
				ss << pde.type << "   ";

			// Put default value
			if (pde.defaultVal.length() > 0)
				ss << "default=[" << pde.defaultVal << "]   ";

			// Put required tag, but only if no default value
			if ((pde.required) && (pde.defaultVal.length() == 0))
				ss << "[[REQUIRED]]    ";

			// Put brief description
			if (pde.description.length() > 0)
				ss << pde.description;

			// Omit linebreaks when we're on the last element
			if (counter < paramInfos.size()-1)
				ss << std::endl << std::endl;

			counter++;
		}
	}

	return ss.str();
}

void Hazelnupp::ApplyConstraints()
{
	// Enforce required parameters / default values
	for (const auto& pc : parameterConstraints)
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
					throw HazelnuppConstraintMissingValue(
						pc.second.key,
						GetDescription(pc.second.key)
				);
			}
		}

	return;
}

ParamConstraint Hazelnupp::GetConstraint(const std::string& parameter) const
{
	return parameterConstraints.find(parameter)->second;
}

void Hazelnupp::ClearConstraint(const std::string& parameter)
{
	parameterConstraints.erase(parameter);
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
	parameterAbreviations.insert(std::pair<std::string, std::string>(abbrev, target));
	return;
}

const std::string& Hazelnupp::GetAbbreviation(const std::string& abbrev) const
{
	if (!HasAbbreviation(abbrev))
		return Placeholders::g_emptyString;

	return parameterAbreviations.find(abbrev)->second;
}

bool Hazelnupp::HasAbbreviation(const std::string& abbrev) const
{
	return parameterAbreviations.find(abbrev) != parameterAbreviations.end();
}

void Hazelnupp::ClearAbbreviation(const std::string& abbrevation)
{
	parameterAbreviations.erase(abbrevation);
	return;
}

void Hazelnupp::ClearAbbreviations()
{
	parameterAbreviations.clear();
	return;
}

void Hazelnupp::RegisterConstraint(const std::string& key, const ParamConstraint& constraint)
{
	// Magic syntax, wooo
	(parameterConstraints[key] = constraint).key = key;
	return;
}

void Hazelnupp::ClearConstraints()
{
	parameterConstraints.clear();
	return;
}

void Hazelnupp::SetCrashOnFail(bool crashOnFail)
{
	this->crashOnFail = crashOnFail;
	return;
}

const ParamConstraint* Hazelnupp::GetConstraintForKey(const std::string& key) const
{
	const auto constraint = parameterConstraints.find(key);

	if (constraint == parameterConstraints.end())
		return nullptr;

	return &constraint->second;
}


/*** ../Hazelnupp/IntValue.cpp ***/

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


/*** ../Hazelnupp/ListValue.cpp ***/

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


/*** ../Hazelnupp/Parameter.cpp ***/


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


/*** ../Hazelnupp/StringTools.cpp ***/


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


/*** ../Hazelnupp/StringValue.cpp ***/

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


/*** ../Hazelnupp/Value.cpp ***/


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


/*** ../Hazelnupp/VoidValue.cpp ***/


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
	return "";
}

const std::vector<Value*>& VoidValue::GetList() const
{
	static const std::vector<Value*> empty;
	return empty;
}

