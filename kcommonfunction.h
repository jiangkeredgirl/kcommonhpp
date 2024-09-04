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
    out << std::setprecision(precision) << std::setw(set) << setfill('0') << std::fixed << a_value;
    return out.str();
}

template <typename T>
inline std::string to_string_with_setw(const T a_value, const int set)
{
    std::ostringstream out;
    out << std::setw(set) << std::setfill('0') << std::fixed << a_value;
    return out.str();
}

inline double to_double(const std::string& valueAsString, const int precision = 3, const int set = 6)
{
    istringstream totalSString(valueAsString);
    double valueAsDouble = 0;
    // maybe use some manipulators
    totalSString >> std::setprecision(precision) >> std::setw(set) >> std::fixed >> valueAsDouble;
    if (!totalSString)
        throw std::runtime_error("Error converting to double");
    return valueAsDouble;
}

inline string BytesToString(const std::vector<std::byte>& bytes)
{
    // 输入
#if 0
    std::cout << "Byte array in hex: ";
    for (std::byte b : bytes)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << std::to_integer<int>(b) << " ";
    }
    std::cout << std::endl;
#endif
    // 创建一个 std::vector<std::byte> 并填充一些数据
    //std::vector<std::byte> bytes = {std::byte{0x48}, std::byte{0x65}, std::byte{0x6C}, std::byte{0x6C}, std::byte{0x6F}};  // "Hello" 的 ASCII 值
    // 创建一个 std::string 并将 bytes 转换到字符串
    std::string str;
    str.resize(bytes.size());  // 调整字符串大小以匹配字节数组的大小
    std::transform(bytes.begin(), bytes.end(), str.begin(), [](std::byte b) -> char { return std::to_integer<char>(b); });
    // 输出结果
#if 0
    std::cout << "String: " << str << std::endl;
#endif
    return str;
}

inline std::vector<std::byte> StringToBytes(const string& str)
{
    // 输入
#if 0
    std::cout << "String: " << str << std::endl;
#endif
    // 创建一个 std::string
    //std::string str = "Hello";
    // 创建一个 std::vector<std::byte> 来存储字节数据
    std::vector<std::byte> bytes;
    // 将字符串转换为 std::vector<std::byte>
    bytes.reserve(str.size());  // 预留空间以避免频繁分配内存
    for (char c : str)
    {
        bytes.push_back(std::byte(static_cast<unsigned char>(c)));
    }
    // 输出结果
#if 0
    std::cout << "Byte array in hex: ";
    for (std::byte b : bytes)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << std::to_integer<int>(b) << " ";
    }
    std::cout << std::endl;
#endif
    return bytes;
}

inline string BytesToHexString(const std::vector<std::byte>& bytes)
{
    std::stringstream ss;
    for (std::byte b : bytes)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << std::to_integer<int>(b) << " ";
    }
#if 0
    std::cout << "Byte array in hex: ";
    std::cout << ss.str();
    std::cout << std::endl;
    std::cout << "String: " << ss.str() << std::endl;
#endif
    return ss.str();
}

inline std::vector<std::byte> HexStringToBytes(const string& hexstr)
{
    std::vector<std::byte> bytes;
    std::stringstream ss(hexstr);
    std::string byteStr;
    while (ss >> byteStr)
    {
        // 将每个两字符的十六进制数转换为 byte
        std::byte byteValue = static_cast<std::byte>(std::stoi(byteStr, nullptr, 16));
        bytes.push_back(byteValue);
    }
#if 0
    std::cout << "String: " << hexstr << std::endl;
    std::cout << "Byte array in hex: ";
    for (std::byte b : bytes)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << std::to_integer<int>(b) << " ";
    }
    std::cout << std::endl;
#endif
    return bytes;
}

inline std::string StringToHexString(const std::string& str)
{
    std::vector<std::byte> bytes = StringToBytes(str);
    std::stringstream ss;
    for (std::byte b : bytes)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << std::to_integer<int>(b) << " ";
    }
#if 0
    std::cout << "Byte array in hex: ";
    std::cout << ss.str();
    std::cout << std::endl;
    std::cout << "String: " << ss.str() << std::endl;
#endif
    return ss.str();
}

inline string HexStringToString(const string& hexstr)
{
    std::vector<std::byte> bytes;
    std::stringstream ss(hexstr);
    std::string byteStr;
    while (ss >> byteStr)
    {
        // 将每个两字符的十六进制数转换为 byte
        std::byte byteValue = static_cast<std::byte>(std::stoi(byteStr, nullptr, 16));
        bytes.push_back(byteValue);
    }
#if 0
    std::cout << "String: " << hexstr << std::endl;
    std::cout << "Byte array in hex: ";
    for (std::byte b : bytes)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << std::to_integer<int>(b) << " ";
    }
    std::cout << std::endl;
#endif
    string str = BytesToString(bytes);
    return str;
}

