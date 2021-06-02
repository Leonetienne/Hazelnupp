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
