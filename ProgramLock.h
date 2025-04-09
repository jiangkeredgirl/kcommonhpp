/*********************************************************************
 * \file   ProgramLock.h
 * \brief  程序锁，防止程序重复启动
 *
 * \author jiang
 * \date   2024.11.27
 *********************************************************************/
#pragma once

#include <iostream>
#include <mutex>
#include <thread>
#include <stdexcept>
#include <exception>
#include <chrono>
#include <atomic>
#include <iostream>
#include <windows.h>


class ProgramLock
{
public:
    ProgramLock()
    {
        // 创建互斥体
        m_hMutex = CreateMutex(NULL, TRUE, TEXT("UniqueProgramMutex"));
    }
    ~ProgramLock()
    {
        // 释放互斥体
        CloseHandle(m_hMutex);
    }    
    bool isProgramRunning()
    {
        bool is_runing = false;
        // 如果互斥体已被获取，则说明程序已经在运行
        if (GetLastError() == ERROR_ALREADY_EXISTS)
        {
            std::cout << "程序已经在运行！" << std::endl;
            is_runing =  1;  // 如果程序已在运行，退出
        }
        return is_runing;
    }

private:
    HANDLE m_hMutex;
};

#if 0
void runProgram() {
    std::cout << "程序正在运行..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));  // 模拟程序运行
    std::cout << "程序运行完毕。" << std::endl;
}

int main() {
    try {
        ProgramLock lock;  // 程序锁定，防止重复启动

        runProgram();  // 模拟程序的主逻辑

    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;  // 出现异常时返回错误码
    }

    return 0;  // 程序正常退出
}
#endif
