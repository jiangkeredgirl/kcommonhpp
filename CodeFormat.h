/*********************************************************************
 * \file   CodeFormat.h
 * \brief  编码格式转换
 * 
 * \author jiang
 * \date   2023.06.16
 *********************************************************************/
#pragma once

#include <string>
#include <sstream>
#include "cstandard.h"

#ifdef UNICODE
typedef wstring tstring;
#else
typedef  string tstring;
#endif // !UNICODE


inline std::string /*gb2312_to_*/utf8(std::string const& strGb2312)
{
	std::vector<wchar_t> buff(strGb2312.size());
#ifdef _MSC_VER
	std::locale loc("zh-CN");
#else
	std::locale loc("zh_CN.GB18030");
#endif
	wchar_t* pwszNext = nullptr;
	const char* pszNext = nullptr;
	mbstate_t state = {};
	int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >
		(loc).in(state,
			strGb2312.data(), strGb2312.data() + strGb2312.size(), pszNext,
			buff.data(), buff.data() + buff.size(), pwszNext);

	if (std::codecvt_base::ok == res)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
		return cutf8.to_bytes(std::wstring(buff.data(), pwszNext));
	}

	return "";

}

inline std::string /*utf8_to_*/gb2312(std::string const& strUtf8)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
	std::wstring wTemp = cutf8.from_bytes(strUtf8);
#ifdef _MSC_VER
	std::locale loc("zh-CN");
#else
	std::locale loc("zh_CN.GB18030");
#endif
	const wchar_t* pwszNext = nullptr;
	char* pszNext = nullptr;
	mbstate_t state = {};

	std::vector<char> buff(wTemp.size() * 2);
	int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >
		(loc).out(state,
			wTemp.data(), wTemp.data() + wTemp.size(), pwszNext,
			buff.data(), buff.data() + buff.size(), pszNext);

	if (std::codecvt_base::ok == res)
	{
		return std::string(buff.data(), pszNext);
	}
	return "";
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

