/*********************************************************************
 * \file   kfile.h
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
#include "CodeFormat.h"
using namespace std;
namespace fs = std::filesystem;


struct KFile
{
	string file_name;       //< 文件名 "file.txt"
	string relative_path;	//< 文件相对路径 "D:\file.txt"
	string absolute_path;   //< 文件绝对路径 ".\file.txt"
	string file_stem;       //< 文件名不带后缀 "file"
	string file_exten;      //< 文件扩展名 ".txt" 
	string parent_path;     //< 上级目录
	bool is_directory;      //< 是否是文件夹
	int64_t file_size;      //< 文件大小 单位 字节

	explicit KFile()
	{

	}
	explicit KFile(const char* file_path)
	{
		KFile(string(file_path));
	}
	explicit KFile(const string& file_path)
	{
		std::filesystem::path fs_path(file_path);
		file_name      = fs_path.filename().string();
		relative_path  = fs_path.relative_path().string();
		absolute_path  = std::filesystem::absolute(fs_path).string();
		file_stem      = fs_path.stem().string();
		file_exten     = fs_path.extension().string();
		parent_path    = fs_path.parent_path().string();
		is_directory   = std::filesystem::is_directory(fs_path);
		std::error_code ec;
		file_size      = std::filesystem::file_size(fs_path, ec);
		if (ec)
		{
			std::cout << __FILE__ << utf8(fs_path.string()) << " : " << ec.message() << '\n';
		}
		else
		{
			std::cout << __FILE__ << utf8(fs_path.string()) << " size = " << HumanReadable{ std::uintmax_t(file_size) } << '\n';
		}
	}
	explicit KFile(const std::filesystem::path& fs_path)
	{
		file_name       = fs_path.filename().string();
		relative_path   = fs_path.relative_path().string();
		absolute_path   = std::filesystem::absolute(fs_path).string();
		file_stem       = fs_path.stem().string();
		file_exten      = fs_path.extension().string();
		is_directory    = std::filesystem::is_directory(fs_path);
		std::error_code ec;
		file_size       = std::filesystem::file_size(fs_path, ec);
		if (ec)
		{
			std::cout << __FILE__ << utf8(fs_path.string()) << " : " << ec.message() << '\n';
		}
		else
		{
			std::cout << __FILE__ << utf8(fs_path.string()) << " size = " << HumanReadable{ std::uintmax_t(file_size) } << '\n';
		}
	}
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
	inline static int CreateDirByFile(const string& file_path)
	{
		int error_code = 1;
		std::filesystem::path fs_path(file_path);
		//fs_path.parent_path()会返回文件路径中的文件夹路径。
		std::error_code ec;
		bool is_success = std::filesystem::create_directories(fs_path.parent_path());
		if (ec)
		{
			std::cout << __FILE__ << fs_path << " : " << ec.message() << '\n';
		}
		else
		{
			error_code = 0;
			std::cout << __FILE__ << fs_path << " success created" << '\n';
		}
		return error_code;
	}
	inline static int CreateDir(const string& dir_path)
	{
		int error_code = 1;
		std::error_code ec;
		std::filesystem::create_directories(dir_path);
		if (ec)
		{
			std::cout << __FILE__ << dir_path << " : " << ec.message() << '\n';
		}
		else
		{
			error_code = 0;
			std::cout << __FILE__ << dir_path << " success created" << '\n';
		}
		return error_code;
	}
	inline static int CreateFileCover(const string& file_path)
	{
		int error_code = 0;
		error_code = CreateDirByFile(file_path);
		if (error_code == 0)
		{
			std::filesystem::path fs_path(file_path);
			string filename = fs_path.filename().string();
			ofstream file(file_path, std::ios::out | std::ios::trunc);
			if (!file.is_open())
			{
				error_code = 1;
			}
		}
		return error_code;
	}
	inline static int CreateFileAppend(const string& file_path)
	{
		int error_code = 0;
		error_code = CreateDirByFile(file_path);
		if (error_code == 0)
		{
			std::filesystem::path fs_path(file_path);
			string filename = fs_path.filename().string();
			ofstream file(file_path, std::ios::out | std::ios::app);
			if (!file.is_open())
			{
				error_code = 1;
			}
		}
		return error_code;
	}
	inline static int DeleteDir(const string& dir_path)
	{
		int error_code = 1;
		std::uintmax_t deleted_number = std::filesystem::remove_all(dir_path);
		if (deleted_number > 0)
		{
			error_code = 0;
		}
		return error_code;
	}
	inline static int DeleteFile(const string& file_path)
	{
		int error_code = 1;
		bool is_deleted = std::filesystem::remove(file_path);
		if (is_deleted)
		{
			error_code = 0;
		}
		return error_code;
	}
	inline static bool IsDirectory(const string& dir_path)
	{
		return std::filesystem::is_directory(dir_path);
	}
	inline static bool IsExist(const string& file_path)
	{
		return std::filesystem::exists(file_path);
	}
	inline static int ReadFile(__in const string& filename, __out string& file_content)
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
	inline static int WriteFile(__in const string& filename, __out const string& file_content)
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
	inline static int PathIsExist(__in const string& path)
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
	inline static string GetFileName(__in const string& path_name)
	{
		string file_name(path_name);
		string::size_type pos = file_name.find_last_of("\\/");
		if (pos != string::npos)
		{
			file_name = file_name.substr(pos + 1);
		}
		return file_name;
	}
	inline static string GetDirectoryName(__in const string& path_name)
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
	inline static string GetWorkPath()
	{
		char path_name[1024 + 1] = { 0 };
		_getcwd(path_name, 1024);
		return path_name;
	}
	inline static string GetProgramPath()
	{
		string fullpath;
		errno_t err = 0;
		if (_wpgmptr != nullptr)
		{
			wchar_t* wpath = nullptr;
			err = _get_wpgmptr(&wpath);
			if (err == 0)
			{
				fullpath = WstringToString(wpath);
			}
		}
		else if (_pgmptr != nullptr)
		{
			char* path = nullptr;
			err = _get_pgmptr(&path);
			if (err == 0)
			{
				fullpath = path;
			}
		}
		return fullpath;
	}
};

struct KDirectory : public KFile
{
	list<KFile> sub_files;      //< 子文件 
	list<KDirectory> sub_dirs;  //< 子文件夹 
	inline static int FindFiles(const string& root_path, KDirectory& dir)
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
			dir.file_name = file.file_name;
			dir.relative_path = file.relative_path;
			dir.absolute_path = file.absolute_path;
			dir.file_exten = file.file_exten;
			dir.is_directory = file.is_directory;
			dir.file_size = file.file_size;
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
	inline static int FindOnlyFiles(const string& _root_path, list<KFile>& files)
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
};

