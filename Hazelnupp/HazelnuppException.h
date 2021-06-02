#pragma once
#include <stdexcept>

/** Generic hazelnupp exception
*/
class HazelnuppException : public std::exception 
{
public:
	HazelnuppException() {};
	HazelnuppException(const std::string& msg) : message{ msg } {};

	//! Will return an error message
	const std::string& What() const
	{
		return message;
	}

protected:
	std::string message;
};

class HazelnuppInvalidKeyException : public HazelnuppException
{
public:
	HazelnuppInvalidKeyException() : HazelnuppException() {};
	HazelnuppInvalidKeyException(const std::string& msg) : HazelnuppException(msg) {};
};

class HazelnuppValueNotConvertibleException : public HazelnuppException
{
public:
	HazelnuppValueNotConvertibleException() : HazelnuppException() {};
	HazelnuppValueNotConvertibleException(const std::string& msg) : HazelnuppException(msg) {};
};

class HazelnuppConstraintException : public HazelnuppException
{
public:
	HazelnuppConstraintException() : HazelnuppException() {};
	HazelnuppConstraintException(const std::string& msg) : HazelnuppException(msg) {};
};

class HazelnuppConstraintTypeMissmatch : public HazelnuppConstraintException
{
public:
	HazelnuppConstraintTypeMissmatch() : HazelnuppConstraintException() {};
	HazelnuppConstraintTypeMissmatch(const std::string& msg) : HazelnuppConstraintException(msg) {};
};

class HazelnuppConstraintMissingValue : public HazelnuppConstraintException
{
public:
	HazelnuppConstraintMissingValue() : HazelnuppConstraintException() {};
	HazelnuppConstraintMissingValue(const std::string& msg) : HazelnuppConstraintException(msg) {};
};
