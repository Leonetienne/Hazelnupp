#include "Hazelnupp.h"
#include "VoidValue.h"
#include "IntValue.h"
#include "FloatValue.h"
#include "StringValue.h"
#include "ListValue.h"
#include "HazelnuppException.h"
#include "StringTools.h"
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
	catch (const HazelnuppConstraintTypeMissmatch& hctm)
	{
		if (crashOnFail)
		{
			std::cout << GenerateDocumentation() << std::endl;
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
			std::cout << GenerateDocumentation() << std::endl;
			std::cerr << "Fatal error: Missing required command-line parameter \"" << hctm.What() << "\"!";
			quick_exit(-1010);
		}
		else
			throw hctm; // yeet
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

const std::string Hazelnp::Hazelnupp::GetDescription(const std::string& parameter) const
{
	// Do we already have a description for this parameter?
	const auto par = parameterDescriptions.find(parameter);
	if (par == parameterDescriptions.end())
		// No? Then return ""
		return "";

	// We do? Then return it
	return par->second;
}

void Hazelnp::Hazelnupp::ClearDescription(const std::string& parameter)
{
	// This will just do nothing if the entry does not exist
	parameterDescriptions.erase(parameter);
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
	for (const auto& it : abbreviations)
	{
		// Do we already have that param in the paramInfo set?
		if (paramInfos.find(it.second) == paramInfos.end())
			// No? Create it.
			paramInfos[it.second] = ParamDocEntry();

		paramInfos[it.second].abbreviation = it.first;
	}

	// Collect constraints
	for (const auto& it : constraints)
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

			ss << std::endl << std::endl;
		}
	}

	return ss.str();
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
