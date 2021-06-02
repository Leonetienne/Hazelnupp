#include <iostream>
#include <array>
#include "Hazelnupp.h"
#include "IntValue.h"

int main(int argc, char** argv)
{
	std::string arg0 = "meinpfad";
	std::string arg1 = "--word";
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

	Hazelnupp args(testArgv.size(), testArgv.data());
	//Hazelnupp args(argc, argv);

	int i = *(IntValue*)args["--word"];
	std::cout << i << std::endl;

	//std::cout << args.GetExecutableName() << std::endl;

	return 0;
}
