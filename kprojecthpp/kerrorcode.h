/*********************************************************************
 * \file   errorcode.h
 * \brief  整个软件的错误码定义
 * 
 * \author jiang
 * \date   2023.05.29
 *********************************************************************/
#pragma once
#include <map>
using namespace std;
#include "macrotostring.h"


 /// 错误等级
enum E_ERROR_LEVEL
{
	ERROR_LEVEL_NONE,     //< 没有错误	
	ERROR_LEVEL_WARN,     //< 警告类错误
	ERROR_LEVEL_ERROR,    //< 一般错误
	ERROR_LEVEL_FATAL,    //< 严重错误
};

/// error_code  0-100 reserver
#define ERRORCODE_OK                            0
#define ERRORCODE_FAIL                          1


/// 定义错误码的错误等级
const map<int, int> g_erorr_code_level = 
{
	{ERRORCODE_OK,   ERROR_LEVEL_NONE},
	{ERRORCODE_FAIL, ERROR_LEVEL_ERROR}
};

/// <summary>
/// 根据错误码，返回该错误码的错误等级
/// </summary>
/// <param name="error_code"></param>
/// <returns></returns>
inline int ErrorLevel(int error_code)
{
	if (g_erorr_code_level.count(error_code))
	{
		return g_erorr_code_level.find(error_code)->second;
	}
	return ERROR_LEVEL_NONE;
}

/// <summary>
/// 错误等级枚举值字符串化
/// </summary>
/// <param name="error_level"></param>
/// <returns></returns>
inline string ErrorLevelStr(int error_level)
{
	switch (error_level)
	{
		ENUM_NAME_VALUE_FEILD_STR_CASE(ERROR_LEVEL_NONE, error_level);
		ENUM_NAME_VALUE_FEILD_STR_CASE(ERROR_LEVEL_WARN, error_level);
		ENUM_NAME_VALUE_STR_CASE(ERROR_LEVEL_ERROR, "一般错误");
		ENUM_NAME_VALUE_FEILD_STR_CASE(ERROR_LEVEL_FATAL, error_level);
	default:
		return("warn:error_level:" + to_string(error_level) + " unknown");
		break;
	}
	return "";
}

inline string ErrorCodeStr(int errorcode)
{
	switch (errorcode)
	{
		ENUM_NAME_VALUE_FEILD_STR_CASE(ERRORCODE_OK, errorcode, "无错误");
		ENUM_NAME_VALUE_FEILD_STR_CASE(ERRORCODE_FAIL, errorcode, "错误未定义");
	default:
		return("warn:errorcode:" + to_string(errorcode) + " unknown");
		break;
	}
	return "";
}