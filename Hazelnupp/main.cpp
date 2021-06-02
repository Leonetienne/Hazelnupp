#include <iostream>
#include "Hazelnupp.h"
#include "IntValue.h"

int main(int argc, char** argv)
{
	while (1)
	{
		// Mock command-line params
		std::vector<const char*> testArgv = {
			"meinpfad",
			"-w",
			"-99",
			"--alfred",
			"apfel",
			"banane",
			"triangle",
			"--numbers",
			"1",
			"2",
			"3",
			"4",
			"5",
		};

		argc = testArgv.size();
		argv = const_cast<char**>(testArgv.data());

		// Prepare Hazelnupp parser
		Hazelnupp args;

		ParamConstraint pc;
		pc.key = "--alfredo";
		pc.constrainType = true;
		pc.wantedType = DATA_TYPE::LIST;
		pc.required = true;
		pc.defaultValue = { "coca cola", "fanta" };

		args.RegisterConstraints({
			pc
		});

		args.RegisterAbbreviation("-w", "--word");

		// Parse
		args.Parse(argc, argv);
		
		// Use
		if (args.HasParam("--alfredo"))
		{
			std::cout << args["--alfredo"].GetInt32() << std::endl;
		}
		else
		{
			std::cout << "No --alfredo!" << std::endl;
		}
	}

	return 0;
}
