#pragma once
#include "Parameter.h"
#include "ParamConstraint.h"
#include <unordered_map>
#include <vector>

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
	const Value* operator[](const std::string& key) const;

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
};
