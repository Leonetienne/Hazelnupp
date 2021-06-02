#include "VoidValue.h"

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
	throw std::bad_cast();
}

int VoidValue::GetInt32() const
{
	throw std::bad_cast();
}

long double VoidValue::GetFloat64() const
{
	throw std::bad_cast();
}

double VoidValue::GetFloat32() const
{
	throw std::bad_cast();
}

std::string VoidValue::GetString() const
{
	throw std::bad_cast();
}

const std::vector<Value*>& VoidValue::GetList() const
{
	throw std::bad_cast();
}
