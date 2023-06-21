#pragma once
#ifndef LOG_WRAPPER_H
#define LOG_WRAPPER_H

#include <string>
#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <chrono>
#include <sstream>
#include <cinttypes>
#include <ctime>
#include <iomanip>
#include <map>
using namespace std;
using namespace chrono;
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/async.h"
#include "spdlog/pattern_formatter.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/common.h"
using namespace spdlog;
#include "ktime.h"
#include "SpdLogWrapper.h"


//#define SIZE_100M           (1024 * 1024 * 100)  //redmine 上传文件限制最大50M，所以由100M改成49M
//#define FILES_NUM           1000

//#define LOG_INFO   LOG_INFO
//#define LOG_WARN   spd::get("log")->warn
//#define LOG_ERROR  LOG_ERROR


#define LOG_ERROR(log_format, ...)                 LOGGERNAME_ERROR("default_logger",      log_format, ##__VA_ARGS__)
#define LOG_WARN(log_format, ...)                  LOGGERNAME_WARN("default_logger",       log_format, ##__VA_ARGS__)
#define LOG_INFO(log_format, ...)                  LOGGERNAME_INFO("default_logger",       log_format, ##__VA_ARGS__)
#define LOG_DEBUG(log_format, ...)                 LOGGERNAME_DEBUG("default_logger",      log_format, ##__VA_ARGS__)
#define LOG_TRACE(log_format, ...)                 LOGGERNAME_TRACE("default_logger",      log_format, ##__VA_ARGS__)
#define LOG_TEMP(log_format, ...)                  LOGGERNAME_INFO("temp_logger",          log_format, ##__VA_ARGS__)


//#define GUI_FORMAT(log_format, qt_format) std::string(log_format).append(qt_format).c_str()

#define LOG_GUI_CLICK(widget) LOG_INFO("[GUI_CLICK] button name:{}, text:{}", widget->objectName().toLocal8Bit().data(), widget->text().toLocal8Bit().data())
#define LOG_GUI_INPUT(widget) LOG_INFO("[GUI_INPUT] edit name:{}, text:{}", widget->objectName().toLocal8Bit().data(), widget->text().toLocal8Bit().data())
#define LOG_GUI_POPUP(widget) LOG_INFO("[GUI_POPUP] widget or dialog name:{}", widget->objectName().toLocal8Bit().data())

//static map<string, string> g_log_file_paths;
//map<string, std::shared_ptr<spdlog::logger>> g_loggers;

inline void WriteLogHeader(const string& loggername, const filename_t& filename, std::FILE* file_stream, const string& log_pattern);

/**  */
inline int InitLog(const string& root_log_dir, spdlog::level::level_enum log_level)
{
	string default_log_path;
	string temp_log_path;

	string date = KTime<>::GetDate(KTime<>::GetNowDateTime());
	string time = KTime<>::GetTime(KTime<>::GetNowDateTime());
	default_log_path = root_log_dir + "/" + date + "/" + date + "-" + time;
	temp_log_path = default_log_path;

	KFile::CreateDir(default_log_path);
	KFile::CreateDir(temp_log_path);

	default_log_path += "/drawbloodrobot.log";
	temp_log_path += "/drawbloodrobot_temp.log";
	const string  log_pattern("[%i-%t-%Y-%m-%d %H:%M:%S.%e] [%l] %v");

	SpdlogInit();

	bool is_async = true;
	bool is_daily = true;
	std::shared_ptr<spdlog::logger>  default_logger = CreateLogger("default_logger", true, true, is_async, is_daily, false, default_log_path, log_level, log_pattern, [log_pattern](const filename_t& filename, std::FILE* file_stream) {WriteLogHeader("default_logger", filename, file_stream, log_pattern); });
	std::shared_ptr<spdlog::logger>  temp_logger = CreateLogger("temp_logger", true, true, is_async, is_daily, false, temp_log_path, log_level, log_pattern, [log_pattern](const filename_t& filename, std::FILE* file_stream) {WriteLogHeader("temp_logger", filename, file_stream, log_pattern); });

	//spdlog::set([](const std::string &msg) { spdlog::get("console")->error("*** LOGGER ERROR ***: {}", msg); });
	//spdlog::get("console")->info("some invalid message to trigger an error {}{}{}{}", 3);

	return 0;
}

inline void FlushLog()
{
	DropLog();
}

inline void WriteLogHeader(const string& loggername, const filename_t& filename, std::FILE* file_stream, const string& log_pattern)
{
	char head_buf[1000] = { 0 };
	sprintf(head_buf,
		"log header:logger name:%s log filename:%s log pattern:%s\n"
		, loggername.c_str(), filename.c_str(), log_pattern.c_str());
	fputs(head_buf, file_stream);
	fflush(file_stream);
}

#endif // LOG_WRAPPER_H

