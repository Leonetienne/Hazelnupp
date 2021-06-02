#include "CppUnitTest.h"
#include "helper.h"
#include "../Hazelnupp/Hazelnupp.h"
#include "../Hazelnupp/HazelnuppException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
				ParamConstraint("--elenor-int", {"5994"}),
				ParamConstraint("--federich-float", {"420.69"}),
				ParamConstraint("--siegbert-string", {"banana"}),
				ParamConstraint("--lieber-liste", {"banana", "apple", "59"})
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
				ParamConstraint("--elenor-int", {"6871"}),
				ParamConstraint("--federich-float", {"-199.44"}),
				ParamConstraint("--siegbert-string", {"bornana"}),
				ParamConstraint("--lieber-liste", {"bornana", "ollpe", "5"})
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
				ParamConstraint("--num-apples", true, DATA_TYPE::INT),
				ParamConstraint("--table-height", true, DATA_TYPE::FLOAT),
				ParamConstraint("--license-plate", true, DATA_TYPE::STRING),
				ParamConstraint("--fav-fruits", true, DATA_TYPE::LIST),
				ParamConstraint("--indices", true, DATA_TYPE::LIST),
				ParamConstraint("--force", true, DATA_TYPE::VOID),
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
						ParamConstraint("--elenor-int", std::vector<std::string>(), true),
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
						ParamConstraint("--elenor-int", true, DATA_TYPE::INT),
					});

					nupp.Parse(C_Ify(args));
				}
			);

			return;
		}
	};
}
