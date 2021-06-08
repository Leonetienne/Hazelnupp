#include "CppUnitTest.h"
#include "helper.h"
#include "../Hazelnupp/CmdArgsInterface.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Hazelnp;

namespace TestHazelnupp
{
	TEST_CLASS(_Abbreviations)
	{
	public:

		// Tests keys exist after parsing
		TEST_METHOD(KeysExist)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"-ms",
				"billybob",
				"-mv",
				"-mf",
				"-23.199",
				"-mi",
				"199",
				"-mnl",
				"1",
				"2",
				"3",
				"4",
				"-msl",
				"apple",
				"banana",
				"pumpkin",
				});

			// Exercise
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterAbbreviation("-ms", "--my_string");
			cmdArgsI.RegisterAbbreviation("-mv", "--my_void");
			cmdArgsI.RegisterAbbreviation("-mi", "--my_int");
			cmdArgsI.RegisterAbbreviation("-mf", "--my_float");
			cmdArgsI.RegisterAbbreviation("-mnl", "--my_num_list");
			cmdArgsI.RegisterAbbreviation("-msl", "--my_str_list");

			cmdArgsI.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(cmdArgsI.HasParam("--my_string"));
			Assert::IsTrue(cmdArgsI.HasParam("--my_void"));
			Assert::IsTrue(cmdArgsI.HasParam("--my_float"));
			Assert::IsTrue(cmdArgsI.HasParam("--my_int"));
			Assert::IsTrue(cmdArgsI.HasParam("--my_num_list"));
			Assert::IsTrue(cmdArgsI.HasParam("--my_str_list"));

			return;
		}

		// Tests keys are of the correct type after parsing
		TEST_METHOD(CorrectType)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"-ms",
				"billybob",
				"-mv",
				"-mf",
				"-23.199",
				"-mi",
				"199",
				"-mnl",
				"1",
				"2",
				"3",
				"4",
				"-msl",
				"apple",
				"banana",
				"pumpkin",
				});

			// Exercise
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterAbbreviation("-ms", "--my_string");
			cmdArgsI.RegisterAbbreviation("-mv", "--my_void");
			cmdArgsI.RegisterAbbreviation("-mi", "--my_int");
			cmdArgsI.RegisterAbbreviation("-mf", "--my_float");
			cmdArgsI.RegisterAbbreviation("-mnl", "--my_num_list");
			cmdArgsI.RegisterAbbreviation("-msl", "--my_str_list");

			cmdArgsI.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(cmdArgsI["--my_string"].GetDataType() == DATA_TYPE::STRING);
			Assert::IsTrue(cmdArgsI["--my_void"].GetDataType() == DATA_TYPE::VOID);
			Assert::IsTrue(cmdArgsI["--my_float"].GetDataType() == DATA_TYPE::FLOAT);
			Assert::IsTrue(cmdArgsI["--my_int"].GetDataType() == DATA_TYPE::INT);
			Assert::IsTrue(cmdArgsI["--my_num_list"].GetDataType() == DATA_TYPE::LIST);
			Assert::IsTrue(cmdArgsI["--my_str_list"].GetDataType() == DATA_TYPE::LIST);

			return;
		}

		// Tests keys have the correct value after parsing
		TEST_METHOD(CorrectValues)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"-ms",
				"billybob",
				"-mv",
				"-mf",
				"-23.199",
				"-mi",
				"199",
				"-mnl",
				"1",
				"2",
				"3",
				"4",
				"-msl",
				"apple",
				"banana",
				"pumpkin",
				});

			// Exercise
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterAbbreviation("-ms", "--my_string");
			cmdArgsI.RegisterAbbreviation("-mv", "--my_void");
			cmdArgsI.RegisterAbbreviation("-mi", "--my_int");
			cmdArgsI.RegisterAbbreviation("-mf", "--my_float");
			cmdArgsI.RegisterAbbreviation("-mnl", "--my_num_list");
			cmdArgsI.RegisterAbbreviation("-msl", "--my_str_list");

			cmdArgsI.Parse(C_Ify(args));

			// Verify
			Assert::AreEqual(cmdArgsI["--my_string"].GetString(), std::string("billybob"));
			Assert::AreEqual(cmdArgsI["--my_float"].GetFloat32(), -23.199);
			Assert::AreEqual(cmdArgsI["--my_int"].GetInt32(), 199);
			Assert::AreEqual(cmdArgsI["--my_num_list"].GetList()[0]->GetInt32(), 1);
			Assert::AreEqual(cmdArgsI["--my_num_list"].GetList()[1]->GetInt32(), 2);
			Assert::AreEqual(cmdArgsI["--my_num_list"].GetList()[2]->GetInt32(), 3);
			Assert::AreEqual(cmdArgsI["--my_num_list"].GetList()[3]->GetInt32(), 4);
			Assert::AreEqual(cmdArgsI["--my_str_list"].GetList()[0]->GetString(), std::string("apple"));
			Assert::AreEqual(cmdArgsI["--my_str_list"].GetList()[1]->GetString(), std::string("banana"));
			Assert::AreEqual(cmdArgsI["--my_str_list"].GetList()[2]->GetString(), std::string("pumpkin"));

			return;
		}

		// Tests that abbreviations can be queried
		TEST_METHOD(Get_Abbreviation)
		{
			// Setup
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterAbbreviation("-ms", "--my_string");
			cmdArgsI.RegisterAbbreviation("-mv", "--my_void");
			cmdArgsI.RegisterAbbreviation("-mi", "--my_int");
			cmdArgsI.RegisterAbbreviation("-mf", "--my_float");
			cmdArgsI.RegisterAbbreviation("-mnl", "--my_num_list");
			cmdArgsI.RegisterAbbreviation("-msl", "--my_str_list");

			// Exercise, verify
			Assert::AreEqual(std::string("--my_num_list"), cmdArgsI.GetAbbreviation("-mnl"));
			Assert::AreEqual(std::string("--my_void"), cmdArgsI.GetAbbreviation("-mv"));
			
			return;
		}

		// Tests that getting an unknown abbreviation will ersult in an empty string
		TEST_METHOD(Unknown_Abbrevation_Is_Empty_String)
		{
			// Setup
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterAbbreviation("-ms", "--my_string");
			cmdArgsI.RegisterAbbreviation("-mv", "--my_void");
			cmdArgsI.RegisterAbbreviation("-mi", "--my_int");
			cmdArgsI.RegisterAbbreviation("-mf", "--my_float");
			cmdArgsI.RegisterAbbreviation("-mnl", "--my_num_list");
			cmdArgsI.RegisterAbbreviation("-msl", "--my_str_list");

			// Exercise
			cmdArgsI.ClearAbbreviations();

			// Verify
			Assert::AreEqual(std::string(), cmdArgsI.GetAbbreviation("-t"));

			return;
		}

		// Tests that HasAbbreviation works
		TEST_METHOD(Has_Abbreviation)
		{
			// Setup
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			// Exercise, verify
			Assert::IsFalse(cmdArgsI.HasAbbreviation("-f"));
			Assert::IsFalse(cmdArgsI.HasAbbreviation("-m"));

			cmdArgsI.RegisterAbbreviation("-f", "--force");

			Assert::IsTrue(cmdArgsI.HasAbbreviation("-f"));
			Assert::IsFalse(cmdArgsI.HasAbbreviation("-m"));

			return;
		}

		// Tests that abbreviations can be deleted individually
		TEST_METHOD(Clear_Abbreviation)
		{
			// Setup
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterAbbreviation("-ms", "--my_string");
			cmdArgsI.RegisterAbbreviation("-mv", "--my_void");
			cmdArgsI.RegisterAbbreviation("-mi", "--my_int");
			cmdArgsI.RegisterAbbreviation("-mf", "--my_float");
			cmdArgsI.RegisterAbbreviation("-mnl", "--my_num_list");
			cmdArgsI.RegisterAbbreviation("-msl", "--my_str_list");

			// Exercise
			cmdArgsI.ClearAbbreviation("-mv");
			cmdArgsI.ClearAbbreviation("-mf");
			cmdArgsI.ClearAbbreviation("-msl");

			// Verify
			Assert::IsTrue(cmdArgsI.HasAbbreviation("-ms"));
			Assert::IsFalse(cmdArgsI.HasAbbreviation("-mv"));
			Assert::IsTrue(cmdArgsI.HasAbbreviation("-mi"));
			Assert::IsFalse(cmdArgsI.HasAbbreviation("-mf"));
			Assert::IsTrue(cmdArgsI.HasAbbreviation("-mnl"));
			Assert::IsFalse(cmdArgsI.HasAbbreviation("-msl"));

			return;
		}

		// Tests that all abbreviations can be deleted at once
		TEST_METHOD(Clear_Abbreviations)
		{
			// Setup
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterAbbreviation("-ms" , "--my_string");
			cmdArgsI.RegisterAbbreviation("-mv" , "--my_void");
			cmdArgsI.RegisterAbbreviation("-mi" , "--my_int");
			cmdArgsI.RegisterAbbreviation("-mf" , "--my_float");
			cmdArgsI.RegisterAbbreviation("-mnl", "--my_num_list");
			cmdArgsI.RegisterAbbreviation("-msl", "--my_str_list");

			// Exercise
			cmdArgsI.ClearAbbreviations();

			// Verify
			Assert::IsFalse(cmdArgsI.HasAbbreviation("-ms" ));
			Assert::IsFalse(cmdArgsI.HasAbbreviation("-mv" ));
			Assert::IsFalse(cmdArgsI.HasAbbreviation("-mi" ));
			Assert::IsFalse(cmdArgsI.HasAbbreviation("-mf" ));
			Assert::IsFalse(cmdArgsI.HasAbbreviation("-mnl"));
			Assert::IsFalse(cmdArgsI.HasAbbreviation("-msl"));

			return;
		}
	};
}
