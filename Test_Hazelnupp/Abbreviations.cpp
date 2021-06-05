#include "CppUnitTest.h"
#include "helper.h"
#include "../Hazelnupp/Hazelnupp.h"

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
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterAbbreviation("-ms", "--my_string");
			nupp.RegisterAbbreviation("-mv", "--my_void");
			nupp.RegisterAbbreviation("-mi", "--my_int");
			nupp.RegisterAbbreviation("-mf", "--my_float");
			nupp.RegisterAbbreviation("-mnl", "--my_num_list");
			nupp.RegisterAbbreviation("-msl", "--my_str_list");

			nupp.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(nupp.HasParam("--my_string"));
			Assert::IsTrue(nupp.HasParam("--my_void"));
			Assert::IsTrue(nupp.HasParam("--my_float"));
			Assert::IsTrue(nupp.HasParam("--my_int"));
			Assert::IsTrue(nupp.HasParam("--my_num_list"));
			Assert::IsTrue(nupp.HasParam("--my_str_list"));

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
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterAbbreviation("-ms", "--my_string");
			nupp.RegisterAbbreviation("-mv", "--my_void");
			nupp.RegisterAbbreviation("-mi", "--my_int");
			nupp.RegisterAbbreviation("-mf", "--my_float");
			nupp.RegisterAbbreviation("-mnl", "--my_num_list");
			nupp.RegisterAbbreviation("-msl", "--my_str_list");

			nupp.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(nupp["--my_string"].GetDataType() == DATA_TYPE::STRING);
			Assert::IsTrue(nupp["--my_void"].GetDataType() == DATA_TYPE::VOID);
			Assert::IsTrue(nupp["--my_float"].GetDataType() == DATA_TYPE::FLOAT);
			Assert::IsTrue(nupp["--my_int"].GetDataType() == DATA_TYPE::INT);
			Assert::IsTrue(nupp["--my_num_list"].GetDataType() == DATA_TYPE::LIST);
			Assert::IsTrue(nupp["--my_str_list"].GetDataType() == DATA_TYPE::LIST);

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
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterAbbreviation("-ms", "--my_string");
			nupp.RegisterAbbreviation("-mv", "--my_void");
			nupp.RegisterAbbreviation("-mi", "--my_int");
			nupp.RegisterAbbreviation("-mf", "--my_float");
			nupp.RegisterAbbreviation("-mnl", "--my_num_list");
			nupp.RegisterAbbreviation("-msl", "--my_str_list");

			nupp.Parse(C_Ify(args));

			// Verify
			Assert::AreEqual(nupp["--my_string"].GetString(), std::string("billybob"));
			Assert::AreEqual(nupp["--my_float"].GetFloat32(), -23.199);
			Assert::AreEqual(nupp["--my_int"].GetInt32(), 199);
			Assert::AreEqual(nupp["--my_num_list"].GetList()[0]->GetInt32(), 1);
			Assert::AreEqual(nupp["--my_num_list"].GetList()[1]->GetInt32(), 2);
			Assert::AreEqual(nupp["--my_num_list"].GetList()[2]->GetInt32(), 3);
			Assert::AreEqual(nupp["--my_num_list"].GetList()[3]->GetInt32(), 4);
			Assert::AreEqual(nupp["--my_str_list"].GetList()[0]->GetString(), std::string("apple"));
			Assert::AreEqual(nupp["--my_str_list"].GetList()[1]->GetString(), std::string("banana"));
			Assert::AreEqual(nupp["--my_str_list"].GetList()[2]->GetString(), std::string("pumpkin"));

			return;
		}

		// Tests that abbreviations can be queried
		TEST_METHOD(Get_Abbreviation)
		{
			// Setup
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterAbbreviation("-ms", "--my_string");
			nupp.RegisterAbbreviation("-mv", "--my_void");
			nupp.RegisterAbbreviation("-mi", "--my_int");
			nupp.RegisterAbbreviation("-mf", "--my_float");
			nupp.RegisterAbbreviation("-mnl", "--my_num_list");
			nupp.RegisterAbbreviation("-msl", "--my_str_list");

			// Exercise, verify
			Assert::AreEqual(std::string("--my_num_list"), nupp.GetAbbreviation("-mnl"));
			Assert::AreEqual(std::string("--my_void"), nupp.GetAbbreviation("-mv"));
			
			return;
		}

		// Tests that getting an unknown abbreviation will ersult in an empty string
		TEST_METHOD(Unknown_Abbrevation_Is_Empty_String)
		{
			// Setup
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterAbbreviation("-ms", "--my_string");
			nupp.RegisterAbbreviation("-mv", "--my_void");
			nupp.RegisterAbbreviation("-mi", "--my_int");
			nupp.RegisterAbbreviation("-mf", "--my_float");
			nupp.RegisterAbbreviation("-mnl", "--my_num_list");
			nupp.RegisterAbbreviation("-msl", "--my_str_list");

			// Exercise
			nupp.ClearAbbreviations();

			// Verify
			Assert::AreEqual(std::string(), nupp.GetAbbreviation("-t"));

			return;
		}

		// Tests that HasAbbreviation works
		TEST_METHOD(Has_Abbreviation)
		{
			// Setup
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			// Exercise, verify
			Assert::IsFalse(nupp.HasAbbreviation("-f"));
			Assert::IsFalse(nupp.HasAbbreviation("-m"));

			nupp.RegisterAbbreviation("-f", "--force");

			Assert::IsTrue(nupp.HasAbbreviation("-f"));
			Assert::IsFalse(nupp.HasAbbreviation("-m"));

			return;
		}

		// Tests that abbreviations can be deleted individually
		TEST_METHOD(Clear_Abbreviation)
		{
			// Setup
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterAbbreviation("-ms", "--my_string");
			nupp.RegisterAbbreviation("-mv", "--my_void");
			nupp.RegisterAbbreviation("-mi", "--my_int");
			nupp.RegisterAbbreviation("-mf", "--my_float");
			nupp.RegisterAbbreviation("-mnl", "--my_num_list");
			nupp.RegisterAbbreviation("-msl", "--my_str_list");

			// Exercise
			nupp.ClearAbbreviation("-mv");
			nupp.ClearAbbreviation("-mf");
			nupp.ClearAbbreviation("-msl");

			// Verify
			Assert::IsTrue(nupp.HasAbbreviation("-ms"));
			Assert::IsFalse(nupp.HasAbbreviation("-mv"));
			Assert::IsTrue(nupp.HasAbbreviation("-mi"));
			Assert::IsFalse(nupp.HasAbbreviation("-mf"));
			Assert::IsTrue(nupp.HasAbbreviation("-mnl"));
			Assert::IsFalse(nupp.HasAbbreviation("-msl"));

			return;
		}

		// Tests that all abbreviations can be deleted at once
		TEST_METHOD(Clear_Abbreviations)
		{
			// Setup
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterAbbreviation("-ms" , "--my_string");
			nupp.RegisterAbbreviation("-mv" , "--my_void");
			nupp.RegisterAbbreviation("-mi" , "--my_int");
			nupp.RegisterAbbreviation("-mf" , "--my_float");
			nupp.RegisterAbbreviation("-mnl", "--my_num_list");
			nupp.RegisterAbbreviation("-msl", "--my_str_list");

			// Exercise
			nupp.ClearAbbreviations();

			// Verify
			Assert::IsFalse(nupp.HasAbbreviation("-ms" ));
			Assert::IsFalse(nupp.HasAbbreviation("-mv" ));
			Assert::IsFalse(nupp.HasAbbreviation("-mi" ));
			Assert::IsFalse(nupp.HasAbbreviation("-mf" ));
			Assert::IsFalse(nupp.HasAbbreviation("-mnl"));
			Assert::IsFalse(nupp.HasAbbreviation("-msl"));

			return;
		}
	};
}
