/*********************************************************************
 * \file   FileLock.h
 * \brief  程序的文件锁，防止程序重复启动
 *
 * \author jiang
 * \date   2023.06.01
 *********************************************************************/
#pragma once

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <exception>
#include <cstdio>
#include <cstdlib>

class FileLock
{
public:
    explicit FileLock(const std::string& lockfile) : lockfile_(lockfile)
    {
        lockFile_.open(lockfile_, std::ios::out | std::ios::app);
        if (lockFile_.fail()) {
            throw std::runtime_error("无法创建锁文件！");
        }
        lockFile_.close();

        lockFile_ = std::fstream(lockfile_, std::ios::in | std::ios::out);
        if (lockFile_.fail()) {
            throw std::runtime_error("程序已经在运行！");
        }
    }
    ~FileLock()
    {
        // 程序结束时删除锁文件
        std::remove(lockfile_.c_str());
    }

private:
    std::string lockfile_;
    std::fstream lockFile_;
};


#if 0
void runProgram() {
    std::cout << "程序正在运行..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));  // 模拟程序运行
    std::cout << "程序运行完毕。" << std::endl;
}

int main() {
    try {
        FileLock lock("program.lock");  // 使用文件锁防止程序重复启动
        runProgram();  // 执行程序逻辑
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;  // 出现异常时返回错误码
    }

    return 0;  // 程序正常退出
}
#endif
