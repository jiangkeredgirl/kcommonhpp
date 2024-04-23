/*********************************************************************
 * \file   event.h
 * \brief  事件、事件回调函数定义
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
#include "kcommon.h"
#include "kmacro.h"


/// <summary>
/// 事件基类
/// </summary>
struct KKEvent
{
	EnumEventID event_id = EVENT_ID_UNKNOWN;
	int64_t   event_time = KTime<>::GetNowDateTime(); //< 事件产生的时间
	KKEvent()
	{
		event_id = EVENT_ID_UNKNOWN;
	}
#if 1
	virtual string EventStr() const
	{
		return ClassName(*this) + ":"
			+ "\n  event_id:" + EnumStr(event_id)
			+ "\n, event_time:" + KTime<>::GetDateTime(event_time);
	}
#endif
};

/**
 * 事件1.
 */
struct KKEvent1 : public KKEvent
{
	string user_dat;
	KKEvent1()
	{
		event_id = EVENT_ID_1;
	}
#if 1	
	virtual string EventStr() const override
	{
		return
			KKEvent::EventStr()
			+ "\n, user_dat:" + user_dat;
	}
#endif
};

/**
 * 事件2.
 */
struct KKEvent2 : public KKEvent
{
	int status = 0;
	KKEvent2()
	{
		event_id = EVENT_ID_2;
	}
#if 1
	virtual string EventStr() const override
	{
		return
			KKEvent::EventStr()
			+ "\n, status:" + to_string(status);
	}
#endif
};

/// <summary>
/// 事件函数
/// </summary>
typedef std::function<int(const shared_ptr<KKEvent>& event_data)> EventFunc;
