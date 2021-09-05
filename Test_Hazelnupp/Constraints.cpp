#include "CppUnitTest.h"
#include "helper.h"
#include "../Hazelnupp/CmdArgsInterface.h"
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
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterConstraint("--elenor-int", ParamConstraint::Require({ "5994" }));
			cmdArgsI.RegisterConstraint("--federich-float", ParamConstraint::Require({ "420.69" }));
			cmdArgsI.RegisterConstraint("--siegbert-string", ParamConstraint::Require({ "banana" }));
			cmdArgsI.RegisterConstraint("--lieber-liste", ParamConstraint::Require({ "banana", "apple", "59" }));

			cmdArgsI.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(cmdArgsI.HasParam("--elenor-int"));
			Assert::IsTrue(cmdArgsI["--elenor-int"].GetDataType() == DATA_TYPE::INT);
			Assert::AreEqual(cmdArgsI["--elenor-int"].GetInt32(), 5994);

			Assert::IsTrue(cmdArgsI.HasParam("--federich-float"));
			Assert::IsTrue(cmdArgsI["--federich-float"].GetDataType() == DATA_TYPE::FLOAT);
			Assert::AreEqual(cmdArgsI["--federich-float"].GetFloat32(), 420.69);

			Assert::IsTrue(cmdArgsI.HasParam("--siegbert-string"));
			Assert::IsTrue(cmdArgsI["--siegbert-string"].GetDataType() == DATA_TYPE::STRING);
			Assert::AreEqual(cmdArgsI["--siegbert-string"].GetString(), std::string("banana"));

			Assert::IsTrue(cmdArgsI.HasParam("--lieber-liste"));
			Assert::IsTrue(cmdArgsI["--lieber-liste"].GetDataType() == DATA_TYPE::LIST);
			Assert::AreEqual(cmdArgsI["--lieber-liste"].GetList()[0]->GetString(), std::string("banana"));
			Assert::AreEqual(cmdArgsI["--lieber-liste"].GetList()[1]->GetString(), std::string("apple"));
			Assert::AreEqual(cmdArgsI["--lieber-liste"].GetList()[2]->GetInt32(), 59);

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
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterConstraint("--elenor-int", ParamConstraint::Require({ "6871" }));
			cmdArgsI.RegisterConstraint("--federich-float", ParamConstraint::Require({ "-199.44" }));
			cmdArgsI.RegisterConstraint("--siegbert-string", ParamConstraint::Require({ "bornana" }));
			cmdArgsI.RegisterConstraint("--lieber-liste", ParamConstraint::Require({ "bornana", "ollpe", "5" }));

			cmdArgsI.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(cmdArgsI.HasParam("--elenor-int"));
			Assert::IsTrue(cmdArgsI["--elenor-int"].GetDataType() == DATA_TYPE::INT);
			Assert::AreEqual(cmdArgsI["--elenor-int"].GetInt32(), 5994);

			Assert::IsTrue(cmdArgsI.HasParam("--federich-float"));
			Assert::IsTrue(cmdArgsI["--federich-float"].GetDataType() == DATA_TYPE::FLOAT);
			Assert::AreEqual(cmdArgsI["--federich-float"].GetFloat32(), 420.69);

			Assert::IsTrue(cmdArgsI.HasParam("--siegbert-string"));
			Assert::IsTrue(cmdArgsI["--siegbert-string"].GetDataType() == DATA_TYPE::STRING);
			Assert::AreEqual(cmdArgsI["--siegbert-string"].GetString(), std::string("banana"));

			Assert::IsTrue(cmdArgsI.HasParam("--lieber-liste"));
			Assert::IsTrue(cmdArgsI["--lieber-liste"].GetDataType() == DATA_TYPE::LIST);
			Assert::AreEqual(cmdArgsI["--lieber-liste"].GetList()[0]->GetString(), std::string("banana"));
			Assert::AreEqual(cmdArgsI["--lieber-liste"].GetList()[1]->GetString(), std::string("apple"));
			Assert::AreEqual(cmdArgsI["--lieber-liste"].GetList()[2]->GetInt32(), 59);

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
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterConstraint("--num-apples", ParamConstraint::TypeSafety(DATA_TYPE::INT));
			cmdArgsI.RegisterConstraint("--table-height", ParamConstraint::TypeSafety(DATA_TYPE::FLOAT));
			cmdArgsI.RegisterConstraint("--license-plate", ParamConstraint::TypeSafety(DATA_TYPE::STRING));
			cmdArgsI.RegisterConstraint("--fav-fruits", ParamConstraint::TypeSafety(DATA_TYPE::LIST));
			cmdArgsI.RegisterConstraint("--indices", ParamConstraint::TypeSafety(DATA_TYPE::LIST));
			cmdArgsI.RegisterConstraint("--force", ParamConstraint::TypeSafety(DATA_TYPE::VOID));

			cmdArgsI.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(cmdArgsI.HasParam("--num-apples"));
			Assert::IsTrue(cmdArgsI["--num-apples"].GetDataType() == DATA_TYPE::INT);
			Assert::AreEqual(cmdArgsI["--num-apples"].GetInt32(), 39);

			Assert::IsTrue(cmdArgsI.HasParam("--table-height"));
			Assert::IsTrue(cmdArgsI["--table-height"].GetDataType() == DATA_TYPE::FLOAT);
			Assert::AreEqual(cmdArgsI["--table-height"].GetFloat32(), 400.0);

			Assert::IsTrue(cmdArgsI.HasParam("--license-plate"));
			Assert::IsTrue(cmdArgsI["--license-plate"].GetDataType() == DATA_TYPE::STRING);
			Assert::AreEqual(cmdArgsI["--license-plate"].GetString(), std::string("193273"));

			Assert::IsTrue(cmdArgsI.HasParam("--fav-fruits"));
			Assert::IsTrue(cmdArgsI["--fav-fruits"].GetDataType() == DATA_TYPE::LIST);
			Assert::AreEqual(cmdArgsI["--fav-fruits"].GetList()[0]->GetString(), std::string("apple"));

			Assert::IsTrue(cmdArgsI.HasParam("--indices"));
			Assert::IsTrue(cmdArgsI["--indices"].GetDataType() == DATA_TYPE::LIST);
			Assert::AreEqual(cmdArgsI["--indices"].GetList()[0]->GetInt32(), 9);

			Assert::IsTrue(cmdArgsI.HasParam("--force"));
			Assert::IsTrue(cmdArgsI["--force"].GetDataType() == DATA_TYPE::VOID);

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
					CmdArgsInterface cmdArgsI;
					cmdArgsI.SetCrashOnFail(false);

					cmdArgsI.RegisterConstraint(
						"--elenor-int",
						ParamConstraint::Require()
					);

					cmdArgsI.Parse(C_Ify(args));
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
					CmdArgsInterface cmdArgsI;
					cmdArgsI.SetCrashOnFail(false);

					cmdArgsI.RegisterConstraint(
						"--elenor-int",
						ParamConstraint::TypeSafety(DATA_TYPE::INT)
					);

					cmdArgsI.Parse(C_Ify(args));
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

			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterConstraint("--void1", ParamConstraint::TypeSafety(DATA_TYPE::VOID));
			cmdArgsI.RegisterConstraint("--void2", ParamConstraint::TypeSafety(DATA_TYPE::VOID));
			cmdArgsI.RegisterConstraint("--void3", ParamConstraint::TypeSafety(DATA_TYPE::VOID));
			cmdArgsI.RegisterConstraint("--void4", ParamConstraint::TypeSafety(DATA_TYPE::VOID));
			cmdArgsI.RegisterConstraint("--void5", ParamConstraint::TypeSafety(DATA_TYPE::VOID));


			// Exercise
			cmdArgsI.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(cmdArgsI["--void1"].GetDataType() == DATA_TYPE::VOID);
			Assert::IsTrue(cmdArgsI["--void2"].GetDataType() == DATA_TYPE::VOID);
			Assert::IsTrue(cmdArgsI["--void3"].GetDataType() == DATA_TYPE::VOID);
			Assert::IsTrue(cmdArgsI["--void4"].GetDataType() == DATA_TYPE::VOID);
			Assert::IsTrue(cmdArgsI["--void5"].GetDataType() == DATA_TYPE::VOID);

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

			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterConstraint(
				"--empty-list",
				ParamConstraint::TypeSafety(DATA_TYPE::LIST)
			);


			// Exercise
			cmdArgsI.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(cmdArgsI["--empty-list"].GetDataType() == DATA_TYPE::LIST, L"Wrong datatype");
			Assert::AreEqual(std::size_t(0), cmdArgsI["--empty-list"].GetList().size(), L"Wrong size");

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

			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterConstraint(
				"--empty-string",
				ParamConstraint::TypeSafety(DATA_TYPE::STRING)
			);


			// Exercise
			cmdArgsI.Parse(C_Ify(args));

			// Verify
			Assert::IsTrue(cmdArgsI["--empty-string"].GetDataType() == DATA_TYPE::STRING, L"Wrong datatype");
			Assert::AreEqual(std::size_t(0), cmdArgsI["--empty-string"].GetString().length(), L"Wrong size");

			return;
		}

		// Tests that parsing a void as int fails
		TEST_METHOD(Weird_Load_Conversions_VoidFailsAsInt)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--thisisvoid",
				});

			// Test section: INT
			Assert::ExpectException<HazelnuppConstraintTypeMissmatch>([args]
				{
					CmdArgsInterface cmdArgsI;
					cmdArgsI.SetCrashOnFail(false);

					cmdArgsI.RegisterConstraint(
						"--thisisvoid",
						ParamConstraint::TypeSafety(DATA_TYPE::INT)
					);

					// Exercise
					cmdArgsI.Parse(C_Ify(args));

				}, L"Failed with int");

			return;
		}

		// Tests that parsing a void as float fails
		TEST_METHOD(Weird_Load_Conversions_VoidFailsAsFloat)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--thisisvoid",
				});

			// Test section: FLOAT
			Assert::ExpectException<HazelnuppConstraintTypeMissmatch>([args]
				{
					CmdArgsInterface cmdArgsI;
					cmdArgsI.SetCrashOnFail(false);

					cmdArgsI.RegisterConstraint(
						"--thisisvoid",
						ParamConstraint::TypeSafety(DATA_TYPE::FLOAT)
					);

					// Exercise
					cmdArgsI.Parse(C_Ify(args));

				}, L"Failed with float");

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

			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterConstraint(
				"--default-val",
				ParamConstraint::Require({ "32" }, true)
			);

			cmdArgsI.RegisterConstraint(
				"--not-there",
				ParamConstraint::Require({}, true)
			);

			// Exercise
			cmdArgsI.ClearConstraint("--not-there");


			// Verify
			cmdArgsI.Parse(C_Ify(args));

			// Also verifies that parse does not throw an exception for --not-there

			Assert::IsTrue(cmdArgsI.HasParam("--default-val"), L"Default value is missing");
			Assert::AreEqual(32, cmdArgsI["--default-val"].GetInt32(), L"Default value has wrong value");

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

			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterConstraint(
				"--not-there",
				ParamConstraint::Require({}, true)
			);

			// Exercise
			cmdArgsI.ClearConstraints();

			// Verify
			cmdArgsI.Parse(C_Ify(args));

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

			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			cmdArgsI.RegisterConstraint(
				"--not-there",
				ParamConstraint::Require({}, true)
			);

			//Exercise
			cmdArgsI.RegisterConstraint(
				"--not-there",
				ParamConstraint::Require({}, false)
			);

			// Verify
			cmdArgsI.Parse(C_Ify(args));

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

			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			ParamConstraint dftvalConst_expected =
				ParamConstraint::Require({ "32" }, true);

			cmdArgsI.RegisterConstraint("--default-val", dftvalConst_expected);
			cmdArgsI.RegisterConstraint("--not-there", ParamConstraint::Require({}, true));
			cmdArgsI.RegisterConstraint("--another-one", ParamConstraint::Require({ "bites" }, true));

			// Exercise
			ParamConstraint dftvalConst = cmdArgsI.GetConstraint("--default-val");

			// Verify
			Assert::IsTrue(dftvalConst_expected.required == dftvalConst.required, L"required");
			Assert::IsTrue(dftvalConst_expected.defaultValue == dftvalConst.defaultValue, L"defaultValue");
			Assert::IsTrue(dftvalConst_expected.requiredType == dftvalConst.requiredType, L"requiredType");
			Assert::IsTrue(dftvalConst_expected.constrainType == dftvalConst.constrainType, L"constrainType");

			return;
		}

		// Tests that an HazelnuppConstraintIncompatibleParameters gets raised if a required parameter
		// is incompatible with another parameter passed alongside
		// This test will use the single-string-to-initializer-list proxy method
		TEST_METHOD(Exception_Constraint_Incompatible_Parameters_ProxyMethod)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--make-background-glow",
				"--make-background-transparent",
			});

			Assert::ExpectException<HazelnuppConstraintIncompatibleParameters>(
				[args]
				{
					CmdArgsInterface cmdArgsI;
					cmdArgsI.SetCrashOnFail(false);

					cmdArgsI.RegisterConstraint(
						"--make-background-glow",
						ParamConstraint::Incompatibility("--make-background-transparent")
					);

					cmdArgsI.Parse(C_Ify(args));
				}
				);

			return;
		}

		// Tests that an HazelnuppConstraintIncompatibleParameters gets raised if a required parameter
		// is incompatible with another parameter passed alongside
		TEST_METHOD(Exception_Constraint_Incompatible_Parameters)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--make-background-glow",
				"--make-background-transparent",
			});

			Assert::ExpectException<HazelnuppConstraintIncompatibleParameters>(
				[args]
				{
					CmdArgsInterface cmdArgsI;
					cmdArgsI.SetCrashOnFail(false);

					cmdArgsI.RegisterConstraint(
						"--make-background-glow",
						ParamConstraint::Incompatibility({"--make-background-transparent"})
					);

					cmdArgsI.Parse(C_Ify(args));
				}
				);

			return;
		}

		// Tests that an HazelnuppConstraintIncompatibleParameters gets raised if a required parameter
		// is incompatible with another parameter passed alongside
		// This test will register multiple incompatibilities
		TEST_METHOD(Exception_Constraint_Incompatible_Parameters_Multiple_Incompatibilities)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--make-background-blue",
				"--make-background-transparent",
				"--make-background-glow",
				"--make-background-green",
			});

			Assert::ExpectException<HazelnuppConstraintIncompatibleParameters>(
				[args]
				{
					CmdArgsInterface cmdArgsI;
					cmdArgsI.SetCrashOnFail(false);

					cmdArgsI.RegisterConstraint(
						"--make-background-glow",
						ParamConstraint::Incompatibility({
							"--make-background-transparent",
							"--make-background-green",
							"--make-background-blue",
							})
					);

					cmdArgsI.Parse(C_Ify(args));
				}
				);

			return;
		}

		// Tests that an HazelnuppConstraintIncompatibleParameters is NOT raised, if incompatible parameters
		// are NOT supplied
		TEST_METHOD(Constraint_Incompatible_Parameters_Are_Not_Incompatible)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--make-background-glow",
				"--set-food-delicious",
			});

			{
				CmdArgsInterface cmdArgsI;
				cmdArgsI.SetCrashOnFail(false);

				cmdArgsI.RegisterConstraint(
					"--make-background-glow",
					ParamConstraint::Incompatibility({ "--make-background-transparent" })
				);

				cmdArgsI.RegisterConstraint(
					"--set-food-delicious",
					ParamConstraint::Incompatibility({ "--set-food-disgusting" })
				);

				cmdArgsI.Parse(C_Ify(args));
			}

			return;
		}

		// Tests that an HazelnuppConstraintIncompatibleParameters is NOT raised, if the incompatible
		// paremter IS supplied, but not the one attached to the constraint
		TEST_METHOD(Constraint_Incompatible_Parameters_Are_Not_Incompatible_Constrained_Parameter_Not_Passed)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--make-background-transparent",
				"--set-food-delicious",
				});

			{
				CmdArgsInterface cmdArgsI;
				cmdArgsI.SetCrashOnFail(false);

				cmdArgsI.RegisterConstraint(
					"--make-background-glow",
					ParamConstraint::Incompatibility({ "--make-background-transparent" })
				);

				cmdArgsI.Parse(C_Ify(args));
			}

			return;
		}

		// Tests that daisychained construction of constraints works. Tests for the require constraint to work
		TEST_METHOD(Daisychained_Construction_Test_Require)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
			});

			Assert::ExpectException<HazelnuppConstraintMissingValue>(
				[args]
				{
					CmdArgsInterface cmdArgsI;
					cmdArgsI.SetCrashOnFail(false);

					cmdArgsI.RegisterConstraint(
						"--width",
						ParamConstraint::Require()
						.AddTypeSafety(DATA_TYPE::FLOAT)
						.AddIncompatibilities({ "--antiwidth" })
					);

					cmdArgsI.Parse(C_Ify(args));
				}
			);

			return;
		}

		// Tests that daisychained construction of constraints works. Tests for the type safety constraint to work
		TEST_METHOD(Daisychained_Construction_Test_TypeSafety)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--width",
				"alejandro"
			});

			Assert::ExpectException<HazelnuppConstraintTypeMissmatch>(
				[args]
				{
					CmdArgsInterface cmdArgsI;
					cmdArgsI.SetCrashOnFail(false);

					cmdArgsI.RegisterConstraint(
						"--width",
						ParamConstraint::TypeSafety(DATA_TYPE::FLOAT)
						.AddRequire()
						.AddIncompatibilities({ "--antiwidth" })
					);

					cmdArgsI.Parse(C_Ify(args));
				}
				);

			return;
		}

		// Tests that daisychained construction of constraints works. Tests for the incompatibility constraint to work
		TEST_METHOD(Daisychained_Construction_Test_Incompatibility)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--width",
				"8930",
				"--antiwidth"
			});

			Assert::ExpectException<HazelnuppConstraintIncompatibleParameters>(
				[args]
				{
					CmdArgsInterface cmdArgsI;
					cmdArgsI.SetCrashOnFail(false);

					cmdArgsI.RegisterConstraint(
						"--width",
						ParamConstraint::Incompatibility({ "--antiwidth" })
						.AddTypeSafety(DATA_TYPE::FLOAT)
						.AddRequire()
					);

					cmdArgsI.Parse(C_Ify(args));
				}
			);

			return;
		}

		// Tests that daisychained construction of constraints works. Tests for the command to pass, because all constraints should be respected
		TEST_METHOD(Daisychained_Construction_Test_AllOkay)
		{
			// Setup
			ArgList args({
				"/my/fake/path/wahoo.out",
				"--width",
				"8930"
			});

			{
				CmdArgsInterface cmdArgsI;
				cmdArgsI.SetCrashOnFail(false);

				cmdArgsI.RegisterConstraint(
					"--width",
					ParamConstraint::Require()
					.AddTypeSafety(DATA_TYPE::FLOAT)
					.AddIncompatibilities({ "--antiwidth" })
				);

				cmdArgsI.Parse(C_Ify(args));
			}

			return;
		}
	};
}
