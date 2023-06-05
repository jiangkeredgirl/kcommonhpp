#pragma once

#include <string>
#include <chrono>
#include <sstream>
#include <cinttypes>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <map>
using namespace std;
using namespace chrono;


inline time_t GetNowDateTimeStamp()
{
	std::time_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return timestamp; //毫秒时间
}

inline time_t GetDateTimeStamp(const string& timestr)  ///< h:%d:m:%d:s:%d:ms:%d
{
	tm tm_;
	int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, millisecond = 0;
	sscanf(timestr.c_str(), "%d-%d-%d %d.%d.%d.%d", &year, &month, &day, &hour, &minute, &second, &millisecond);
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

inline time_t GetDateTimeStamp(const string& timeformat, const string& timestr)  ///< h:%d:m:%d:s:%d:ms:%d
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

inline string GetDateTimeString(std::time_t timestamp)
{
	auto const millisecs = timestamp % 1000;
	std::stringstream ss;
	ss << std::put_time(std::localtime(&(timestamp /= 1000)), "%Y-%m-%d %H.%M.%S") << "." << millisecs;
	return ss.str(); //毫秒时间
}

inline string GetDateString(std::time_t timestamp)
{
	std::stringstream ss;
	ss << std::put_time(std::localtime(&(timestamp /= 1000)), "%Y-%m-%d");
	return ss.str(); //毫秒时间
}

inline string GetTimeString(std::time_t timestamp)
{
	auto const millisecs = timestamp % 1000;
	std::stringstream ss;
	ss << std::put_time(std::localtime(&(timestamp /= 1000)), "%H.%M.%S") << "." << millisecs;
	return ss.str(); //毫秒时间
}


///////////////////////////////////////////////////////

inline time_t GetNowDateTimeStampMicro()
{
	std::time_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return timestamp; //微秒时间
}

inline time_t GetDateTimeStampMicro(const string& timestr)  ///< h:%d:m:%d:s:%d:ms:%d
{
	tm tm_;
	int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, millisecond = 0, microsecond = 0;
	sscanf(timestr.c_str(), "%d-%d-%d %d.%d.%d.%d.%d", &year, &month, &day, &hour, &minute, &second, &millisecond, &microsecond);
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

inline time_t GetDateTimeStampMicro(const string& timeformat, const string& timestr)  ///< h:%d:m:%d:s:%d:ms:%d
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

inline string GetDateTimeStringMicro(std::time_t timestamp)
{
	auto const millisecs = (timestamp % 1000000) / 1000;
	auto const microsecs = timestamp % 1000;
	std::stringstream ss;
	ss << std::put_time(std::localtime(&(timestamp /= 1000)), "%Y-%m-%d %H.%M.%S") << "." << millisecs << "." << microsecs;
	return ss.str(); //微秒时间
}
