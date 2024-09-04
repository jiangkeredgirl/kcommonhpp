/*********************************************************************
 * \file   macrotostring.h
 * \brief  将一些数字、宏定义、枚举转换成有意义字符串，在log里方便阅读
 * 
 * \author jiang
 * \date   2023.05.29
 *********************************************************************/
#pragma once


#include "CodeFormat.h"


/// 输出枚举、宏定义的字符串
#define MACRO_NAME_STR(macro_name)                                      (#macro_name)   //< 打印宏定义名
#define MACRO_VALUE_STR(macro_name)                                     (MACRO_NAME_STR(macro_name))   //< 打印宏定义值
#define MACRO_NAME_VALUE_STR(macro_name)                                                                                                         (string(#macro_name) + "#" + MACRO_VALUE_STR(macro_name)) //< 打印宏定义名和值
#define MACRO_NAME_VALUE_STR_CASE(macro_name, ...)                      case macro_name: return                                   /*utf8tolocal*/(string(#macro_name) + "#" + MACRO_VALUE_STR(macro_name) + "#" + string(## __VA_ARGS__));break;
#define MACRO_NAME_VALUE_FEILD_STR_CASE(macro_name, feild_name, ...)    case macro_name: return /*utf8tolocal*/"[" + (string(#feild_name) + ":" + string(#macro_name) + "#" + MACRO_VALUE_STR(macro_name) + "#" + string(## __VA_ARGS__)) + "]";break;

#define ENUM_NAME_STR(macro_name)                                      (#macro_name)   //< 打印宏定义名
#define ENUM_VALUE_STR(macro_name)                                     (to_string(macro_name))   //< 打印宏定义值
#define ENUM_NAME_VALUE_STR(macro_name)                                                                                                         (string(#macro_name) + "#" + ENUM_VALUE_STR(macro_name)) //< 打印宏定义名和值
#define ENUM_NAME_VALUE_STR_CASE(macro_name, ...)                      case macro_name: return                                   /*utf8tolocal*/(string(#macro_name) + "#" + ENUM_VALUE_STR(macro_name) + "#" + string(## __VA_ARGS__));break;
#define ENUM_NAME_VALUE_FEILD_STR_CASE(macro_name, feild_name, ...)    case macro_name: return /*utf8tolocal*/"[" + (string(#feild_name) + ":" + string(#macro_name) + "#" + ENUM_VALUE_STR(macro_name) + "#" + string(## __VA_ARGS__)) + "]";break;
//#define MACRO_NAME_STR(name, value)                                 (utf8tolocal(string(#name) + ":" + #value))
//#define MACRO_NAME_STR_CASE(name, value, ...)    case value: return (utf8tolocal(string(#name) + ":" + #value)) + (utf8tolocal(string(__VA_ARGS__)));break;
