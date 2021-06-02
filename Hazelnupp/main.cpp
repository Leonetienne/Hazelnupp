#include <iostream>
#include <array>
#include "Hazelnupp.h"
#include "IntValue.h"

int main(int argc, char** argv)
{
	std::string arg0 = "meinpfad";
	std::string arg1 = "-w";
	std::string arg2 = "6669";
	std::string arg3 = "--alfred";
	std::string arg4 = "banane7";

	std::array<const char*, 5> testArgv = {
		arg0.data(),
		arg1.data(),
		arg2.data(),
		arg3.data(),
		arg4.data()
	};

	Hazelnupp args;

	args.RegisterAbbreviation("-w", "--word");

	//args.Parse(testArgv.size(), testArgv.data());
	args.Parse(argc, argv);

	if (args.HasParam("--word"))
	{
		int i = args["--word"]->GetInt32();
		std::cout << i << std::endl;
	}
	else
	{
		std::cout << "No --word!" << std::endl;
	}

	//std::cout << args.GetExecutableName() << std::endl;

	return 0;
}
