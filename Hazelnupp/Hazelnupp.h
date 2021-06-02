#pragma once
#include "Parameter.h"
#include <unordered_map>

class Hazelnupp
{
public:
	Hazelnupp();
	Hazelnupp(int argc, const char* const* argv);

	//! Will parse command line arguments
	void Parse(int argc, const char* const* argv);

	std::unordered_map<std::string, Parameter*> parameters;
};
