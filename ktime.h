/*********************************************************************
 * \file   ktime.h
 * \brief  日期时间函数
 *
 * \author jiang
 * \date   2023.06.14
 *********************************************************************/
#pragma once

#include <string>
#include <chrono>
#include <sstream>
#include <cinttypes>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <map>
#include "ProducerConsumerTemplate.h"
using namespace std;
using namespace chrono;

template <class TimeUnit = std::chrono::milliseconds>
class KTime
{
public:
	inline static time_t GetNowDateTime()
	{
		std::time_t timestamp = std::chrono::duration_cast<TimeUnit>(std::chrono::system_clock::now().time_since_epoch()).count();
		return timestamp; //毫秒时间
#if 0
		std::time_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		return timestamp; //微秒时间
#endif
	}
	inline static time_t GetDateTime(const string& timestr)  ///< h:%d:m:%d:s:%d:ms:%d
	{
		typedef std::function<time_t(const string& timestr)> GetDateTimeF;
		map<string, GetDateTimeF> get_datetime_f;
		get_datetime_f[ClassName<seconds>()] = std::bind((time_t(*)(const string & timestr)) & GetDateTimeSecond, std::placeholders::_1);
		get_datetime_f[ClassName<milliseconds>()] = std::bind((time_t(*)(const string & timestr)) & GetDateTimeMilli, std::placeholders::_1);
		get_datetime_f[ClassName<microseconds>()] = std::bind((time_t(*)(const string & timestr)) & GetDateTimeMicro, std::placeholders::_1);
		string time_unit = ClassName<TimeUnit>();
		if (get_datetime_f.count(time_unit))
		{
			return get_datetime_f[time_unit](timestr);
		}
		return -1;
	}
	inline static time_t GetDateTime(const string& timestr, const string& timeformat)  ///< h:%d:m:%d:s:%d:ms:%d
	{
		typedef std::function<time_t(const string& timestr, const string& timeformat)> GetDateTimeF;
		map<string, GetDateTimeF> get_datetime_f;
		get_datetime_f[ClassName<seconds>()] = std::bind((time_t(*)(const string & timestr, const string & timeformat)) & GetDateTimeSecond, std::placeholders::_1, std::placeholders::_2);
		get_datetime_f[ClassName<milliseconds>()] = std::bind((time_t(*)(const string & timestr, const string & timeformat)) & GetDateTimeMilli, std::placeholders::_1, std::placeholders::_2);
		get_datetime_f[ClassName<microseconds>()] = std::bind((time_t(*)(const string & timestr, const string & timeformat)) & GetDateTimeMicro, std::placeholders::_1, std::placeholders::_2);
		string time_unit = ClassName<TimeUnit>();
		if (get_datetime_f.count(time_unit))
		{
			return get_datetime_f[time_unit](timestr, timeformat);
		}
		return -1;
	}
	inline static string GetDateTime(std::time_t timestamp)
	{
		typedef std::function<string(std::time_t timestamp)> GetDateTimeF;
		map<string, GetDateTimeF> get_datetime_f;
		get_datetime_f[ClassName<seconds>()] = std::bind((string(*)(std::time_t)) & GetDateTimeSecond, std::placeholders::_1);
		get_datetime_f[ClassName<milliseconds>()] = std::bind((string(*)(std::time_t)) & GetDateTimeMilli, std::placeholders::_1);
		get_datetime_f[ClassName<microseconds>()] = std::bind((string(*)(std::time_t)) & GetDateTimeMicro, std::placeholders::_1);
		string time_unit = ClassName<TimeUnit>();
		if (get_datetime_f.count(time_unit))
		{
			return get_datetime_f[time_unit](timestamp);
		}
		return "time null";
	}
	inline static string GetDate(std::time_t timestamp)
	{
		map<string, int> time_units;
		time_units[ClassName<seconds>()]      = 1;
		time_units[ClassName<milliseconds>()] = 2;
		time_units[ClassName<microseconds>()] = 3;
		std::time_t loctime = 0;
		string time_unit = ClassName<TimeUnit>();
		switch (time_units[time_unit])
		{
		case 1:
		{
			loctime = timestamp;
			break;
		}
		case 2:
		{
			loctime = timestamp / 1000;
			break;
		}
		case 3:
		{
			loctime = timestamp / 1000000;
			break;
		}
		default:
			loctime = timestamp;
			break;
		}
		std::stringstream ss;
		ss << std::put_time(std::localtime(&loctime), "%Y-%m-%d");
		return ss.str();
	}
	inline static string GetTime(std::time_t timestamp)
	{
		map<string, int> time_units;
		time_units[ClassName<seconds>()]      = 1;
		time_units[ClassName<milliseconds>()] = 2;
		time_units[ClassName<microseconds>()] = 3;
		std::time_t loctime = 0;
		std::time_t millis = 0;
		std::time_t micros = 0;
		string time_unit = ClassName<TimeUnit>();
		std::stringstream ss;
		switch (time_units[time_unit])
		{
		case 1:
		{
			loctime = timestamp;
			millis = 0;
			micros = 0;
			ss << std::put_time(std::localtime(&loctime), "%H.%M.%S");
			break;
		}
		case 2:
		{
			loctime = timestamp / 1000;
			millis = timestamp % 1000;
			micros = 0;
			ss << std::put_time(std::localtime(&loctime), "%H.%M.%S") << "." << millis;
			break;
		}
		case 3:
		{
			loctime = timestamp / 1000000;
			millis = (timestamp / 1000) % 1000;
			micros = timestamp % 1000;
			ss << std::put_time(std::localtime(&loctime), "%H.%M.%S") << "." << millis << "." << micros;
			break;
		}
		default:
			loctime = timestamp;
			millis = 0;
			micros = 0;
			ss << std::put_time(std::localtime(&loctime), "%H.%M.%S") << "." << millis;
			break;
		}
		return ss.str();
	}
	inline static time_t GetClockTime(const string& timestr)  ///< h:%d:m:%d:s:%d:ms:%d
	{
		typedef std::function<time_t(const string& timestr)> GetClockTimeF;
		map<string, GetClockTimeF> get_clocktime_f;
		get_clocktime_f[ClassName<seconds>()]      = std::bind((time_t(*)(const string & timestr)) & GetClockTimeSecond, std::placeholders::_1);
		get_clocktime_f[ClassName<milliseconds>()] = std::bind((time_t(*)(const string & timestr)) & GetClockTimeMilli, std::placeholders::_1);
		get_clocktime_f[ClassName<microseconds>()] = std::bind((time_t(*)(const string & timestr)) & GetClockTimeMicro, std::placeholders::_1);
		string time_unit = ClassName<TimeUnit>();
		if (get_clocktime_f.count(time_unit))
		{
			return get_clocktime_f[time_unit](timestr);
		}
		return -1;
	}
	inline static string GetClockTime(std::time_t timestamp)
	{
		typedef std::function<string(std::time_t timestamp)> GetClockTimeF;
		map<string, GetClockTimeF> get_clocktime_f;
		get_clocktime_f[ClassName<seconds>()] = std::bind((string(*)(std::time_t)) & GetClockTimeSecond, std::placeholders::_1);
		get_clocktime_f[ClassName<milliseconds>()] = std::bind((string(*)(std::time_t)) & GetClockTimeMilli, std::placeholders::_1);
		get_clocktime_f[ClassName<microseconds>()] = std::bind((string(*)(std::time_t)) & GetClockTimeMicro, std::placeholders::_1);
		string time_unit = ClassName<TimeUnit>();
		if (get_clocktime_f.count(time_unit))
		{
			return get_clocktime_f[time_unit](timestamp);
		}
		return "time null";
	}

private:
	inline static __int64 GetClockTimeSecond(const string& strclocktime)  ///< %d:%d:%d.%.%d
	{
		__int64 clocktime = 0;
		int h = 0;
		int m = 0;
		int s = 0;
		sscanf_s(strclocktime.c_str(), "%d:%d:%d", &h, &m, &s);
		clocktime += h * 3600;
		clocktime += m * 60;
		clocktime += s;
		return clocktime;
	}
	inline static string  GetClockTimeSecond(__int64 clocktime)  ///< h:%d:m:%d:s:%d:ms:%d
	{
		int second = 1;
		int h = (clocktime / (second * 3600)) % 60;
		int m = (clocktime / (second * 60)) % 60;
		int s = (clocktime / second) % 60;
		char str_time[128] = { 0 };
		sprintf_s(str_time, ("%d:%d:%d"), h, m, s);
		return str_time;
	}
	inline static __int64 GetClockTimeMilli(const string& strclocktime)  ///< %d:%d:%d.%.%d
	{
		__int64 clocktime = 0;
		int h = 0;
		int m = 0;
		int s = 0;
		int ms = 0;
		sscanf_s(strclocktime.c_str(), "%d:%d:%d.%d", &h, &m, &s, &ms);
		clocktime += h * (1000 * 3600);
		clocktime += m * (1000 * 60);
		clocktime += s * 1000;
		clocktime += ms;
		return clocktime;
	}
	inline static string  GetClockTimeMilli(__int64 clocktime)  ///< h:%d:m:%d:s:%d:ms:%d
	{
		int second = 1000;
		int h = (clocktime / (second * 3600)) % 60;
		int m = (clocktime / (second * 60)) % 60;
		int s = (clocktime / second) % 60;
		int milli = clocktime % 1000;
		char str_time[128] = { 0 };
		sprintf_s(str_time, ("%d:%d:%d.%d"), h, m, s, milli);
		return str_time;
	}
	inline static __int64 GetClockTimeMicro(const string& strclocktime)  ///< %d:%d:%d.%.%d
	{
		__int64 clocktime = 0;
		int h = 0;
		int m = 0;
		int s = 0;
		int millis = 0;
		int micros = 0;
		sscanf_s(strclocktime.c_str(), "%d:%d:%d.%d.%d", &h, &m, &s, &millis, &micros);
		clocktime += h * (1000 * 3600) * 1000;
		clocktime += m * (1000 * 60) * 1000;
		clocktime += s * 1000 * 1000;
		clocktime += millis * 1000;
		clocktime += micros;
		return clocktime;
	}
	inline static string  GetClockTimeMicro(__int64 clocktime)  ///< h:%d:m:%d:s:%d:ms:%d
	{
		int second = 1000000;
		int h = (clocktime / (second * 3600)) % 60;
		int m = (clocktime / (second * 60)) % 60;
		int s = (clocktime / second) % 60;
		int milli = (clocktime / 1000) % 1000;
		int micro = clocktime % 1000;
		char str_time[128] = { 0 };
		sprintf_s(str_time, ("%d:%d:%d.%d.%d"), h, m, s, milli, micro);
		return str_time;
	}

private:
	inline static time_t GetDateTimeSecond(const string& timestr)  ///< h:%d:m:%d:s:%d:ms:%d
	{
		tm tm_;
		int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
		sscanf(timestr.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
		tm_.tm_year = year - 1900;
		tm_.tm_mon = month - 1;
		tm_.tm_mday = day;
		tm_.tm_hour = hour;
		tm_.tm_min = minute;
		tm_.tm_sec = second;
		tm_.tm_isdst = 0;

		time_t t_ = mktime(&tm_); //已经减了8个时区
		return t_; //秒时间
	}
	inline static time_t GetDateTimeSecond(const string& timestr, const string& timeformat)  ///< h:%d:m:%d:s:%d:ms:%d
	{
		tm tm_;
		int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
		sscanf(timestr.c_str(), timeformat.c_str(), &year, &month, &day, &hour, &minute, &second);
		tm_.tm_year = year - 1900;
		tm_.tm_mon = month - 1;
		tm_.tm_mday = day;
		tm_.tm_hour = hour;
		tm_.tm_min = minute;
		tm_.tm_sec = second;
		tm_.tm_isdst = 0;

		time_t t_ = mktime(&tm_); //已经减了8个时区
		return t_; //秒时间
	}
	inline static string GetDateTimeSecond(std::time_t timestamp)
	{
		std::time_t loctime = timestamp;
		std::stringstream ss;
		ss << std::put_time(std::localtime(&loctime), "%Y-%m-%d %H:%M:%S");
		return ss.str(); //秒时间
	}
	inline static time_t GetDateTimeMilli(const string& timestr)  ///< h:%d:m:%d:s:%d:ms:%d
	{
		tm tm_;
		int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, millisecond = 0;
		sscanf(timestr.c_str(), "%d-%d-%d %d:%d:%d.%d", &year, &month, &day, &hour, &minute, &second, &millisecond);
		tm_.tm_year = year - 1900;
		tm_.tm_mon = month - 1;
		tm_.tm_mday = day;
		tm_.tm_hour = hour;
		tm_.tm_min = minute;
		tm_.tm_sec = second;
		tm_.tm_isdst = 0;

		time_t t_ = mktime(&tm_) * 1000 + millisecond; //已经减了8个时区
		return t_; //毫秒时间
	}
	inline static time_t GetDateTimeMilli(const string& timestr, const string& timeformat)  ///< h:%d:m:%d:s:%d:ms:%d
	{
		tm tm_;
		int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, millisecond = 0;
		sscanf(timestr.c_str(), timeformat.c_str(), &year, &month, &day, &hour, &minute, &second, &millisecond);
		tm_.tm_year = year - 1900;
		tm_.tm_mon = month - 1;
		tm_.tm_mday = day;
		tm_.tm_hour = hour;
		tm_.tm_min = minute;
		tm_.tm_sec = second;
		tm_.tm_isdst = 0;

		time_t t_ = mktime(&tm_) * 1000 + millisecond; //已经减了8个时区
		return t_; //毫秒时间
	}
	inline static string GetDateTimeMilli(std::time_t timestamp)
	{
		std::time_t loctime = timestamp / 1000;
		std::time_t millisecs = timestamp % 1000;
		std::stringstream ss;
		ss << std::put_time(std::localtime(&loctime), "%Y-%m-%d %H:%M:%S") << "." << millisecs;
		return ss.str(); //毫秒时间
	}
	inline static time_t GetDateTimeMicro(const string& timestr)  ///< h:%d:m:%d:s:%d:ms:%d
	{
		tm tm_;
		int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, millisecond = 0, microsecond = 0;
		sscanf(timestr.c_str(), "%d-%d-%d %d:%d:%d.%d.%d", &year, &month, &day, &hour, &minute, &second, &millisecond, &microsecond);
		tm_.tm_year = year - 1900;
		tm_.tm_mon = month - 1;
		tm_.tm_mday = day;
		tm_.tm_hour = hour;
		tm_.tm_min = minute;
		tm_.tm_sec = second;
		tm_.tm_isdst = 0;

		time_t t_ = mktime(&tm_) * 1000000 + millisecond * 1000 + microsecond; //已经减了8个时区
		return t_; //微秒时间
	}
	inline static time_t GetDateTimeMicro(const string& timestr, const string& timeformat)  ///< h:%d:m:%d:s:%d:ms:%d
	{
		tm tm_;
		int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, millisecond = 0, microsecond = 0;
		sscanf(timestr.c_str(), timeformat.c_str(), &year, &month, &day, &hour, &minute, &second, &millisecond, &microsecond);
		tm_.tm_year = year - 1900;
		tm_.tm_mon = month - 1;
		tm_.tm_mday = day;
		tm_.tm_hour = hour;
		tm_.tm_min = minute;
		tm_.tm_sec = second;
		tm_.tm_isdst = 0;

		time_t t_ = mktime(&tm_) * 1000000 + millisecond * 1000 + microsecond; //已经减了8个时区
		return t_; //微秒时间
	}
	inline static string GetDateTimeMicro(std::time_t timestamp)
	{
		std::time_t loctime = timestamp / 1000000;
		std::time_t millisecs = (timestamp / 1000) % 1000;
		std::time_t microsecs = timestamp % 1000;
		std::stringstream ss;
		ss << std::put_time(std::localtime(&loctime), "%Y-%m-%d %H:%M:%S") << "." << millisecs << "." << microsecs;
		return ss.str(); //微秒时间
	}
};

