#pragma once
#include "Parameter.h"
#include "ParamConstraint.h"
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
		//! Construct the ParamConstraint struct yourself to combine Require and TypeSafety! You can also use the ParamConstraint constructor!
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
