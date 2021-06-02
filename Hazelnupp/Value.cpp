#include "Value.h"

Value::Value(DATA_TYPE type)
	:
	type{ type }
{
	return;
}

DATA_TYPE Value::GetDataType() const
{
	return type;
}
//
//long long int Value::GetInt64() const
//{
//	if (type != DATA_TYPE::INT)
//		throw std::bad_cast();
//
//	return ((IntValue*)this)->GetValue();
//}
//
//int Value::GetInt32() const
//{
//	if (type != DATA_TYPE::INT)
//		throw std::bad_cast();
//
//	return (int)((IntValue*)this)->GetValue();
//}
//
//long double Value::GetFloat64() const
//{
//	if (type != DATA_TYPE::FLOAT)
//		throw std::bad_cast();
//
//	return ((FloatValue*)this)->GetValue();
//}
//
//double Value::GetFloat32() const
//{
//	if (type != DATA_TYPE::FLOAT)
//		throw std::bad_cast();
//
//	return (double)((FloatValue*)this)->GetValue();
//}
//
//const std::string& Value::GetString() const
//{
//	if (type != DATA_TYPE::STRING)
//		throw std::bad_cast();
//
//	return ((StringValue*)this)->GetValue();
//}
//
//const std::vector<Value*>& Value::GetList() const
//{
//	if (type != DATA_TYPE::LIST)
//		throw std::bad_cast();
//
//	return ((ListValue*)this)->GetValue();
//}
