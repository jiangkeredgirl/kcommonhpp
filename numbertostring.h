/*********************************************************************
 * \file   numbertostring.h
 * \brief  将一些数字、宏定义转换成有意义字符串，在log里方便阅读
 * 
 * \author jiang
 * \date   2023.05.29
 *********************************************************************/
#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <strstream>
using namespace std;


/// 输出枚举
#define STR_ENUM(name, value)        return ((string(#name) + ":" + #value));
#define STR_ENUM_CASE(name, value, ...)   case value: return ((string(#name) + ":" + #value + string(__VA_ARGS__)));break;
