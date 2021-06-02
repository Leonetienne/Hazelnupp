#include "VoidValue.h"
#include "HazelnuppException.h"

VoidValue::VoidValue()
	:
	Value(DATA_TYPE::VOID)
{
	return;
}

Value* VoidValue::Deepcopy() const
{
	return new VoidValue();
}

std::string VoidValue::GetAsOsString() const
{
	return "VoidValue";
}



long long int VoidValue::GetInt64() const
{
	throw HazelnuppValueNotConvertibleException();
}

int VoidValue::GetInt32() const
{
	throw HazelnuppValueNotConvertibleException();
}

long double VoidValue::GetFloat64() const
{
	throw HazelnuppValueNotConvertibleException();
}

double VoidValue::GetFloat32() const
{
	throw HazelnuppValueNotConvertibleException();
}

std::string VoidValue::GetString() const
{
	throw HazelnuppValueNotConvertibleException();
}

const std::vector<Value*>& VoidValue::GetList() const
{
	throw HazelnuppValueNotConvertibleException();
}
