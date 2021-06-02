#include "Hazelnupp.h"
#include "VoidValue.h"
#include "IntValue.h"
#include "FloatValue.h"
#include "StringValue.h"
#include "ListValue.h"
#include "StringTools.h"
#include <iostream>

Hazelnupp::Hazelnupp()
{
	return;
}

Hazelnupp::Hazelnupp(const int argc, const char* const* argv)
{
	Parse(argc, argv);
	return;
}

void Hazelnupp::Parse(const int argc, const char* const* argv)
{
	// Populate raw arguments
	PopulateRawArgs(argc, argv);


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

	std::cout << "Terminated." << std::endl;

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

	Value* parsedVal = ParseValue(values);
	out_Par = new Parameter(key, parsedVal);

	delete parsedVal;
	parsedVal = nullptr;

	return i;
}

void Hazelnupp::PopulateRawArgs(const int argc, const char* const* argv)
{
	rawArgs.reserve(argc);
	for (int i = 0; i < argc; i++)
		rawArgs.emplace_back(std::string(argv[i]));

	return;
}

bool Hazelnupp::HasParam(const std::string& key) const
{
	return parameters.find(key) != parameters.end();
}

Value* Hazelnupp::ParseValue(const std::vector<std::string>& values)
{
	// Void-type
	if (values.size() == 0)
	{
		return new VoidValue;
	}

	// List-type
	else if (values.size() > 1)
	{
		ListValue* newList = new ListValue();
		for (const std::string& val : values)
		{
			Value* tmp = ParseValue(std::vector<std::string>({ val }));
			newList->AddValue(tmp);
			delete tmp;
		}
		return newList;
	}

	// Now we're only dealing with a single value
	const std::string& val = values[0];
	
	// String
	if (!StringTools::IsNumeric(val, true))
		return new StringValue(val);

	// Numeric
	bool isInt;
	long double num;
	
	if (StringTools::ParseNumber(val, isInt, num))
	{
		// Integer
		if (isInt)
			return new IntValue((long long int)num);

		// Double
		return new FloatValue(num);
	}

	// Failed
	return nullptr;
}

const std::string& Hazelnupp::GetExecutableName() const
{
	return executableName;
}

const Value* Hazelnupp::operator[](const std::string& key)
{
	return parameters[key]->Value();
}
