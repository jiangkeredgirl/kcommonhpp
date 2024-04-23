/*********************************************************************
 * \file   kmacro.h
 * \brief  枚举、宏定义
 *
 * \author jiang
 * \date   2023.06.01
 *********************************************************************/
#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <string>
using namespace std;
#include "macrotostring.h"

/// 宏定义
#define PI          (3.14)     //< pi的值
#define GRAVITY     (9.8)      //< 重力的值
#define MP3         ("mp3")    //< MP3文件后缀
#define YEARDAYS    (365)      //< 一年365天


/// 枚举定义
#define ENUM_NUM_BASE     (1000)   //< 枚举基准值
/**
 * 状态机枚举.
 */
enum EnumStateMachine
{
	ENUM_STATE_MACHINE_1 = ENUM_NUM_BASE + __LINE__, //< 状态机状态枚举
	ENUM_STATE_MACHINE_2,                            //< 状态机状态枚举
	ENUM_STATE_MACHINE_3,                            //< 状态机状态枚举
};

/// <summary>
/// 事件id
/// </summary>
enum EnumEventID
{
	EVENT_ID_UNKNOWN = ENUM_NUM_BASE + __LINE__,
	EVENT_ID_1,
	EVENT_ID_2,

};

inline string MacroStr(int macro)
{
	switch (macro)
	{
		MACRO_NAME_VALUE_FEILD_STR_CASE(YEARDAYS, macro, "一年365天");
	default:
		return("warn:macro value:" + to_string(macro) + " unknown");
		break;
	}
	return "";
}

inline string EnumStr(int enumvalue)
{
	switch (enumvalue)
	{
		ENUM_NAME_VALUE_FEILD_STR_CASE(ENUM_STATE_MACHINE_1, enumvalue);
		ENUM_NAME_VALUE_FEILD_STR_CASE(ENUM_STATE_MACHINE_2, enumvalue, "状态机状态枚举");
		ENUM_NAME_VALUE_FEILD_STR_CASE(ENUM_STATE_MACHINE_3, enumvalue, "状态机状态枚举");
	default:
		return("warn:enum value:" + to_string(enumvalue) + " unknown");
		break;
	}
	return "";
}