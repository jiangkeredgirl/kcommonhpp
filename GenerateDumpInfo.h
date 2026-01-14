/*********************************************************************
 * \file   GenerateDumpInfo.h
 * \brief  windows程序crash时打印crash信息
 *
 * \author jiang
 * \date   2024.12.16
 *********************************************************************/

//#ifndef _M_IX86
//#error "The following code only works for x86!"
//#endif
//#ifndef _M_IX86
//#error "The following code only works for x86!"
//#endif

#pragma once
#include <string>
//#include <Windows.h>
#include <DbgHelp.h>
//#include <char.h>
#include "kcommon.h"

class GenerateDumpInfo
{
public:
    GenerateDumpInfo(){};
    ~GenerateDumpInfo(){};
public:
    static GenerateDumpInfo& instance()
    {
        static GenerateDumpInfo _instance;
        return _instance;
    }

    int Generate(const std::string dumpFileName)
    {
        m_dumpFileName = dumpFileName;
        SetUnhandledExceptionFilter(GenerateDumpInfo::ExceptionFilter);
        return 0;
    }

private:
    static int GenerateMiniDump(PEXCEPTION_POINTERS pExceptionPointers)
    {
        // 定义函数指针
        typedef BOOL(WINAPI * MiniDumpWriteDumpT)(
            HANDLE,
            DWORD,
            HANDLE,
            MINIDUMP_TYPE,
            PMINIDUMP_EXCEPTION_INFORMATION,
            PMINIDUMP_USER_STREAM_INFORMATION,
            PMINIDUMP_CALLBACK_INFORMATION
            );
        // 从 "DbgHelp.dll" 库中获取 "MiniDumpWriteDump" 函数
        MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
        HMODULE hDbgHelp = LoadLibrary("DbgHelp.dll");
        if (NULL == hDbgHelp)
        {
            return EXCEPTION_CONTINUE_EXECUTION;
        }
        pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");


        if (NULL == pfnMiniDumpWriteDump)
        {
            FreeLibrary(hDbgHelp);
            return EXCEPTION_CONTINUE_EXECUTION;
        }
        // 创建 dmp 文件件
        char szFileName[MAX_PATH] = { 0 };
        SYSTEMTIME stLocalTime;
        GetLocalTime(&stLocalTime);
        sprintf(szFileName, "%s-%04d%02d%02d-%02d%02d%02d.dmp",
                (GenerateDumpInfo::instance().m_dumpFileName).c_str(), stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
                stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);
        HANDLE hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
                                      FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
        if (INVALID_HANDLE_VALUE == hDumpFile)
        {
            FreeLibrary(hDbgHelp);
            return EXCEPTION_CONTINUE_EXECUTION;
        }
        // 写入 dmp 文件
        MINIDUMP_EXCEPTION_INFORMATION expParam;
        expParam.ThreadId = GetCurrentThreadId();
        expParam.ExceptionPointers = pExceptionPointers;
        expParam.ClientPointers = FALSE;
        pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
                             hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &expParam : NULL), NULL, NULL);
        // 释放文件
        CloseHandle(hDumpFile);
        FreeLibrary(hDbgHelp);
        return EXCEPTION_EXECUTE_HANDLER;
    }

    static LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
    {
        // 这里做一些异常的过滤或提示
        if (IsDebuggerPresent())
        {
            return EXCEPTION_CONTINUE_SEARCH;
        }
        return GenerateMiniDump(lpExceptionInfo);
    }

private:
    std::string m_dumpFileName;
};

