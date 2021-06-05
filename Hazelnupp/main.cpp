#include <iostream>
#include "Hazelnupp.h"
#include "IntValue.h"

using namespace Hazelnp;

int main(int argc, char** argv)
{
	Hazelnupp nupp;

	nupp.SetBriefDescription("This is the testing application for Hazelnupp.");

	nupp.RegisterDescription("--help", "This will display the parameter documentation.");
	nupp.RegisterDescription("--force", "Just forces it.");
	nupp.RegisterDescription("--width", "The width of something...");
	nupp.RegisterDescription("--name", "The names to target");
	nupp.RegisterDescription("--fruit", "The fruit to use");

	nupp.RegisterAbbreviation("-f", "--force");
	nupp.RegisterAbbreviation("-w", "--width");
	nupp.RegisterAbbreviation("-h", "--height");

	nupp.RegisterConstraint("--width", ParamConstraint::TypeSafety(DATA_TYPE::FLOAT));
	nupp.RegisterConstraint("--name", ParamConstraint(true, DATA_TYPE::LIST, {"peter", "hannes"}, true));
	nupp.RegisterConstraint("--fruit", ParamConstraint(true, DATA_TYPE::STRING, {}, true));

	nupp.Parse(argc, argv);

	return 0;
}
