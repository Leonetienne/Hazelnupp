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

/** Gets thrown when an non-existent key gets dereferenced
*/
class HazelnuppInvalidKeyException : public HazelnuppException
{
public:
	HazelnuppInvalidKeyException() : HazelnuppException() {};
	HazelnuppInvalidKeyException(const std::string& msg) : HazelnuppException(msg) {};
};

/** Gets thrown when an attempt is made to retrieve the wrong data type from a value, when the value not convertible
*/
class HazelnuppValueNotConvertibleException : public HazelnuppException
{
public:
	HazelnuppValueNotConvertibleException() : HazelnuppException() {};
	HazelnuppValueNotConvertibleException(const std::string& msg) : HazelnuppException(msg) {};
};

/** Gets thrown something bad happens because of parameter constraints
*/
class HazelnuppConstraintException : public HazelnuppException
{
public:
	HazelnuppConstraintException() : HazelnuppException() {};
	HazelnuppConstraintException(const std::string& msg) : HazelnuppException(msg) {};
};

/** Gets thrown when a parameter is of a type that does not match the required type, and is not convertible to it
*/
class HazelnuppConstraintTypeMissmatch : public HazelnuppConstraintException
{
public:
	HazelnuppConstraintTypeMissmatch() : HazelnuppConstraintException() {};
	HazelnuppConstraintTypeMissmatch(const std::string& msg) : HazelnuppConstraintException(msg) {};
};

/** Gets thrown when a parameter constrained to be required is not provided, and has no default value set
*/
class HazelnuppConstraintMissingValue : public HazelnuppConstraintException
{
public:
	HazelnuppConstraintMissingValue() : HazelnuppConstraintException() {};
	HazelnuppConstraintMissingValue(const std::string& msg) : HazelnuppConstraintException(msg) {};
};
