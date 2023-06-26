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

