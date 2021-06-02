#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

/// <summary>
/// Internal helper class. Feel free to use it tho, lol
/// </summary>
class StringTools
{
public:
	//! Will return wether or not a given char is in a string
	static bool Contains(const std::string& str, const char c);

	//! Will replace a part of a string with another string
	static std::string Replace(const std::string str, const char find, const std::string subst);

	//! Will replace a part of a string with another string
	static std::string Replace(const std::string str, const std::string find, const std::string subst);

	//! Will return true if the given string consists only of digits (including signage)
	static bool IsNumeric(const std::string str, const bool allowDecimalPoint = false);

	//! Will convert the number in str to a number.  
	//! Returns wether or not the operation was successful.  
	//! Also returns wether the number is an integer, or floating point. If int, cast out_number to int.
	static bool ParseNumber(const std::string str, bool& out_isInt, long double& out_number);

	//! Will split a string by a delimiter char. The delimiter will be excluded!
	static std::vector<std::string> SplitString(const std::string str, const char delimiter);

	//! Will split a string by a delimiter string. The delimiter will be excluded!
	static std::vector<std::string> SplitString(const std::string str, const std::string delimiter);

	//! Will make a string all lower-case
	static std::string ToLower(const std::string str);
};
