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

			nupp.RegisterConstraint("--elenor-int", ParamConstraint::Require({"5994"}));
			nupp.RegisterConstraint("--federich-float", ParamConstraint::Require({ "420.69" }));
			nupp.RegisterConstraint("--siegbert-string", ParamConstraint::Require({"banana"}));
			nupp.RegisterConstraint("--lieber-liste", ParamConstraint::Require({ "banana", "apple", "59" }));

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

			nupp.RegisterConstraint("--elenor-int", ParamConstraint::Require({ "6871" }));
			nupp.RegisterConstraint("--federich-float", ParamConstraint::Require({"-199.44"}));
			nupp.RegisterConstraint("--siegbert-string", ParamConstraint::Require({"bornana"}));
			nupp.RegisterConstraint("--lieber-liste", ParamConstraint::Require({"bornana", "ollpe", "5"}));

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

			nupp.RegisterConstraint("--num-apples", ParamConstraint::TypeSafety(DATA_TYPE::INT));
			nupp.RegisterConstraint("--table-height", ParamConstraint::TypeSafety(DATA_TYPE::FLOAT));
			nupp.RegisterConstraint("--license-plate", ParamConstraint::TypeSafety(DATA_TYPE::STRING));
			nupp.RegisterConstraint("--fav-fruits", ParamConstraint::TypeSafety(DATA_TYPE::LIST));
			nupp.RegisterConstraint("--indices", ParamConstraint::TypeSafety(DATA_TYPE::LIST));
			nupp.RegisterConstraint("--force", ParamConstraint::TypeSafety(DATA_TYPE::VOID));

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

					nupp.RegisterConstraint(
						"--elenor-int",
						ParamConstraint::Require()
					);

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

					nupp.RegisterConstraint(
						"--elenor-int",
						ParamConstraint::TypeSafety( DATA_TYPE::INT)
					);

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

			nupp.RegisterConstraint("--void1", ParamConstraint::TypeSafety(DATA_TYPE::VOID));
			nupp.RegisterConstraint("--void2", ParamConstraint::TypeSafety(DATA_TYPE::VOID));
			nupp.RegisterConstraint("--void3", ParamConstraint::TypeSafety(DATA_TYPE::VOID));
			nupp.RegisterConstraint("--void4", ParamConstraint::TypeSafety(DATA_TYPE::VOID));
			nupp.RegisterConstraint("--void5", ParamConstraint::TypeSafety(DATA_TYPE::VOID));


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

		// Tests that a void can be converted to an empty list
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

			nupp.RegisterConstraint(
				"--empty-list",
				ParamConstraint::TypeSafety(DATA_TYPE::LIST)
			);


			// Exercise
			nupp.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(nupp["--empty-list"].GetDataType() == DATA_TYPE::LIST, L"Wrong datatype");
			Assert::AreEqual(std::size_t(0), nupp["--empty-list"].GetList().size(), L"Wrong size");

			return;
		}

		// Tests that a void can be converted to an empty string
		TEST_METHOD(Weird_Load_Conversions_VoidToEmptyString)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--dummy",
				"--empty-string",
			});

			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterConstraint(
				"--empty-string",
				ParamConstraint::TypeSafety(DATA_TYPE::STRING)
			);


			// Exercise
			nupp.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(nupp["--empty-string"].GetDataType() == DATA_TYPE::STRING, L"Wrong datatype");
			Assert::AreEqual(std::size_t(0), nupp["--empty-string"].GetString().length(), L"Wrong size");

			return;
		}

		// Tests that constraints can be cleared invidivually
		TEST_METHOD(Can_Clear_Constraints_Individually)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--dummy",
				"--empty-list",
			});

			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterConstraint(
				"--default-val",
				ParamConstraint::Require({"32"}, true)
			);

			nupp.RegisterConstraint(
				"--not-there",
				ParamConstraint::Require({}, true)
			);

			// Exercise
			nupp.ClearConstraint("--not-there");


			// Verify
			nupp.Parse(C_Ify(args));

			// Also verifies that parse does not throw an exception for --not-there

			Assert::IsTrue(nupp.HasParam("--default-val"), L"Default value is missing");
			Assert::AreEqual(32, nupp["--default-val"].GetInt32(), L"Default value has wrong value");

			return;
		}

		// Tests that constraints can be cleared invidivually
		TEST_METHOD(Can_Clear_All_Constraints)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--dummy",
				"--empty-list",
			});

			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterConstraint(
				"--not-there",
				ParamConstraint::Require({}, true)
			);

			// Exercise
			nupp.ClearConstraints();

			// Verify
			nupp.Parse(C_Ify(args));

			return;
		}

		// Tests that setting a constraint for a parameter again will overwrite the existing one
		TEST_METHOD(Can_Override_Constraints)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--dummy",
				"--empty-list",
			});

			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			nupp.RegisterConstraint(
				"--not-there",
				ParamConstraint::Require({}, true)
			);

			//Exercise
			nupp.RegisterConstraint(
				"--not-there",
				ParamConstraint::Require({}, false)
			);

			// Verify
			nupp.Parse(C_Ify(args));

			return;
		}

		// Tests that the GetConstraint method returns the correct constraint information
		TEST_METHOD(Get_Constraint)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--dummy",
				"--empty-list",
			});

			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			ParamConstraint dftvalConst_expected = 
				ParamConstraint::Require({"32"}, true);
	
			nupp.RegisterConstraint("--default-val", dftvalConst_expected);
			nupp.RegisterConstraint("--not-there", ParamConstraint::Require({}, true));
			nupp.RegisterConstraint("--another-one", ParamConstraint::Require({ "bites" }, true));

			// Exercise
			ParamConstraint dftvalConst = nupp.GetConstraint("--default-val");

			// Verify
			Assert::IsTrue(dftvalConst_expected.required == dftvalConst.required, L"required");
			Assert::IsTrue(dftvalConst_expected.defaultValue == dftvalConst.defaultValue, L"defaultValue");
			Assert::IsTrue(dftvalConst_expected.wantedType == dftvalConst.wantedType, L"wantedType");
			Assert::IsTrue(dftvalConst_expected.constrainType == dftvalConst.constrainType, L"constrainType");

			return;
		}
	};
}
