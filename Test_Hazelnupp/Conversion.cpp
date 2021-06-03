#include "CppUnitTest.h"
#include "helper.h"
#include "../Hazelnupp/Hazelnupp.h"
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
			Hazelnupp nupp(C_Ify(args));
			nupp.SetCrashOnFail(false);

			// Verify
			const Hazelnupp* ptnupp = &nupp;

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetInt64();
				}
			, L"Int64");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetInt32();
				}
			, L"Int32");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetFloat64();
				}
			, L"Float64");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetFloat32();
				}
			, L"Float32");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetList();
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
			Hazelnupp nupp(C_Ify(args));
			nupp.SetCrashOnFail(false);

			// Verify
			const Hazelnupp* ptnupp = &nupp;

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetInt64();
				}
			, L"Int64");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetInt32();
				}
			, L"Int32");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetFloat64();
				}
			, L"Float64");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetFloat32();
				}
			, L"Float32");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetString();
				}
			, L"String");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetList();
				}
			, L"List");


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
			Hazelnupp nupp(C_Ify(args));
			nupp.SetCrashOnFail(false);

			// Verify
			const Hazelnupp* ptnupp = &nupp;

			Assert::AreEqual(39ll, nupp["--pud"].GetInt64(), L"Int64");
			Assert::AreEqual(39, nupp["--pud"].GetInt32(), L"Int32");
			Assert::IsTrue(39.0l == nupp["--pud"].GetFloat64(), L"Float64");
			Assert::AreEqual(39.0, nupp["--pud"].GetFloat32(), L"Float32");
			Assert::AreEqual(std::string("39"), nupp["--pud"].GetString(), L"String");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetList();
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
			Hazelnupp nupp(C_Ify(args));
			nupp.SetCrashOnFail(false);

			// Verify
			const Hazelnupp* ptnupp = &nupp;

			Assert::AreEqual(39ll, nupp["--pud"].GetInt64(), L"Int64");
			Assert::AreEqual(39, nupp["--pud"].GetInt32(), L"Int32");
			Assert::IsTrue(39.5l == nupp["--pud"].GetFloat64(), L"Float64");
			Assert::AreEqual(39.5, nupp["--pud"].GetFloat32(), L"Float32");
			Assert::AreEqual(std::string("39.5"), nupp["--pud"].GetString(), L"String");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetList();
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
			Hazelnupp nupp(C_Ify(args));
			nupp.SetCrashOnFail(false);

			// Verify
			const Hazelnupp* ptnupp = &nupp;

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetInt64();
				}
			, L"Int64");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetInt32();
				}
			, L"Int32");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetFloat64();
				}
			, L"Float64");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetFloat32();
				}
			, L"Float32");

			Assert::ExpectException<HazelnuppValueNotConvertibleException>(
				[ptnupp]
				{
					(*ptnupp)["--pud"].GetString();
				}
			, L"String");

			return;
		}
	};
}
