#include <iostream>
#include "Hazelnupp.h"
#include "IntValue.h"

int main(int argc, char** argv)
{
	while (1)
	{
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

		args.Parse(testArgv.size(), testArgv.data());
		//args.Parse(argc, argv);
		
		if (args.HasParam("--alfredo"))
		{
			std::cout << args["--alfredo"] << std::endl;
		}
		else
		{
			std::cout << "No --alfredo!" << std::endl;
		}
	}

	return 0;
}
