[![Hazelnupp](https://raw.githubusercontent.com/Leonetienne/Hazelnupp/master/Brand/nupp.png)](https://github.com/Leonetienne/Hazelnupp)

# [Documentation](https://leonetienne.github.io/Hazelnupp/)
## [Direct link to docs of the main class](https://leonetienne.github.io/Hazelnupp/classHazelnupp.html)


<style>
#readme a[href] {
	color: #A6BF29;
}
</style>

# Hazelnupp
is a simple, easy to use command line parameter parser.  
Hazelnupp does not support windows-, or bsd-style arguments. Only linux-style.  
  
What is the linux-style? This:
```
# Using a long parameter
$ a.out --long-parameter 1234

# Using an abbreviated parameter
$ a.out -lp 1234
```

## Note
These examples reference exceptions. These are not enabled by default. The default behaviour for user-fault exceptions is to produce output to `stderr` and kill the process.  
To enable exceptions, call this method:
```cpp
Hazelnupp args;
args.SetCrashOnFail(false);
```

## Index
1. [Importing into a project](#importing-into-a-project)
2. [What's the concept?](#whats-the-concept)
3. [Minimal working example](#minimal-working-example)
4. [Abbreviations](#abbreviations)
5. [Constraints](#constraints)
6. [Automatic parameter documentation](#automatic-parameter-documentation)
7. [More examples?](#more-examples)
8. [What is not supported?](#what-is-not-supported)
9. [Further notes](#further-notes)
10. [Contributing](#contributing)
11. [LICENSE](#license)

<span id="importing-into-a-project"></span>
## Importing into a project
> How do i actually import this into my existing project?

Super easily! Just grab the latest files (2) from [/INCLUDE](https://github.com/Leonetienne/Hazelnupp/tree/master/INCLUDE) and put them into your project!
You may have to add the .cpp to your compile list, but most IDEs should do this automatically.

<span id="whats-the-concept"></span>
## What's the concept?
The concept is that each parameter must be one of five types. These are:
* Void
* Int
* Float
* String
* List (non-recursive)

Here are examples on how to create them
```
# Void
$ a.out --foo

# Int
$ a.out --foo 5

# Float
$ a.out --foo 5.5

# String
$ a.out --foo peter

# List (any type above works)
$ a.out --foo peter jake jeff billy

# List, mixed types
$ a.out --foo 1 2 3 4 peter willy billy bob 3
```

These parameters can then be accessed via a simple lookup!

<span id="minimal-working-example"></span>
## Minimal working example
So what's the simplest way to use Hazelnupp to work with command-line parameters? See:
```cpp
#include "Hazelnupp.h"
using namespace Hazelnp;

int main(int argc, char** argv)
{
	Hazelnupp args(argc, argv);

	if (args.HasParam("--force"))
		// do forced
	else
		// be gentle

	return 0;
}
```

Looks super easy! But what about actual values?
```cpp
#include "Hazelnupp.h"
using namespace Hazelnp;

int main(int argc, char** argv)
{
	Hazelnupp args(argc, argv);

	// Either check via HasParam(), or do a try-catch
	try
	{
		int myInt          = args["--my-int"].GetInt32();
		double myFlt       = args["--my-float"].GetFloat32();
		std::string myStr  = args["--my-string"].GetString();
	}
	catch (HazelnuppInvalidKeyException&)
	{
		return -1;
	}

	return 0;
}
```

What about lists?
```cpp
#include "Hazelnupp.h"
using namespace Hazelnp;

int main(int argc, char** argv)
{
	Hazelnupp args(argc, argv);

	const auto& myList = args["--my-list"].GetList(); // std::vector<Value*>
	
	for (const auto* it : myList)
	{
		// Should probably check for type-correctness with it->GetDataType()
		std::cout << it->GetString() << std::endl;
	}

	return 0;
}
```

<span id="abbreviations"></span>
## Abbreviations
Abbreviations are a very important part of command line arguments. Like, typing `-f` instead of `--force`.
Here's how to use them in Hazelnupp:
```cpp
#include "Hazelnupp.h"
using namespace Hazelnp;

int main(int argc, char** argv)
{
	Hazelnupp args;

	// Register abbreviations
	args.RegisterAbbreviation("-f", "--force");
	
	// Parse
	args.Parse(argc, argv);

	if (args.HasParam("--force")) // This key will be present, even if the user passed '-f'
		// do forced
	else
		// be gentle

	return 0;
}
```

<span id="constraints"></span>
## Constraints
> That's all cool and stuff, but this looks like a **LOT** of error-checking and not elegant at all! How would i *actually* use this?

For exactly this reason, there are constraints. With this, you can control what the data looks like! Constraints serve two main purposes:

### Requiring data
With `ParamConstraint::Require()` you can declare that a paramater must either always be present, or provide a default value.  
* If a parameter is not present, but has a default value, it will be automatically created.
* If a parameter is not present, and has no default value, an exception will be thrown.

Minimal working example:
```cpp
#include "Hazelnupp.h"
using namespace Hazelnp;

int main(int argc, char** argv)
{
	Hazelnupp args;
	
	// Register constraints
	args.RegisterConstraints({
		ParamConstraint::Require("--this-is-required"), // This missing throws an exception
		ParamConstraint::Require("--also-required-but-defaulted", {"122"}), // This will default to 122
	});
	
	// Parse
	args.Parse(argc, argv);

	return 0;
}
```

### Type safety
With type safety you can always be certain that you are working with the correct type!  
By creating a type-constraint you force Hazelnupp to use a certain type.  
If a supplied type does not match, but is convertible, it will be converted.  
If it is not convertible, an exception will be thrown.

These conversions are:
* int -> [float, string, list, void]
* float ->[int, string, list, void]
* string -> [list, void]
* list -> [void]
* void -> [list]

The conversions `*->list` just create a list with a single entry (except for `void->list` which produces an empty list).  
The `*->void` conversions just drop their value.

Minimal working example:
```cpp
#include "Hazelnupp.h"
using namespace Hazelnp;

int main(int argc, char** argv)
{
	Hazelnupp args;
	
	// Register constraints
	args.RegisterConstraints({
		ParamConstraint::TypeSafety("--this-must-be-int", DATA_TYPE::INT)
	});
	
	// Parse
	args.Parse(argc, argv);

	return 0;
}
```
If `--this-must-be-int` would be passed as a float, it would be converted to int.
If it was passed, for example, as a string, it would throw an exception.

---
Note that you can also combine these two constraint-types by populating the struct yourself:
```cpp
ParamConstraint pc;
pc.key = "--my-key";
pc.constrainType = true;
pc.wantedType = DATA_TYPE::STRING;
pc.defaultValue = {}; // no default value
pc.required = true;

args.RegisterConstraints({pc});
```
What doesn't work is inserting multiple constraints for one key. It will just discard the oldest one. But that's okay because one can describe all possible constraints for a single key in **one** struct.

<span id="automatic-parameter-documentation"></span>
## Automatic parameter documentation
Hazelnupp does automatically create a parameter documentation, accessible via `--help`.  
If you want to use `--help` yourself, just turn it off.
```cpp
Hazelnupp args;
args.SetCatchHelp(false);
```

What does this automatically generated documentation look like?
```
$ a.out --help

This is the testing application for Hazelnupp.

==== AVAILABLE PARAMETERS ====

--help   This will display the parameter documentation.

--names   LIST   default=['peter' 'hannes']   The names to target

--force   -f   Just forces it.

--width   -w   FLOAT   The width of something...

--fruit   STRING   [[REQUIRED]]    The fruit to use

--height   -h
```

This documentation is automatically fed by any information provided on parameters.  
You have to set the brief descriptions yourself though.
```cpp
Hazelnupp args;
args.RegisterDescription("--force", "Just forces it.");
```

Additionally you can provide a brief description of your application to be added right above the parameter list.
```cpp
Hazelnupp args;
args.SetBriefDescription("This is the testing application for Hazelnupp.");
```

If you want to display this information somewhere else, you can always access it as a string via `args.GenerateDocumentation()`.

<span id="more-examples"></span>
## More examples?
Check out the [tests](https://github.com/Leonetienne/Hazelnupp/tree/master/Test_Hazelnupp)! They may help you out!  
Also make sure to check out the [doxygen docs](https://leonetienne.github.io/Hazelnupp/)!

<span id="what-is-not-supported"></span>
## What is not supported?
Chaining abbreviated parameters, like this:
```
# This is not supported. It would think -ltr is one parameter.
$ a.out -ltr

# Instead do this
$ a.out -l -t -r

```

Using parameters multiple times
```
# This is not supported.
# Let's say -i is short for --input
$ a.out -i hello.txt -i shoe.txt -i somsang.txt

# Instead do this
$ a.out -i hello.txt shoe.txt somsang.txt
```

<span id="further-notes"></span>
## Further notes
This is still in alpha! There is no guarantee at all that this actually works.  
Whilst i did my best do make sure it does, i bet there are still a few flaws i've overlooked.  
Please know that i am not obliged to work on fixes. I do have other stuff to do.
This does not mean that i won't, but i'm not sure when.  
Feel free to submit a PR if you think you improved it in any way :)

<span id="#contributing"></span>
## Contributing
If you want to contribute, feel free to fork the repository, and submit a pull request.  
Bugfixes and tests are almost certain to be accepted, features should agreed upon and come with tests.  
Just create an issue with the tag `feature request`. Don't forget to update the UML (`Hazelnupp.vpp`)! The (free) modelling software used is [Visual Paradigm](https://www.visual-paradigm.com).  
  
Any code added must match the existing style!
* Objects begin with a lowercase initial
* Classifiers and Functions/Methods begin with an uppercase initial
* Classifiers are camel-case
* Classifiers get documented via `/** */` for doxygen. See existing classifiers
* Members (methods and objects) get documented via `//!` for doxygen. See existing definitions.
* `{` always gets a new line
* Enumerations (and their values) and macros are all-upper case snake-case
* No `using namespace std`
* Do `using namespace Hazelnp` in cpp files. Don't do `Hazelnp::` if possible
* Files outside the project (like STL) have to be included with `#include <>`. Not `""`

<span id="license"></span>
## LICENSE
```
Copyright (c) 2021, Leon Etienne

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
