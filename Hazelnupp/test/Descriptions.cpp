#include "Catch2.h"
#include "helper.h"
#include <Hazelnupp/CmdArgsInterface.h>
#include <Hazelnupp/HazelnuppException.h>

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
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			std::string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse in quam tincidunt sapien euismod egestas eget vel dolor. Duis non turpis porttitor, convallis velit at.";

			// Exercise
			cmdArgsI.SetBriefDescription(text);

			// Verify
			Assert::AreEqual(text, cmdArgsI.GetBriefDescription());

			return;
		}

		// Tests that a parameters description can be set and returned
		TEST_METHOD(Can_Set_Parameter_Description)
		{
			// Setup
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			// Exercise
			cmdArgsI.RegisterDescription("--force", "Just force it");
			cmdArgsI.RegisterDescription("--lose", "Just lose it");
			cmdArgsI.RegisterDescription("--crazy", "Go crazy");

			// Verify
			Assert::AreEqual(std::string("Just force it"), cmdArgsI.GetDescription("--force"));
			Assert::AreEqual(std::string("Go crazy"), cmdArgsI.GetDescription("--crazy"));
			Assert::AreEqual(std::string("Just lose it"), cmdArgsI.GetDescription("--lose"));

			return;
		}

		//! Tests that HasDescription works
		TEST_METHOD(Has_Description)
		{
			// Setup
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			// Exercise, verify
			Assert::IsFalse(cmdArgsI.HasDescription("--force"));
			Assert::IsFalse(cmdArgsI.HasDescription("--main"));

			cmdArgsI.RegisterDescription("--force", "Just force it");

			Assert::IsTrue(cmdArgsI.HasDescription("--force"));
			Assert::IsFalse(cmdArgsI.HasDescription("--main"));

			return;
		}

		// Tests that an unknown parameter returns an empty string
		TEST_METHOD(No_Description_Is_Empty_String)
		{
			// Setup
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			// Exercise
			cmdArgsI.RegisterDescription("--force", "Just force it");
			cmdArgsI.RegisterDescription("--lose", "Just lose it");
			cmdArgsI.RegisterDescription("--crazy", "Go crazy");

			// Verify
			Assert::AreEqual(std::string(), cmdArgsI.GetDescription("--man"));

			return;
		}

		// Tests that a description can be deleted
		TEST_METHOD(ClearDescription)
		{
			// Setup
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			// Exercise
			cmdArgsI.RegisterDescription("--force", "Just force it");
			cmdArgsI.RegisterDescription("--lose", "Just lose it");
			cmdArgsI.RegisterDescription("--crazy", "Go crazy");

			cmdArgsI.ClearDescription("--lose");

			// Verify
			
			// These two should still work
			Assert::AreEqual(std::string("Just force it"), cmdArgsI.GetDescription("--force"));
			Assert::AreEqual(std::string("Go crazy"), cmdArgsI.GetDescription("--crazy"));
			
			Assert::IsFalse(cmdArgsI.HasDescription("--lose"));

			return;
		}

		// Tests that all descriptions can be deleted at once
		TEST_METHOD(ClearDescriptions)
		{
			// Setup
			CmdArgsInterface cmdArgsI;
			cmdArgsI.SetCrashOnFail(false);

			// Exercise
			cmdArgsI.RegisterDescription("--force", "Just force it");
			cmdArgsI.RegisterDescription("--lose", "Just lose it");
			cmdArgsI.RegisterDescription("--crazy", "Go crazy");

			cmdArgsI.ClearDescriptions();

			// Verify

			// These two should still work
			Assert::IsFalse(cmdArgsI.HasDescription("--force"));
			Assert::IsFalse(cmdArgsI.HasDescription("--crazy"));
			Assert::IsFalse(cmdArgsI.HasDescription("--lose"));

			return;
		}
	};
}
