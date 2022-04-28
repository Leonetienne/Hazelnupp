#pragma once
#include <string>

namespace Hazelnp
{
	namespace Placeholders
	{
		//! The only purpose of this is to provide the ability to return an empty string as an error for std::string& methods.
		static const std::string g_emptyString;
	}
}
