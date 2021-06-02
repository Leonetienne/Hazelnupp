#include "CppUnitTest.h"
#include "helper.h"
#include "../Hazelnupp/Hazelnupp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestHazelnupp
{
	TEST_CLASS(_Basics)
	{
	public:
		
		// Tests the application path gets exported correctly
		TEST_METHOD(ApplicationPathWorks)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out"
				});

			// Exercise
			Hazelnupp nupp(C_Ify(args));

			// Verify
			Assert::AreEqual(std::string("/my/fake/path/wahoo.out"), nupp.GetExecutableName());

			return;
		}

		// Tests keys exist after parsing
		TEST_METHOD(BasicParsing_KeysExist)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--my_string",
				"billybob",
				"--my_void",
				"--my_float",
				"-23.199",
				"--my_int",
				"199",
				"--my_num_list",
				"1",
				"2",
				"3",
				"4",
				"--my_str_list",
				"apple",
				"banana",
				"pumpkin",
				});

			// Exercise
			Hazelnupp nupp(C_Ify(args));

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
		TEST_METHOD(BasicParsing_CorrectType)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--my_string",
				"billybob",
				"--my_void",
				"--my_float",
				"-23.199",
				"--my_int",
				"199",
				"--my_num_list",
				"1",
				"2",
				"3",
				"4",
				"--my_str_list",
				"apple",
				"banana",
				"pumpkin",
				});

			// Exercise
			Hazelnupp nupp(C_Ify(args));

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
		TEST_METHOD(BasicParsing_CorrectValues)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--my_string",
				"billybob",
				"--my_void",
				"--my_float",
				"-23.199",
				"--my_int",
				"199",
				"--my_num_list",
				"1",
				"2",
				"3",
				"4",
				"--my_str_list",
				"apple",
				"banana",
				"pumpkin",
				});

			// Exercise
			Hazelnupp nupp(C_Ify(args));

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
	};
}
