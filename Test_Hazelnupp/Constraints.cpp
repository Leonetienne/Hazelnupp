#include "CppUnitTest.h"
#include "helper.h"
#include "../Hazelnupp/Hazelnupp.h"
#include "../Hazelnupp/HazelnuppException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Hazelnp;

namespace TestHazelnupp
{
	TEST_CLASS(_Constraints)
	{
	public:

		// Tests that default values get added
		TEST_METHOD(DefaultValues_GetAdded)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--dummy",
				"123"
			});

			// Exercise
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterConstraints({
				ParamConstraint::Require("--elenor-int", {"5994"}),
				ParamConstraint::Require("--federich-float", {"420.69"}),
				ParamConstraint::Require("--siegbert-string", {"banana"}),
				ParamConstraint::Require("--lieber-liste", {"banana", "apple", "59"})
			});

			nupp.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(nupp.HasParam("--elenor-int"));
			Assert::IsTrue(nupp["--elenor-int"].GetDataType() == DATA_TYPE::INT);
			Assert::AreEqual(nupp["--elenor-int"].GetInt32(), 5994);

			Assert::IsTrue(nupp.HasParam("--federich-float"));
			Assert::IsTrue(nupp["--federich-float"].GetDataType() == DATA_TYPE::FLOAT);
			Assert::AreEqual(nupp["--federich-float"].GetFloat32(), 420.69);

			Assert::IsTrue(nupp.HasParam("--siegbert-string"));
			Assert::IsTrue(nupp["--siegbert-string"].GetDataType() == DATA_TYPE::STRING);
			Assert::AreEqual(nupp["--siegbert-string"].GetString(), std::string("banana"));

			Assert::IsTrue(nupp.HasParam("--lieber-liste"));
			Assert::IsTrue(nupp["--lieber-liste"].GetDataType() == DATA_TYPE::LIST);
			Assert::AreEqual(nupp["--lieber-liste"].GetList()[0]->GetString(), std::string("banana"));
			Assert::AreEqual(nupp["--lieber-liste"].GetList()[1]->GetString(), std::string("apple"));
			Assert::AreEqual(nupp["--lieber-liste"].GetList()[2]->GetInt32(), 59);

			return;
		}

		// Tests that the default values do not override actually set values
		TEST_METHOD(DefaultValues_DefaultDoesntOverride)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--dummy",
				"--elenor-int",
				"5994",
				"--federich-float",
				"420.69",
				"--siegbert-string",
				"banana",
				"--lieber-liste",
				"banana",
				"apple",
				"59"
			});

			// Exercise
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterConstraints({
				ParamConstraint::Require("--elenor-int", {"6871"}),
				ParamConstraint::Require("--federich-float", {"-199.44"}),
				ParamConstraint::Require("--siegbert-string", {"bornana"}),
				ParamConstraint::Require("--lieber-liste", {"bornana", "ollpe", "5"})
			});

			nupp.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(nupp.HasParam("--elenor-int"));
			Assert::IsTrue(nupp["--elenor-int"].GetDataType() == DATA_TYPE::INT);
			Assert::AreEqual(nupp["--elenor-int"].GetInt32(), 5994);

			Assert::IsTrue(nupp.HasParam("--federich-float"));
			Assert::IsTrue(nupp["--federich-float"].GetDataType() == DATA_TYPE::FLOAT);
			Assert::AreEqual(nupp["--federich-float"].GetFloat32(), 420.69);

			Assert::IsTrue(nupp.HasParam("--siegbert-string"));
			Assert::IsTrue(nupp["--siegbert-string"].GetDataType() == DATA_TYPE::STRING);
			Assert::AreEqual(nupp["--siegbert-string"].GetString(), std::string("banana"));

			Assert::IsTrue(nupp.HasParam("--lieber-liste"));
			Assert::IsTrue(nupp["--lieber-liste"].GetDataType() == DATA_TYPE::LIST);
			Assert::AreEqual(nupp["--lieber-liste"].GetList()[0]->GetString(), std::string("banana"));
			Assert::AreEqual(nupp["--lieber-liste"].GetList()[1]->GetString(), std::string("apple"));
			Assert::AreEqual(nupp["--lieber-liste"].GetList()[2]->GetInt32(), 59);

			return;
		}

		// Tests that data types get forced according to the constraints
		TEST_METHOD(ForceTypes)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--dummy",
				"--num-apples",
				"39.75",
				"--table-height",
				"400",
				"--license-plate",
				"193273",
				"--fav-fruits",
				"apple",
				"--indices",
				"9",
				"--force",
				"plsdontuseme"
			});

			// Exercise
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterConstraints({
				ParamConstraint::TypeSafety("--num-apples", DATA_TYPE::INT),
				ParamConstraint::TypeSafety("--table-height", DATA_TYPE::FLOAT),
				ParamConstraint::TypeSafety("--license-plate", DATA_TYPE::STRING),
				ParamConstraint::TypeSafety("--fav-fruits", DATA_TYPE::LIST),
				ParamConstraint::TypeSafety("--indices", DATA_TYPE::LIST),
				ParamConstraint::TypeSafety("--force", DATA_TYPE::VOID),
			});

			nupp.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(nupp.HasParam("--num-apples"));
			Assert::IsTrue(nupp["--num-apples"].GetDataType() == DATA_TYPE::INT);
			Assert::AreEqual(nupp["--num-apples"].GetInt32(), 39);

			Assert::IsTrue(nupp.HasParam("--table-height"));
			Assert::IsTrue(nupp["--table-height"].GetDataType() == DATA_TYPE::FLOAT);
			Assert::AreEqual(nupp["--table-height"].GetFloat32(), 400.0);

			Assert::IsTrue(nupp.HasParam("--license-plate"));
			Assert::IsTrue(nupp["--license-plate"].GetDataType() == DATA_TYPE::STRING);
			Assert::AreEqual(nupp["--license-plate"].GetString(), std::string("193273"));

			Assert::IsTrue(nupp.HasParam("--fav-fruits"));
			Assert::IsTrue(nupp["--fav-fruits"].GetDataType() == DATA_TYPE::LIST);
			Assert::AreEqual(nupp["--fav-fruits"].GetList()[0]->GetString(), std::string("apple"));

			Assert::IsTrue(nupp.HasParam("--indices"));
			Assert::IsTrue(nupp["--indices"].GetDataType() == DATA_TYPE::LIST);
			Assert::AreEqual(nupp["--indices"].GetList()[0]->GetInt32(), 9);

			Assert::IsTrue(nupp.HasParam("--force"));
			Assert::IsTrue(nupp["--force"].GetDataType() == DATA_TYPE::VOID);

			return;
		}

		// Tests that an HazelnuppConstraintMissingValue gets raised if a required parameter
		// is missing and does not have a default parameter
		TEST_METHOD(Exception_MissingImportant_Parameter_WithoutDefault)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--dummy",
				"--federich-float",
				"420.69",
				"--siegbert-string",
				"banana",
				"--lieber-liste",
				"banana",
				"apple",
				"59"
			});

			Assert::ExpectException<HazelnuppConstraintMissingValue>(
				[args]
				{
					Hazelnupp nupp;
					nupp.SetCrashOnFail(false);

					nupp.RegisterConstraints({
						ParamConstraint::Require("--elenor-int"),
					});

					nupp.Parse(C_Ify(args));
				}
			);

			return;
		}

		// Tests that an HazelnuppConstraintTypeMissmatch gets raised if a required parameter
		// is missing of the wrong type and cannot be converted
		TEST_METHOD(Exception_TypeMismatch_Parameter_NotConvertable)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--dummy",
				"--elenor-int",
				"hello"
				"--federich-float",
				"420.69",
				"--siegbert-string",
				"banana",
				"--lieber-liste",
				"banana",
				"apple",
				"59"
			});

			Assert::ExpectException<HazelnuppConstraintTypeMissmatch>(
				[args]
				{
					Hazelnupp nupp;
					nupp.SetCrashOnFail(false);

					nupp.RegisterConstraints({
						ParamConstraint::TypeSafety("--elenor-int", DATA_TYPE::INT),
					});

					nupp.Parse(C_Ify(args));
				}
			);

			return;
		}

		// Tests that everything can be converted to void
		TEST_METHOD(Weird_Load_Conversions_ToVoid)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--dummy",
				"--void1",
				"--void2",
				"12",
				"--void3",
				"9.5",
				"--void4",
				"hello",
				"--void5",
				"foo",
				"baz"
			});

			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterConstraints({
				ParamConstraint::TypeSafety("--void1", DATA_TYPE::VOID),
				ParamConstraint::TypeSafety("--void2", DATA_TYPE::VOID),
				ParamConstraint::TypeSafety("--void3", DATA_TYPE::VOID),
				ParamConstraint::TypeSafety("--void4", DATA_TYPE::VOID),
				ParamConstraint::TypeSafety("--void5", DATA_TYPE::VOID)
			});


			// Exercise
			nupp.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(nupp["--void1"].GetDataType() == DATA_TYPE::VOID);
			Assert::IsTrue(nupp["--void2"].GetDataType() == DATA_TYPE::VOID);
			Assert::IsTrue(nupp["--void3"].GetDataType() == DATA_TYPE::VOID);
			Assert::IsTrue(nupp["--void4"].GetDataType() == DATA_TYPE::VOID);
			Assert::IsTrue(nupp["--void5"].GetDataType() == DATA_TYPE::VOID);

			return;
		}

		// Tests that everything a void can be converted to an empty list
		TEST_METHOD(Weird_Load_Conversions_VoidToEmptyList)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--dummy",
				"--empty-list",
			});

			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterConstraints({
				ParamConstraint::TypeSafety("--empty-list", DATA_TYPE::LIST),
			});


			// Exercise
			nupp.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(nupp["--empty-list"].GetDataType() == DATA_TYPE::LIST);
			Assert::AreEqual(std::size_t(0), nupp["--empty-list"].GetList().size());

			return;
		}
	};
}
