#include "Catch2.h"
#include "helper.h"
#include <Hazelnupp/CmdArgsInterface.h>
#include <Hazelnupp/HazelnuppException.h>

using namespace Hazelnp;

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
			CmdArgsInterface cmdArgsI(C_Ify(args));
			cmdArgsI.SetCrashOnFail(false);

			// Verify
			Assert::AreEqual(std::string("/my/fake/path/wahoo.out"), cmdArgsI.GetExecutableName());

			return;
		}

		// Edgecase test: We only have one param.
		TEST_METHOD(Only_One_Param)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--dummy"
			});

			// Exercise
			CmdArgsInterface cmdArgsI(C_Ify(args));
			cmdArgsI.SetCrashOnFail(false);

			// Verify
			Assert::IsTrue(cmdArgsI.HasParam("--dummy"));

			return;
		}

		// Edgecase test: We begin with an actual value, instead of an argument.
		TEST_METHOD(Weird_Case_1)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"dummy"
			});

			// Exercise
			CmdArgsInterface cmdArgsI(C_Ify(args));
			cmdArgsI.SetCrashOnFail(false);

			// Verify (no exception)

			return;
		}

		// Edgecase test: We begin with first an actual value, and then an argument.
		TEST_METHOD(Weird_Case_2)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"dummy",
				"--dummy"
			});

			// Exercise
			CmdArgsInterface cmdArgsI(C_Ify(args));
			cmdArgsI.SetCrashOnFail(false);

			// Verify
			Assert::IsTrue(cmdArgsI.HasParam("--dummy"), L"Failed has-param");
			Assert::IsTrue(cmdArgsI["--dummy"].GetDataType() == DATA_TYPE::VOID, L"Failed type");

			return;
		}

		// Tests keys exist after parsing
		TEST_METHOD(KeysExist)
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
			CmdArgsInterface cmdArgsI(C_Ify(args));
			cmdArgsI.SetCrashOnFail(false);

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
			CmdArgsInterface cmdArgsI(C_Ify(args));
			cmdArgsI.SetCrashOnFail(false);

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
			CmdArgsInterface cmdArgsI(C_Ify(args));
			cmdArgsI.SetCrashOnFail(false);

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

		// Tests that an HazelnuppInvalidKeyException gets raised, if an invalid gey is tried to access
		TEST_METHOD(Exception_On_Invalid_Key)
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

			CmdArgsInterface cmdArgsI(C_Ify(args));
			cmdArgsI.SetCrashOnFail(false);

			// Exercise, Verify
			Assert::ExpectException<HazelnuppInvalidKeyException>(
				[args]
				{
					CmdArgsInterface cmdArgsI(C_Ify(args));
					cmdArgsI["--borrnana"];
				}
			);

			return;
		}
	};
}
