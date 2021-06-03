#include "StringTools.h"

using namespace Hazelnp;

bool StringTools::Contains(const std::string& str, const char c)
{
    for (const char& i : str)
        if (i == c)
            return true;

    return false;
}

std::string StringTools::Replace(const std::string& str, const char find, const std::string& subst)
{
    std::stringstream ss;

    for (std::size_t i = 0; i < str.length(); i++)
    {
        if (str[i] != find) ss << str[i];
        else ss << subst;
    }

    return ss.str();
}

std::string StringTools::Replace(const std::string& str, const std::string& find, const std::string& subst)
{
    if (find.length() == 0) return str;

    std::stringstream ss;

    std::size_t posFound = 0;
    std::size_t lastFound = 0;

    while (posFound != std::string::npos)
    {
        lastFound = posFound;
        posFound = str.find(find, posFound);

        if (posFound != std::string::npos)
        {
            ss << str.substr(lastFound, posFound - lastFound) << subst;
            posFound += find.length();
        }
        else
        {
            ss << str.substr(lastFound, (str.length()) - lastFound);
        }
    }

    return ss.str();
}


bool StringTools::IsNumeric(const std::string& str, const bool allowDecimalPoint)
{
    if (str.length() == 0) return false;

    bool alreadyParsedDecimalPoint = false;
    std::size_t digitCount = 0;

    for (std::size_t i = 0; i < str.length(); i++)
    {
        if (!(
            ((str[i] >= '0') && (str[i] <= '9')) ||
            ((str[i] == '-') && (i == 0)) ||
            ((str[i] == '.') && (allowDecimalPoint) && (!alreadyParsedDecimalPoint) && (digitCount > 0))
            )) return false;


        // Here we just have to check for the character. Not for any other conditions.
        // Why? Because if these conditions failed, the function would have already returned false.
        if (((str[i] >= '0') && (str[i] <= '9'))) digitCount++;
        if (str[i] == '.') alreadyParsedDecimalPoint = true;
    }

    // Even if we did not find any invalid chars, we should still return false, if we found no digits at all.
    return digitCount > 0;
}

bool StringTools::ParseNumber(const std::string& str, bool& out_isInt, long double& out_number)
{
    bool isDecimal = false;

    if (str.length() == 0) return false;
    if (Contains(str, '.')) isDecimal = true;

    if (isDecimal)
    {
        try
        {
            out_number = std::stold(str);
            out_isInt = false;
        }
        catch (std::invalid_argument&)
        {
            return false;
        }
        catch (std::out_of_range&)
        {
            return false;
        }
    }
    else
    {
        try
        {
            out_number = (long double)std::stoll(str);
            out_isInt = true;
        }
        catch (std::invalid_argument&)
        {
            return false;
        }
        catch (std::out_of_range&)
        {
            return false;
        }
    }

    return true;
}

std::vector<std::string> StringTools::SplitString(const std::string& str, const char delimiter)
{
    if (str.length() == 0) return std::vector<std::string>();

    return SplitString(str, delimiter);
}

std::vector<std::string> StringTools::SplitString(const std::string& str, const std::string& delimiter)
{
    if (str.length() == 0) return std::vector<std::string>();

    std::vector<std::string> parts;

    if (delimiter.length() == 0) // If the delimiter is "" (empty), just split between every single char. Not useful, but logical
    {
        for (std::size_t i = 0; i < str.length(); i++)
        {
            parts.push_back(std::string({ str[i] }));
        }
        return parts;
    }

    std::size_t posFound = 0;
    std::size_t lastFound = 0;

    while (posFound != std::string::npos)
    {
        lastFound = posFound;
        posFound = str.find(delimiter, posFound);

        std::string found;

        if (posFound != std::string::npos)
        {
            found = str.substr(lastFound, posFound - lastFound);
            posFound += delimiter.length();
        }
        else
        {
            found = str.substr(lastFound, str.length() - lastFound);
        }

        parts.push_back(found);
    }

    return parts;
}

std::string StringTools::ToLower(const std::string& str)
{
    std::stringstream ss;
    for (std::size_t i = 0; i < str.length(); i++)
    {
        if ((str[i] >= 'A') && (str[i] <= 'Z')) ss << (char)(((int)str[i]) + 32);
        else if (str[i] == -60) ss << (char)-28; // AE => ae
        else if (str[i] == -42) ss << (char)-10; // OE => oe
        else if (str[i] == -36) ss << (char)-4;  // UE => ue
        else ss << str[i];
    }

    return ss.str();
}
