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
	catch (HazelnuppConstraintTypeMissmatch hctm)
	{
		std::cerr << "Fatal error: Command-line parameter value-type mismatch at \"" << hctm.What() << "\"!";
		quick_exit(-1009);
	}
	catch (HazelnuppConstraintMissingValue hctm)
	{
		std::cerr << "Fatal error: Missing required command-line parameter \"" << hctm.What() << "\"!";
		quick_exit(-1010);
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

const ParamConstraint* Hazelnupp::GetConstraintForKey(const std::string& key) const
{
	const auto constraint = constraints.find(key);

	if (constraint == constraints.end())
		return nullptr;

	return &constraint->second;
}
