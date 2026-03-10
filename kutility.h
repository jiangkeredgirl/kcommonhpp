#pragma once

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

#include "cstandard.h"

#ifdef UNICODE
typedef wstring tstring;
#else
typedef  string tstring;
#endif // !UNICODE

//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>

namespace kk
{
	class  Utility
	{
	public:
		typedef struct _TVerionInfo
		{
			int version1;                ///< 主版本号，初始值0，重大结构调整(例如重构) 值增1
			int version2;                ///< 次版本号，初始值0，接口改变(例如增加接口) 值增1
			int version3;                ///< 末版本号，初始值0，bug修复(例如修改bug接口不变) 值增1
			int number;                  ///< 总版本号，初始值0，每次版本发布 值增1
			int type;                    ///< 正式版0， 试用版1 正式版 调试版2
										 //int year;                    ///< 发布时间年
										 //int month;                   ///< 发布时间月
										 //int day;                     ///< 发布时间日
			string build_pc_name;       ///< 编译电脑名
			string build_pc_user_name;  ///< 编译电脑用户名
			string build_datetime;      ///< 编译日期时间
			string file_name;
			string author_name;
			_TVerionInfo()
			{
				version1 = 0;
				version2 = 0;
				version3 = 0;
				number = 0;
				type = 0;
				//year     = 0;
				//month    = 0;
				//day      = 0;
			}
		}TVerionInfo;

	public:
		static string WstringToString(const wstring& ws)
		{
			size_t   i;
			string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
			setlocale(LC_ALL, "chs");
			const wchar_t* _Source = ws.c_str();
			size_t _Dsize = 2 * ws.size() + 1;
			char *_Dest = new char[_Dsize];
			memset(_Dest, 0, _Dsize);
			wcstombs_s(&i, _Dest, _Dsize, _Source, _Dsize);
			string result = _Dest;
			delete[]_Dest;
			setlocale(LC_ALL, curLocale.c_str());
			return result;
		}
		static wstring StringToWstring(const string& s)
		{
			size_t   i;
			setlocale(LC_ALL, "chs");
			const char* _Source = s.c_str();
			size_t _Dsize = s.size() + 1;
			wchar_t *_Dest = new wchar_t[_Dsize];
			wmemset(_Dest, 0, _Dsize);
			mbstowcs_s(&i, _Dest, _Dsize, _Source, _Dsize);
			wstring result = _Dest;
			delete[]_Dest;
			setlocale(LC_ALL, "C");
			return result;
		}
		static tstring StrToTStr(const string& str)
		{
#ifdef UNICODE
			return StringToWstring(str);
#else
			return str;
#endif // !UNICODE
		}
		static string TStrToStr(const tstring& tstr)
		{
#ifdef UNICODE
			return WstringToString(tstr);
#else
			return tstr;
#endif // !UNICODE
		}
		static string GetFileName(__in const string& path_name)
		{
			string file_name(path_name);
			string::size_type pos = file_name.find_last_of("\\/");
			if (pos != string::npos)
			{
				file_name = file_name.substr(pos + 1);
			}
			return file_name;
		}
		static string GetDirectoryName(__in const string& path_name)
		{
			string directoy_name(path_name);
			string::size_type pos = directoy_name.find_last_of(("\\/"));
			if (pos != string::npos)
			{
				directoy_name = directoy_name.substr(0, pos + 1);
			}
			else
			{
				directoy_name += ('\\');
			}
			return directoy_name;
		}
		static string GetWorkPath()
		{
			char path_name[1024 + 1] = { 0 };
			_getcwd(path_name, 1024);
			return path_name;
		}
		static string GetProgramPath()
		{
			string fullpath;
			errno_t err = 0;
			if (_wpgmptr != nullptr)
			{
				wchar_t *wpath = nullptr;
				err = _get_wpgmptr(&wpath);
				if (err == 0)
				{
					fullpath = WstringToString(wpath);
				}
			}
			else if (_pgmptr != nullptr)
			{
				char *path = nullptr;
				err = _get_pgmptr(&path);
				if (err == 0)
				{
					fullpath = path;
				}
			}
			return fullpath;
		}
		static __int64 GetDateTime()
		{
			__time64_t t;
			return _time64(&t);
		}
		static string GetDateTimeStr(__time64_t t)
		{
			tm today;
			_localtime64_s(&today, &t);
			char str_date[64] = { 0 };
			strftime(str_date, sizeof(str_date), ("%Y-%m-%d_%H-%M-%S"), &today);
			return str_date;
		}
		static __int64 GetDateTime(const string& strDateTime)
		{
			tm dateTime = { 0 };
			sscanf_s(strDateTime.c_str(), "%d-%d-%d_%d-%d-%d"
				, &dateTime.tm_year, &dateTime.tm_mon, &dateTime.tm_mday
				, &dateTime.tm_hour, &dateTime.tm_min, &dateTime.tm_sec);
			dateTime.tm_year -= 1900;
			dateTime.tm_mon -= 1;
			return mktime(&dateTime);
		}
		static string GetDateStr()
		{
			__time64_t t;
			_time64(&t);
			tm today;
			_localtime64_s(&today, &t);
			char str_date[64] = { 0 };
			strftime(str_date, sizeof(str_date), ("%Y-%m-%d"), &today);
			return str_date;
		}
		static string GetLogDateTimeStr()
		{
			__time64_t t = GetDateTime();
			return GetDateTimeStr(t) + "_" + to_string(t);
		}
		static __int64 GetRunTime()
		{
			__int64 runtime = clock();
			return runtime;  /// millisecond 
		}
		static string GetRunTimeStr(__int64 runtime)  ///< h:%d:m:%d:s:%d:ms:%d
		{
			int h = static_cast<int>((runtime) / (1000 * 3600));
			int m = ((runtime) % (1000 * 3600)) / (1000 * 60);
			int s = (((runtime) % (1000 * 3600)) % (1000 * 60)) / 1000;
			int ms = (((runtime) % (1000 * 3600)) % (1000 * 60)) % 1000;
			char str_time[128] = { 0 };
			sprintf_s(str_time, ("%dh:%dm:%ds:%dms"), h, m, s, ms);
			return str_time;
		}
		static __int64 GetRunTime(const string& strRunTime)  ///< h:%d:m:%d:s:%d:ms:%d
		{
			__int64 runtime = 0;
			int h = 0;
			int m = 0;
			int s = 0;
			int ms = 0;
			sscanf_s(strRunTime.c_str(), "%dh:%dm:%ds:%dms", &h, &m, &s, &ms);
			runtime += h * (1000 * 3600);
			runtime += m * (1000 * 60);
			runtime += s * 1000;
			runtime += ms;
			return runtime;
		}
		static int ReadFile(__in const string& filename, __out string& file_content)
		{
			int error_code = -1;
			FILE* file = nullptr;
			error_code = fopen_s(&file, filename.c_str(), "rb");
			if (error_code == 0)
			{
				do
				{
					char* content = nullptr;
					size_t content_size = 0;
					_fseeki64(file, 0, SEEK_END);
					content_size = (size_t)_ftelli64(file);
					content = new char[content_size + 1];
					if (content == nullptr)
					{
						break;
					}
					memset(content, 0, content_size + 1);
					_fseeki64(file, 0, SEEK_SET);
					size_t have_read_size = fread(content, sizeof(char), content_size, file);
					if (have_read_size == content_size)
					{
						file_content = content;
						error_code = 0;
					}
					delete[] content;
				} while (false);
				fclose(file);
			}
			return error_code;
		}
		static int WriteFile(__in const string& filename, __out const string& file_content)
		{
			int error_code = -1;
			FILE* file = nullptr;
			error_code = fopen_s(&file, filename.c_str(), "wb");
			if (error_code == 0)
			{
				size_t write_size = fwrite(file_content.c_str(), sizeof(char), file_content.size(), file);
				fflush(file);
				if (write_size == file_content.size())
				{
					error_code = 0;
				}
				fclose(file);
			}
			return error_code;
		}
		static int CreateDir(__in const string& path)
		{
			int error_code = 0;
			if (_access(path.c_str(), 0) == -1)
			{
				string::size_type pos = path.rfind('\\');
				if (pos != string::npos)
				{
					string base_path = path.substr(0, pos);
					CreateDir(base_path);
				}
				if (_access(path.c_str(), 0) == -1)
				{
					error_code = _mkdir(path.c_str());
				}
			}
			return error_code;
		}
		static int DeleteDir(__in const string& path)
		{
			int error_code = -1;
			if (_access(path.c_str(), 0) == 0)
			{
				string filespec = path + "\\*.*";
				struct __finddata64_t fileinfo;
				intptr_t file = _findfirst64(filespec.c_str(), &fileinfo);
				if (file != -1)
				{
					while (_findnext64(file, &fileinfo) == 0)
					{
						string sub_file = path + "\\" + fileinfo.name;
						if (fileinfo.attrib == _A_SUBDIR)
						{
							if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
							{
								DeleteDir(sub_file);
							}
						}
						else
						{
							remove(sub_file.c_str());
						}
					}
					_findclose(file);
				}
				error_code = _rmdir(path.c_str());
			}
			return error_code;
		}
		static int PathIsExist(__in const string& path)
		{
			if ((_access(path.c_str(), 0)) != -1)
			{
				return 1;  ///< exist
			}
			//else if (path.find(":") != string::npos)
			//{
			//	return 2;  ///< available
			//}
			return 0;
		}

	};
}
