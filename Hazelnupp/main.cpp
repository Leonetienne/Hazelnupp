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
			"hallo",
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

		args.RegisterAbbreviation("-w", "--word");

		args.Parse(testArgv.size(), testArgv.data());
		//args.Parse(argc, argv);
		
		if (args.HasParam("--word"))
		{
			std::cout << *args["--word"] << std::endl;
		}
		else
		{
			std::cout << "No --word!" << std::endl;
		}
	}

	return 0;
}
