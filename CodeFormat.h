/*********************************************************************
 * \file   CodeFormat.h
 * \brief  编码格式转换
 * 
 * \author jiang
 * \date   2023.06.16
 *********************************************************************/
#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "cstandard.h"
#include <windows.h>

#ifdef UNICODE
typedef wstring tstring;
#else
typedef  string tstring;
#endif // !UNICODE

#if 0
inline std::wstring utf8_to_wstring(const std::u8string& u8str) {
    return std::wstring(u8str.begin(), u8str.end());
}

inline std::u8string wstring_to_utf8(const std::wstring& wstr) {
    return std::u8string(wstr.begin(), wstr.end());
}
#endif

inline std::string /*gb2312_to_*/utf8(std::string const& strGb2312);

inline bool isutf8(const std::string& str)
{
	int i = 0;
	while (i < str.length()) {
		unsigned char c = str[i];
		if (c < 0x80) {
			// 单字节字符（0xxxxxxx）
			i++;
		}
		else if ((c & 0xE0) == 0xC0) {
			// 双字节字符（110xxxxx 10xxxxxx）
			if ((i + 1 < str.length()) && ((str[i + 1] & 0xC0) == 0x80)) {
				i += 2;
			}
			else {
				return false;
			}
		}
		else if ((c & 0xF0) == 0xE0) {
			// 三字节字符（1110xxxx 10xxxxxx 10xxxxxx）
			if ((i + 2 < str.length()) && ((str[i + 1] & 0xC0) == 0x80) && ((str[i + 2] & 0xC0) == 0x80)) {
				i += 3;
			}
			else {
				return false;
			}
		}
		else if ((c & 0xF8) == 0xF0) {
			// 四字节字符（11110xxx 10xxxxxx 10xxxxxx 10xxxxxx）
			if ((i + 3 < str.length()) && ((str[i + 1] & 0xC0) == 0x80) && ((str[i + 2] & 0xC0) == 0x80) && ((str[i + 3] & 0xC0) == 0x80)) {
				i += 4;
			}
			else {
				return false;
			}
		}
		else {
			// 不是UTF-8编码的起始字节
			return false;
		}
	}
	return true;
}

inline bool is_str_utf8(const char* str)
{
	unsigned int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
	unsigned char chr = *str;
	bool bAllAscii = true;
	for (unsigned int i = 0; str[i] != '\0'; ++i) {
		chr = *(str + i);
		//判断是否ASCII编码,如果不是,说明有可能是UTF8,ASCII用7位编码,最高位标记为0,0xxxxxxx
		if (nBytes == 0 && (chr & 0x80) != 0) {
			bAllAscii = false;
		}
		if (nBytes == 0) {
			//如果不是ASCII码,应该是多字节符,计算字节数
			if (chr >= 0x80) {
				if (chr >= 0xFC && chr <= 0xFD) {
					nBytes = 6;
				}
				else if (chr >= 0xF8) {
					nBytes = 5;
				}
				else if (chr >= 0xF0) {
					nBytes = 4;
				}
				else if (chr >= 0xE0) {
					nBytes = 3;
				}
				else if (chr >= 0xC0) {
					nBytes = 2;
				}
				else {
					return false;
				}
				nBytes--;
			}
		}
		else {
			//多字节符的非首字节,应为 10xxxxxx
			if ((chr & 0xC0) != 0x80) {
				return false;
			}
			//减到为零为止
			nBytes--;
		}
	}
	//违返UTF8编码规则
	if (nBytes != 0) {
		return false;
	}
	if (bAllAscii) { //如果全部都是ASCII, 也是UTF8
		return true;
	}
	return true;
}

inline bool is_str_gbk(const char* str)
{
	unsigned int nBytes = 0;//GBK可用1-2个字节编码,中文两个 ,英文一个
	unsigned char chr = *str;
	bool bAllAscii = true; //如果全部都是ASCII,
	for (unsigned int i = 0; str[i] != '\0'; ++i) {
		chr = *(str + i);
		if ((chr & 0x80) != 0 && nBytes == 0) {// 判断是否ASCII编码,如果不是,说明有可能是GBK
			bAllAscii = false;
		}
		if (nBytes == 0) {
			if (chr >= 0x80) {
				if (chr >= 0x81 && chr <= 0xFE) {
					nBytes = +2;
				}
				else {
					return false;
				}
				nBytes--;
			}
		}
		else {
			if (chr < 0x40 || chr>0xFE) {
				return false;
			}
			nBytes--;
		}//else end
	}
	if (nBytes != 0) {   //违返规则
		return false;
	}
	if (bAllAscii) { //如果全部都是ASCII, 也是GBK
		return true;
	}
	return true;
}

inline string utf8tolocal(const string& utf8_str)
{
	string localstr = utf8_str;
	if (isutf8(localstr))
	{
		localstr = std::filesystem::path(localstr).string();
	}
	return localstr;
}

inline string localtoutf8(const string& localstr)
{
	string utf8_str = localstr;
	if (!isutf8(utf8_str))
	{
		utf8_str = utf8(utf8_str);
	}
	return utf8_str;
}

inline string WstringToString(const wstring& ws)
{
	size_t   i;
	string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char* _Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs_s(&i, _Dest, _Dsize, _Source, _Dsize);
	string result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}
inline wstring StringToWstring(const string& s)
{
	size_t   i;
	setlocale(LC_ALL, "");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t* _Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs_s(&i, _Dest, _Dsize, _Source, _Dsize);
	wstring result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, "");
	return result;
}
inline tstring StrToTStr(const string& str)
{
#ifdef UNICODE
	return StringToWstring(str);
#else
	return str;
#endif // !UNICODE
}
inline string TStrToStr(const tstring& tstr)
{
#ifdef UNICODE
	return WstringToString(tstr);
#else
	return tstr;
#endif // !UNICODE
}

#ifdef _WIN32
inline std::string utf8(std::string const& strGb2312)
{
	if (strGb2312.empty()) return "";

	// GB2312 -> Unicode
	int wlen = MultiByteToWideChar(CP_ACP, 0, strGb2312.c_str(), -1, nullptr, 0);
	if (wlen <= 0) return "";

	std::vector<wchar_t> wbuff(wlen);
	MultiByteToWideChar(CP_ACP, 0, strGb2312.c_str(), -1, wbuff.data(), wlen);

	// Unicode -> UTF-8
	int len = WideCharToMultiByte(CP_UTF8, 0, wbuff.data(), -1, nullptr, 0, nullptr, nullptr);
	if (len <= 0) return "";

	std::vector<char> buff(len);
	WideCharToMultiByte(CP_UTF8, 0, wbuff.data(), -1, buff.data(), len, nullptr, nullptr);

	return std::string(buff.data());
}

inline std::string gb2312(std::string const& strUtf8)
{
	if (strUtf8.empty()) return "";

	// UTF-8 -> Unicode
	int wlen = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, nullptr, 0);
	if (wlen <= 0) return "";

	std::vector<wchar_t> wbuff(wlen);
	MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, wbuff.data(), wlen);

	// Unicode -> GB2312
	int len = WideCharToMultiByte(CP_ACP, 0, wbuff.data(), -1, nullptr, 0, nullptr, nullptr);
	if (len <= 0) return "";

	std::vector<char> buff(len);
	WideCharToMultiByte(CP_ACP, 0, wbuff.data(), -1, buff.data(), len, nullptr, nullptr);

	return std::string(buff.data());
}
#endif