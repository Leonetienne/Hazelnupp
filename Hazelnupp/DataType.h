#pragma once
#include <string>

namespace Hazelnp
{
	/** The different data types a paramater can be
	*/
	enum class DATA_TYPE
	{
		VOID,
		INT,
		FLOAT,
		STRING,
		LIST
	};

	static inline std::string DataTypeToString(DATA_TYPE type)
	{
		switch (type)
		{
		case DATA_TYPE::VOID:
			return "VOID";

		case DATA_TYPE::INT:
			return "INT";

		case DATA_TYPE::FLOAT:
			return "FLOAT";

		case DATA_TYPE::STRING:
			return "STRING";

		case DATA_TYPE::LIST:
			return "LIST";
		}

		return "";
	}
}
