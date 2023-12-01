/*********************************************************************
 * \file   kcommonfunction.h
 * \brief  常用函数
 *
 * \author jiang
 * \date   2023.06.01
 *********************************************************************/
#pragma once

#include <iostream>
#ifdef _WIN64
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "kfile.h"


#ifdef _WIN64
inline string GetProcessName(HMODULE hModule = NULL)
{
    // 获取当前进程句柄
    HANDLE hProcess = GetCurrentProcess();
    // 获取进程名称
    char szProcessName[MAX_PATH];
    GetModuleFileName(hModule, szProcessName, MAX_PATH);
    //std::cout << "Process name: " << szProcessName << std::endl;
    string process_name = szProcessName;
    process_name = KFile(process_name).file_name;
    return process_name;
}
#else
inline string  GetProcessName()
{
    // 定义一个字符数组用于存储进程名称
    char szProcessName[1024];

    // 使用readlink函数读取/proc/self/exe符号链接指向的路径
    // 并将该路径作为进程名称返回
    ssize_t count = readlink("/proc/self/exe", szProcessName, sizeof(szProcessName));
    if (count != -1)
    {
        szProcessName[count] = '\0';
        std::cout << "Process name: " << szProcessName << std::endl;
    }
    else
    {
        std::cerr << "Failed to get process name." << std::endl;
    }

    return szProcessName;
}
#endif

/**
 * 小数点精度.
 */
inline double Precision(double value, int precision)
{
    stringstream ss;
    ss << fixed << setprecision(precision) << value;
    return stod(ss.str());
}

inline string Precision(string double_value, int precision)
{ 
    return to_string(Precision(stod(double_value), precision));
}

template <typename T>
inline std::string to_string_with_precision(const T a_value, const int precision = 6)
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << a_value;
    return out.str();
}

template <typename T>
inline std::string to_string_with_precision(const T a_value, const int precision, const int set)
{
    std::ostringstream out;
    out << std::setw(set) << std::fixed << std::setprecision(precision) << a_value;
    return out.str();
}

template <typename T>
inline std::string to_string_with_setw(const T a_value, const int set)
{
    std::ostringstream out;
    out << std::setw(set) << setfill('0') << std::fixed << a_value;
    return out.str();
}