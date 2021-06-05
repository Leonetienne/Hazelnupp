#include "CppUnitTest.h"
#include "helper.h"
#include "../Hazelnupp/Hazelnupp.h"
#include "../Hazelnupp/HazelnuppException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Hazelnp;

namespace TestHazelnupp
{
	TEST_CLASS(_Descriptions)
	{
	public:

		// Tests that the application description can be set and returned
		TEST_METHOD(Set_Get_Application_Brief)
		{
			// Setup
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			std::string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse in quam tincidunt sapien euismod egestas eget vel dolor. Duis non turpis porttitor, convallis velit at.";

			// Exercise
			nupp.SetBriefDescription(text);

			// Verify
			Assert::AreEqual(text, nupp.GetBriefDescription());

			return;
		}

		// Tests that a parameters description can be set and returned
		TEST_METHOD(Can_Set_Parameter_Description)
		{
			// Setup
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			// Exercise
			nupp.RegisterDescription("--force", "Just force it");
			nupp.RegisterDescription("--lose", "Just lose it");
			nupp.RegisterDescription("--crazy", "Go crazy");

			// Verify
			Assert::AreEqual(std::string("Just force it"), nupp.GetDescription("--force"));
			Assert::AreEqual(std::string("Go crazy"), nupp.GetDescription("--crazy"));
			Assert::AreEqual(std::string("Just lose it"), nupp.GetDescription("--lose"));

			return;
		}

		//! Tests that HasDescription works
		TEST_METHOD(Has_Description)
		{
			// Setup
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			// Exercise, verify
			Assert::IsFalse(nupp.HasDescription("--force"));
			Assert::IsFalse(nupp.HasDescription("--main"));

			nupp.RegisterDescription("--force", "Just force it");

			Assert::IsTrue(nupp.HasDescription("--force"));
			Assert::IsFalse(nupp.HasDescription("--main"));

			return;
		}

		// Tests that an unknown parameter returns an empty string
		TEST_METHOD(No_Description_Is_Empty_String)
		{
			// Setup
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			// Exercise
			nupp.RegisterDescription("--force", "Just force it");
			nupp.RegisterDescription("--lose", "Just lose it");
			nupp.RegisterDescription("--crazy", "Go crazy");

			// Verify
			Assert::AreEqual(std::string(), nupp.GetDescription("--man"));

			return;
		}

		// Tests that a description can be deleted
		TEST_METHOD(ClearDescription)
		{
			// Setup
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			// Exercise
			nupp.RegisterDescription("--force", "Just force it");
			nupp.RegisterDescription("--lose", "Just lose it");
			nupp.RegisterDescription("--crazy", "Go crazy");

			nupp.ClearDescription("--lose");

			// Verify
			
			// These two should still work
			Assert::AreEqual(std::string("Just force it"), nupp.GetDescription("--force"));
			Assert::AreEqual(std::string("Go crazy"), nupp.GetDescription("--crazy"));
			
			Assert::IsFalse(nupp.HasDescription("--lose"));

			return;
		}

		// Tests that all descriptions can be deleted at once
		TEST_METHOD(ClearDescriptions)
		{
			// Setup
			Hazelnupp nupp;
			nupp.SetCrashOnFail(false);

			// Exercise
			nupp.RegisterDescription("--force", "Just force it");
			nupp.RegisterDescription("--lose", "Just lose it");
			nupp.RegisterDescription("--crazy", "Go crazy");

			nupp.ClearDescriptions();

			// Verify

			// These two should still work
			Assert::IsFalse(nupp.HasDescription("--force"));
			Assert::IsFalse(nupp.HasDescription("--crazy"));
			Assert::IsFalse(nupp.HasDescription("--lose"));

			return;
		}
	};
}
