#include "CppUnitTest.h"
#include "helper.h"
#include "../Hazelnupp/CmdArgsInterface.h"
#include "../Hazelnupp/HazelnuppException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Hazelnp;

namespace TestHazelnupp
{
	TEST_CLASS(_Conversion)
	{
	public:

		// Tests that string conversion methods work
		TEST_METHOD(Convert_String)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--pud",
				"hello"
			});

			// Exercise
			CmdArgsInterface cmdArgsI(C_Ify(args));
			cmdArgsI.SetCrashOnFail(false);

			// Verify
			const CmdArgsInterface* ptcmdArgsI = &cmdArgsI;

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetInt64();
				}
			, L"Int64");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetInt32();
				}
			, L"Int32");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetFloat64();
				}
			, L"Float64");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetFloat32();
				}
			, L"Float32");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetList();
				}
			, L"List");


			return;
		}

		// Tests that void conversion methods work
		TEST_METHOD(Convert_Void)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--pud"
			});

			// Exercise
			CmdArgsInterface cmdArgsI(C_Ify(args));
			cmdArgsI.SetCrashOnFail(false);

			// Verify
			const CmdArgsInterface* ptcmdArgsI = &cmdArgsI;

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetInt64();
				}
			, L"Int64");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetInt32();
				}
			, L"Int32");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetFloat64();
				}
			, L"Float64");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetFloat32();
				}
			, L"Float32");

			// Expect empty string
			Assert::AreEqual(std::string(), cmdArgsI["--pud"].GetString(), L"String");

			// Expect empty list
			Assert::AreEqual(std::size_t(0), cmdArgsI["--pud"].GetList().size(), L"List");

			return;
		}

		// Tests that int conversion methods work
		TEST_METHOD(Convert_Int)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--pud",
				"39"
			});

			// Exercise
			CmdArgsInterface cmdArgsI(C_Ify(args));
			cmdArgsI.SetCrashOnFail(false);

			// Verify
			const CmdArgsInterface* ptcmdArgsI = &cmdArgsI;

			Assert::AreEqual(39ll, cmdArgsI["--pud"].GetInt64(), L"Int64");
			Assert::AreEqual(39, cmdArgsI["--pud"].GetInt32(), L"Int32");
			Assert::IsTrue(39.0l == cmdArgsI["--pud"].GetFloat64(), L"Float64");
			Assert::AreEqual(39.0, cmdArgsI["--pud"].GetFloat32(), L"Float32");
			Assert::AreEqual(std::string("39"), cmdArgsI["--pud"].GetString(), L"String");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetList();
				}
			, L"List");


			return;
		}

		// Tests that float conversion methods work
		TEST_METHOD(Convert_Float)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--pud",
				"39.5"
			});

			// Exercise
			CmdArgsInterface cmdArgsI(C_Ify(args));
			cmdArgsI.SetCrashOnFail(false);

			// Verify
			const CmdArgsInterface* ptcmdArgsI = &cmdArgsI;

			Assert::AreEqual(39ll, cmdArgsI["--pud"].GetInt64(), L"Int64");
			Assert::AreEqual(39, cmdArgsI["--pud"].GetInt32(), L"Int32");
			Assert::IsTrue(39.5l == cmdArgsI["--pud"].GetFloat64(), L"Float64");
			Assert::AreEqual(39.5, cmdArgsI["--pud"].GetFloat32(), L"Float32");
			Assert::AreEqual(std::string("39.5"), cmdArgsI["--pud"].GetString(), L"String");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetList();
				}
			, L"List");


			return;
		}

		// Tests that list conversion methods work
		TEST_METHOD(Convert_List)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--pud",
				"apple",
				"1",
				"2",
				"3"
			});

			// Exercise
			CmdArgsInterface cmdArgsI(C_Ify(args));
			cmdArgsI.SetCrashOnFail(false);

			// Verify
			const CmdArgsInterface* ptcmdArgsI = &cmdArgsI;

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetInt64();
				}
			, L"Int64");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetInt32();
				}
			, L"Int32");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetFloat64();
				}
			, L"Float64");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetFloat32();
				}
			, L"Float32");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptcmdArgsI]
				{
					(*ptcmdArgsI)["--pud"].GetString();
				}
			, L"String");

			return;
		}
	};
}
