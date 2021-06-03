[![Hazelnupp](https://raw.githubusercontent.com/Leonetienne/Hazelnupp/master/Brand/nupp.png)](https://github.com/Leonetienne/Hazelnupp)

# [Documentation](https://leonetienne.github.io/Hazelnupp/)
## [Direct link to docs of the main class](https://leonetienne.github.io/Hazelnupp/classHazelnupp.html)

# Hazelnupp
is a simple, easy to use command line parameter parser.  
Hazelnupp does not support windows-, or bsd-style arguments. Only linux-style.  
  
What is the linux-style? This:
```
# Using a long parameter
a.out --long-parameter 1234
```

```
# Using an abbreviated parameter
a.out -lp 1234
```

## Note
These examples reference exceptions. These are not enabled by default. The default behaviour for user-fault exceptions is to produce output to `stderr` and kill the process.  
To enable exceptions, call this method:
```cpp
Hazelnupp args;
args.SetCrashOnFail(false);
```

## Importing into a project
> How do i actually import this into my existing project?

Super easily! Just grab the latest files (2) from [/INCLUDE](https://github.com/Leonetienne/Hazelnupp/tree/master/INCLUDE) and put them into your project!
You may have to add the .cpp to your compile list, but most IDEs should do this automatically.

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
a.out --foo

# Int
a.out --foo 5

# Float
a.out --foo 5.5

# String
a.out --foo peter

# List (any type above works)
a.out --foo peter jake jeff billy

# List, mixed types
a.out --foo 1 2 3 4 peter willy billy bob 3
```

These parameters can then be accessed via a simple lookup!

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

## More examples?
Check out the unit tests! They may help you out!  
Also make sure to check out the [doxygen docs](https://leonetienne.github.io/Hazelnupp/)!

## Further notes
This is still in alpha! There is no guarantee at all that this actually works.  
Whilst i did my best do make sure it does, i bet there are still a few flaws i did overlook.  
Please know that i am not obliged to work on fixes. I do have other stuff to do.
This does not mean that i won't do it, but i'm not sure when.  
Feel free to submit a PR if you fixed something :)

## What is not supported?
Chaining abbreviated parameters, like this:
```
# This is not supported. It would think -ltr is one parameter.
a.out -ltr

# Instead do this
a.out -l -t -r

```

## LICENSE
```
Copyright (c) 2021, Leon Etienne

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
