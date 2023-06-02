/*********************************************************************
 * \file   FileAssist.h
 * \brief  文件相关函数
 * 
 * \author jiang
 * \date   2023.06.01
 *********************************************************************/
#pragma once

#include <string>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
using namespace std;
namespace fs = std::filesystem;

struct HumanReadable
{
	std::uintmax_t size{};
private: friend
	std::ostream& operator<<(std::ostream& os, HumanReadable hr)
{
	int i{};
	double mantissa = hr.size;
	for (; mantissa >= 1024.; mantissa /= 1024., ++i) {}
	mantissa = std::ceil(mantissa * 10.) / 10.;
	os << mantissa << "BKMGTPE"[i];
	return i == 0 ? os : os << "B (" << hr.size << ')';
}
};

struct KFile
{
    string file_name;       //< 文件名 "file.txt"
    string relative_path;	//< 文件相对路径 "D:\file.txt"
    string absolute_path;   //< 文件绝对路径 ".\file.txt"
    string file_exten;      //< 文件扩展名 ".txt" 
    bool is_directory;      //< 是否是文件夹
    int64_t file_size;      //< 文件大小 单位 字节
    
    KFile()
    {

    }
    KFile(const string& file_path)
    {
        std::filesystem::path fs_path(file_path);
        file_name      = fs_path.filename().string();
        relative_path  = fs_path.relative_path().string();
        absolute_path  = std::filesystem::absolute(fs_path).string();
        file_exten     = fs_path.extension().string();
        is_directory   = std::filesystem::is_directory(fs_path);
        std::error_code ec;
        file_size = std::filesystem::file_size(fs_path, ec);
        if (ec) 
        {
            std::cout << "KFile:" << fs_path << " : " << ec.message() << '\n';
        }
        else 
        {
            std::cout << "KFile:" << fs_path << " size = " << HumanReadable{ std::uintmax_t(file_size) } << '\n';
        }
    }
    KFile(const std::filesystem::path& fs_path)
    {
        file_name = fs_path.filename().string();
        relative_path = fs_path.relative_path().string();
        absolute_path = std::filesystem::absolute(fs_path).string();
        file_exten = fs_path.extension().string();
        is_directory = std::filesystem::is_directory(fs_path);
        std::error_code ec;
        file_size = std::filesystem::file_size(fs_path, ec);
        if (ec)
        {
            std::cout << "KFile:" << fs_path << " : " << ec.message() << '\n';
        }
        else
        {
            std::cout << "KFile:" << fs_path << " size = " << HumanReadable{ std::uintmax_t(file_size) } << '\n';
        }
    }
};
struct KDirectory : public KFile
{
    list<KFile> sub_files;      //< 子文件 
    list<KDirectory> sub_dirs;  //< 子文件夹 
};


inline int CreateDirByFile(const string& file_path)
{
    int error_code = 0;
    std::filesystem::path fs_path(file_path);
    //fs_path.parent_path()会返回文件路径中的文件夹路径。
    std::filesystem::create_directories(fs_path.parent_path());
    return error_code;
}

inline int CreateDir(const string& dir_path)
{
    int error_code = 0;
    std::filesystem::create_directories(dir_path);
    return error_code;
}

inline int CreateFile(const string& file_path)
{
    int error_code = 0;
    CreateDirByFile(file_path);
    std::filesystem::path fs_path(file_path);
    string filename = fs_path.filename().string();
    ofstream file(file_path, std::ios::out | std::ios::app);
    if (!file.is_open())
    {
        error_code = 1;
    }
    return error_code;
}

inline int DeleteDir(const string& dir_path)
{
    int error_code = 0;
    std::filesystem::remove_all(dir_path);
    return error_code;
}

inline int DelFile(const string& file_path)
{
    int error_code = 0;
    std::filesystem::remove(file_path);
    return error_code;
}

inline bool IsDirectory(const string& dir_path)
{
    return std::filesystem::is_directory(dir_path);
}

inline bool IsExist(const string& file_path)
{
    return std::filesystem::exists(file_path);
}

inline int FindFiles(const string& root_path, KDirectory& dir)
{
    do
    {
        if (root_path.empty() || root_path == (".") || root_path == (".."))
        {
            break;
        }
        if (!std::filesystem::exists(root_path))
        {
            break;
        }
        
        std::filesystem::path fs_path(root_path);
        KFile file(fs_path);
        dir.file_name     = file.file_name;
        dir.relative_path = file.relative_path;
        dir.absolute_path = file.absolute_path;
        dir.file_exten    = file.file_exten;
        dir.is_directory  = file.is_directory;
        dir.file_size     = file.file_size;
        for (auto& itr : std::filesystem::directory_iterator(fs_path))
        {
            if (itr.is_directory())
            {
                KDirectory sub_dirs;
                FindFiles(itr.path().string(), sub_dirs);
                dir.sub_dirs.push_back(sub_dirs);
            }
            else if (itr.is_regular_file())
            {
                KFile file(itr.path());
                dir.sub_files.push_back(file);
            }
        }
    } while (false);
    return 0;
}

inline int FindOnlyFiles(const string& _root_path, list<KFile>& files)
{
	for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ _root_path })
	{
		if (dir_entry.is_regular_file())
		{
			KFile file;
			file.file_name = dir_entry.path().filename().string();
			file.relative_path = dir_entry.path().string();
			file.absolute_path = std::filesystem::absolute(dir_entry.path()).string();
			file.file_exten = dir_entry.path().extension().string();
			file.file_size = std::filesystem::file_size(dir_entry.path());
			file.is_directory = std::filesystem::is_directory(dir_entry.path());
            files.push_back(file);
		}
	}
    return 0;
}