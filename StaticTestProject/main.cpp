#include <iostream>
#include "../Hazelnupp/CmdArgsInterface.h"

using namespace Hazelnp;

int main(int argc, char** argv)
{
	CmdArgsInterface args;

	args.SetBriefDescription("This is the testing application for Hazelnupp.");

	args.RegisterDescription("--help", "This will display the parameter documentation.");
	args.RegisterDescription("--force", "Just forces it.");
	args.RegisterDescription("--width", "The width of something...");
	args.RegisterDescription("--name", "The names to target");
	args.RegisterDescription("--fruit", "The fruit to use");

	args.RegisterAbbreviation("-f", "--force");
	args.RegisterAbbreviation("-w", "--width");
	args.RegisterAbbreviation("-h", "--height");
	args.RegisterAbbreviation("-d", "--depth");

	args.RegisterConstraint("--width", ParamConstraint::TypeSafety(DATA_TYPE::FLOAT));
	args.RegisterConstraint("--depth", ParamConstraint::TypeSafety(DATA_TYPE::FLOAT));
	args.RegisterConstraint("--name", ParamConstraint(true, DATA_TYPE::LIST, { "peter", "hannes" }, true, {}));
	args.RegisterConstraint("--fruit", ParamConstraint(true, DATA_TYPE::STRING, {}, true, {}));

	args.RegisterConstraint("--make-food-delicious", ParamConstraint::Incompatibility({ "--make-food-disgusting" , "--foo"}));

	args.Parse(argc, argv);

	return 0;
}
