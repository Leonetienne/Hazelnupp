#pragma once
#include "Parameter.h"
#include <unordered_map>
#include <vector>

class Hazelnupp
{
public:
	Hazelnupp();
	Hazelnupp(const int argc, const char* const* argv);

	//! Will parse command line arguments
	void Parse(const int argc, const char* const* argv);

	//! Will return argv[0], the name of the executable.
	const std::string& GetExecutableName() const;

	//! Will return the value given a key
	const Value* operator[](const std::string& key);

	//! Will check wether a parameter exists given a key, or not
	bool HasParam(const std::string& key) const;

private:
	//! Will translate the c-like args to an std::vector
	void PopulateRawArgs(const int argc, const char* const* argv);

	//! Will parse the next parameter. Returns the index of the next parameter.
	std::size_t ParseNextParameter(const std::size_t parIndex, Parameter*& out_Par);

	Value* EvaluateValues(const std::vector<std::string>& values);

	std::string executableName; //! The path of the executable. Always argv[0]
	std::unordered_map<std::string, Parameter*> parameters;

	std::vector<std::string> rawArgs;

	// EvaluateValues
	friend class ListValue;
};
