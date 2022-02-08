#include "CmdArgsInterface.h"
#include "VoidValue.h"
#include "IntValue.h"
#include "FloatValue.h"
#include "StringValue.h"
#include "ListValue.h"
#include "HazelnuppException.h"
#include "Placeholders.h"
#include "StringTools.h"
#include <iostream>
#include <cstdlib>

using namespace Hazelnp;

CmdArgsInterface::CmdArgsInterface()
{
	return;
}

CmdArgsInterface::CmdArgsInterface(const int argc, const char* const* argv)
{
	Parse(argc, argv);
	return;
}

CmdArgsInterface::~CmdArgsInterface()
{
	for (auto& it : parameters)
		delete it.second;

	parameters.clear();

	return;
}

void CmdArgsInterface::Parse(const int argc, const char* const* argv)
{
	try
	{
		// Populate raw arguments
		PopulateRawArgs(argc, argv);

		// Expand abbreviations
		ExpandAbbreviations();

		executableName = std::string(rawArgs[0]);

		// Read and parse all parameters
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
	catch (const HazelnuppConstraintIncompatibleParameters& exc)
	{
		if (crashOnFail)
		{
			std::cout << GenerateDocumentation() << std::endl << std::endl;
			std::cerr << "Parameter error: " << exc.What() << std::endl;
			exit(-1000);
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
			exit(-1001);
		}
		else
			throw exc; // yeet
	}
	catch (const HazelnuppConstraintTypeMissmatch& exc)
	{
		if (crashOnFail)
		{
			std::cout << GenerateDocumentation() << std::endl << std::endl;
			std::cerr << "Parameter error: " << exc.What() << std::endl;
			exit(-1002);
		}
		else
			throw exc; // yeet
	}
	catch (const HazelnuppConstraintException& exc)
	{
		if (crashOnFail)
		{
			std::cout << GenerateDocumentation() << std::endl << std::endl;
			std::cerr << "Parameter error: " << exc.What() << std::endl;
			exit(-1003);
		}
		else
			throw exc; // yeet
	}
	catch (const HazelnuppException& exc)
	{
		if (crashOnFail)
		{
			std::cout << GenerateDocumentation() << std::endl << std::endl;
			std::cerr << "Parameter error: " << exc.What() << std::endl;
			exit(-1004);
		}
		else
			throw exc; // yeet
	}

	// Catch --help parameter
	if ((catchHelp) && (HasParam("--help")))
	{
		std::cout << GenerateDocumentation() << std::endl;
		exit(0);
	}

	return;
}

std::size_t CmdArgsInterface::ParseNextParameter(const std::size_t parIndex, Parameter*& out_Par)
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

void CmdArgsInterface::PopulateRawArgs(const int argc, const char* const* argv)
{
	rawArgs.clear();
	rawArgs.reserve(argc);
	
	for (int i = 0; i < argc; i++)
		rawArgs.emplace_back(std::string(argv[i]));

	return;
}

void CmdArgsInterface::ExpandAbbreviations()
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

bool CmdArgsInterface::HasParam(const std::string& key) const
{
	return parameters.find(key) != parameters.end();
}

Value* CmdArgsInterface::ParseValue(const std::vector<std::string>& values, const ParamConstraint* constraint)
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
			(constraint->requiredType == DATA_TYPE::LIST))
			return new ListValue();

		// Is a string forced via a constraint? If yes, return an empty string
		else if ((constrainType) &&
			(constraint->requiredType == DATA_TYPE::STRING))
			return new StringValue("");

		// Is an int or float forced via constraint? If yes, throw an exception
		else if ((constrainType) &&
			((constraint->requiredType == DATA_TYPE::INT) ||
			 (constraint->requiredType == DATA_TYPE::FLOAT)))
			throw HazelnuppConstraintTypeMissmatch(
				constraint->key,
				constraint->requiredType,
				rawInputType,
				GetDescription(constraint->key)
			);

		// Else, just return the void type
		return new VoidValue;
	}

	// Force void type by constraint
	else if ((constrainType) &&
		(constraint->requiredType == DATA_TYPE::VOID))
	{
		return new VoidValue;
	}

	// List-type
	else if (values.size() > 1)
	{
		rawInputType = DATA_TYPE::LIST;

		// Should the type be something other than list?
		if ((constrainType) &&
			(constraint->requiredType != DATA_TYPE::LIST))
		{
			throw HazelnuppConstraintTypeMissmatch(
				constraint->key, 
				constraint->requiredType,
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
	if (!Internal::StringTools::IsNumeric(val, true))
	{
		rawInputType = DATA_TYPE::STRING;

		// Is the type not supposed to be a string?
		// void and list are already sorted out
		if ((constrainType) &&
			(constraint->requiredType != DATA_TYPE::STRING))
		{
			// We can only force a list-value from here
			if (constraint->requiredType == DATA_TYPE::LIST)
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
					constraint->requiredType,
					rawInputType,
					GetDescription(constraint->key)
				);
		}

		return new StringValue(val);
	}

	// In this case we have a numeric value.
	// We should still produce a string if requested
	if ((constrainType) &&
		(constraint->requiredType == DATA_TYPE::STRING))
		return new StringValue(val);

	// Numeric
	bool isInt;
	long double num;
	
	if (Internal::StringTools::ParseNumber(val, isInt, num))
	{
		rawInputType = isInt ? DATA_TYPE::INT : DATA_TYPE::FLOAT;

		// Is the type constrained?
		// (only int and float left)
		if (constrainType)
		{
			// Must it be an integer?
			if (constraint->requiredType == DATA_TYPE::INT)
				return new IntValue((long long int)num);
			// Must it be a floating point?
			else if (constraint->requiredType == DATA_TYPE::FLOAT)
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

bool CmdArgsInterface::GetCrashOnFail() const
{
	return crashOnFail;
}

void CmdArgsInterface::SetCatchHelp(bool catchHelp)
{
	this->catchHelp = catchHelp;
	return;
}

bool CmdArgsInterface::GetCatchHelp() const
{
	return catchHelp;
}

void CmdArgsInterface::SetBriefDescription(const std::string& description)
{
	briefDescription = description;
	return;
}

const std::string& CmdArgsInterface::GetBriefDescription()
{
	return briefDescription;
}

void Hazelnp::CmdArgsInterface::RegisterDescription(const std::string& parameter, const std::string& description)
{
	parameterDescriptions[parameter] = description;
	return;
}

const std::string& Hazelnp::CmdArgsInterface::GetDescription(const std::string& parameter) const
{
	// Do we already have a description for this parameter?
	if (!HasDescription(parameter))
		// No? Then return ""
		return Placeholders::g_emptyString;

	// We do? Then return it
	return parameterDescriptions.find(parameter)->second;
}

bool CmdArgsInterface::HasDescription(const std::string& parameter) const
{
	return parameterDescriptions.find(parameter) != parameterDescriptions.end();
}

void CmdArgsInterface::ClearDescription(const std::string& parameter)
{
	// This will just do nothing if the entry does not exist
	parameterDescriptions.erase(parameter);
	return;
}

void Hazelnp::CmdArgsInterface::ClearDescriptions()
{
	parameterDescriptions.clear();
	return;
}

std::string CmdArgsInterface::GenerateDocumentation() const
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
		std::string incompatibilities;
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
		cached.type = DataTypeToString(it.second.requiredType);
		
		// Build default-value string
		std::stringstream vec2str_ss;
		for (const std::string& s : it.second.defaultValue)
		{
			vec2str_ss << '\'' << s << '\'';

			// Add a space if we are not at the last entry
			if ((void*)&s != (void*)&it.second.defaultValue.back())
				vec2str_ss << " ";
		}
		cached.defaultVal = vec2str_ss.str();


		// Build incompatibilities string
		vec2str_ss.str("");
		for (const std::string& s : it.second.incompatibleParameters)
		{
			vec2str_ss << s;

			// Add a comma-space if we are not at the last entry
			if ((void*)&s != (void*)&it.second.incompatibleParameters.back())
				vec2str_ss << ", ";
		}
		cached.incompatibilities = vec2str_ss.str();
	}

	// Now generate the documentation body
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

			// Put incompatibilities
			if (pde.incompatibilities.length() > 0)
				ss << "incompatibilities=[" << pde.incompatibilities << "]   ";

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

void CmdArgsInterface::ApplyConstraints()
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
		// The parameter in question IS supplied
		else
		{
			// Enforce parameter incompatibility

			// Is ANY parameter present listed as incompatible with our current one?
			for (const std::string& incompatibility : pc.second.incompatibleParameters)
				for (const auto& otherParam : parameters)
				{
					if (otherParam.first == incompatibility)
						throw HazelnuppConstraintIncompatibleParameters(pc.second.key, incompatibility);
				}
		}

	return;
}

ParamConstraint CmdArgsInterface::GetConstraint(const std::string& parameter) const
{
	return parameterConstraints.find(parameter)->second;
}

void CmdArgsInterface::ClearConstraint(const std::string& parameter)
{
	parameterConstraints.erase(parameter);
	return;
}

const std::string& CmdArgsInterface::GetExecutableName() const
{
	return executableName;
}

const Value& CmdArgsInterface::operator[](const std::string& key) const
{
	// Throw exception if param is unknown
	if (!HasParam(key))
		throw HazelnuppInvalidKeyException();

	return *parameters.find(key)->second->GetValue();
}

void CmdArgsInterface::RegisterAbbreviation(const std::string& abbrev, const std::string& target)
{
	parameterAbreviations.insert(std::pair<std::string, std::string>(abbrev, target));
	return;
}

const std::string& CmdArgsInterface::GetAbbreviation(const std::string& abbrev) const
{
	if (!HasAbbreviation(abbrev))
		return Placeholders::g_emptyString;

	return parameterAbreviations.find(abbrev)->second;
}

bool CmdArgsInterface::HasAbbreviation(const std::string& abbrev) const
{
	return parameterAbreviations.find(abbrev) != parameterAbreviations.end();
}

void CmdArgsInterface::ClearAbbreviation(const std::string& abbrevation)
{
	parameterAbreviations.erase(abbrevation);
	return;
}

void CmdArgsInterface::ClearAbbreviations()
{
	parameterAbreviations.clear();
	return;
}

void CmdArgsInterface::RegisterConstraint(const std::string& key, const ParamConstraint& constraint)
{
	// Magic syntax, wooo
	(parameterConstraints[key] = constraint).key = key;
	return;
}

void CmdArgsInterface::ClearConstraints()
{
	parameterConstraints.clear();
	return;
}

void CmdArgsInterface::SetCrashOnFail(bool crashOnFail)
{
	this->crashOnFail = crashOnFail;
	return;
}

const ParamConstraint* CmdArgsInterface::GetConstraintForKey(const std::string& key) const
{
	const auto constraint = parameterConstraints.find(key);

	if (constraint == parameterConstraints.end())
		return nullptr;

	return &constraint->second;
}
