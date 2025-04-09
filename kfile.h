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
		std::filesystem::path fs_path(utf8tolocal(file_path));
		FileParse(fs_path);
		if (isutf8(file_path))
		{
			UTF8();
		}
	}
	explicit KFile(const string& file_path)
	{
		std::filesystem::path fs_path(utf8tolocal(file_path));
		FileParse(fs_path);
		if (isutf8(file_path))
		{
			UTF8();
		}
	}
	explicit KFile(const std::filesystem::path& fs_path)
	{
		FileParse(fs_path);
	}

public:
	void FileParse(const std::filesystem::path& fs_path)
	{
		file_name     = fs_path.filename().string();
		relative_path = fs_path.relative_path().string();
		absolute_path = std::filesystem::absolute(fs_path).string();
		file_stem     = fs_path.stem().string();
		file_exten    = fs_path.extension().string();
		parent_path   = fs_path.parent_path().string();
		is_directory  = std::filesystem::is_directory(fs_path);
		std::error_code ec;
		file_size = std::filesystem::file_size(fs_path, ec);
		if (ec)
		{
			std::cerr << __FILE__ << " " << __LINE__ << " " << "file path:" << fs_path << " : " << ec.message() << endl;
		}
		else
		{
#if 1
			std::cout << __FILE__ << " " << __LINE__ << " " << "file path:" << fs_path << " size = " << HumanReadable{ std::uintmax_t(file_size) } << endl;
			std::cout << __FILE__ << " " << __LINE__ << " " << "file_name:" << file_name << endl;
			std::cout << __FILE__ << " " << __LINE__ << " " << "relative_path:" << relative_path << endl;
			std::cout << __FILE__ << " " << __LINE__ << " " << "absolute_path:" << absolute_path << endl;
			std::cout << __FILE__ << " " << __LINE__ << " " << "parent_path:" << parent_path << endl;
#endif
		}		
	}
	void UTF8()
	{
		file_name     = localtoutf8(file_name);
		relative_path = localtoutf8(relative_path);
		absolute_path = localtoutf8(absolute_path);
		file_stem     = localtoutf8(file_stem);
		file_exten    = localtoutf8(file_exten);
		parent_path   = localtoutf8(parent_path);
	}
public:
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
public:
	inline static int CreateDirByFile(const string& file_path)
	{
		int error_code = 1;
		std::filesystem::path fs_path = utf8tolocal(file_path);
		do
		{
			std::error_code ec;
			std::filesystem::file_size(fs_path, ec);
			if (ec)
			{
				std::cerr << __FILE__ << " " << __LINE__ << " " << "file path:" << fs_path << " : " << ec.message() << endl;
				break;
			}
			//fs_path.parent_path()会返回文件路径中的文件夹路径。
			//std::error_code ec;
			if (fs_path.parent_path().empty())
			{
				std::cerr << __FILE__ << " " << __LINE__ << " " << "file path:" << fs_path << " parent_path is null " << endl;
				break;
			}
			bool is_success = std::filesystem::create_directories(fs_path.parent_path());
			if (ec)
			{
				std::cerr << __FILE__ << " " << __LINE__ << " " << "file path:" << fs_path << " : " << ec.message() << endl;
			}
			else
			{
				error_code = 0;
#if 1
				std::cout << __FILE__ << " " << __LINE__ << " " << "file path:" << fs_path << " success created" << endl;
#endif
			}
		} while (0);
		return error_code;
	}
	inline static int CreateDir(const string& dir_path)
	{
		int error_code = 1;
		std::filesystem::path fs_path = utf8tolocal(dir_path);
		std::error_code ec;
		std::filesystem::create_directories(fs_path);
		if (ec)
		{
			std::cerr << __FILE__ << " " << __LINE__ << " " << "file path:" << fs_path << " : " << ec.message() << endl;
		}
		else
		{
			error_code = 0;
#if 1
			std::cout << __FILE__ << " " << __LINE__ << " " << "file path:" << fs_path << " success created" << endl;
#endif
		}
		return error_code;
	}
	inline static int CreateFileCover(const string& file_path)
	{
		int error_code = 0;
		std::filesystem::path fs_path = utf8tolocal(file_path);
		error_code = CreateDirByFile(file_path);
		if (error_code == 0)
		{
			ofstream file(fs_path, std::ios::out | std::ios::trunc);
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
		std::filesystem::path fs_path = utf8tolocal(file_path);
		error_code = CreateDirByFile(file_path);
		if (error_code == 0)
		{
			ofstream file(fs_path, std::ios::out | std::ios::app);
			if (!file.is_open())
			{
				error_code = 1;
			}
		}
		return error_code;
	}
	inline static int DelDir(const string& dir_path)
	{
		int error_code = 1;
		std::filesystem::path fs_path = utf8tolocal(dir_path);
		std::uintmax_t deleted_number = std::filesystem::remove_all(fs_path);
		if (deleted_number > 0)
		{
			error_code = 0;
		}
		return error_code;
	}
	inline static int DelFile(const string& file_path)
	{
		int error_code = 1;
		std::filesystem::path fs_path = utf8tolocal(file_path);
		bool is_deleted = std::filesystem::remove(fs_path);
		if (is_deleted)
		{
			error_code = 0;
		}
		return error_code;
	}
	inline static bool IsDirectory(const string& dir_path)
	{
		std::filesystem::path fs_path = utf8tolocal(dir_path);
		return std::filesystem::is_directory(fs_path);
	}
	inline static bool IsExist(const string& file_path)
	{
		std::filesystem::path fs_path = utf8tolocal(file_path);
		return std::filesystem::exists(fs_path);
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
		error_code = fopen_s(&file, filename.c_str(), "wb+");
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
	inline static int WriteFileAppend(__in const string& filename, __out const string& file_content)
	{
		int error_code = -1;
		FILE* file = nullptr;
		error_code = fopen_s(&file, filename.c_str(), "ab+");
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

	inline static string CheckPathNameDuplicate(const string& path_name)
	{
		string new_path_name = path_name;
		int index = 0;
		while (KFile::IsExist(new_path_name))
		{
			++index;
			KFile kfile(path_name);
			new_path_name = kfile.parent_path + "\\" + kfile.file_stem + "_" + to_string(index) + kfile.file_exten;
		}
		return new_path_name;
	}
	inline static string CheckFileNameDuplicate(const string& file_name, const set<string> file_names)
	{
		string new_file_name = file_name;
		int index = 0;
		while (file_names.count(new_file_name))
		{
			++index;
			KFile kfile(file_name);
			new_file_name = kfile.file_stem + "_" + to_string(index) + kfile.file_exten;
		}
		return new_file_name;
	}
	inline static string CheckNameDuplicate(const string& name, const set<string> names)
	{
		string new_name = name;
		int index = 0;
		while (names.count(new_name))
		{
			++index;
			new_name = name + "_" + to_string(index);
		}
		return new_name;
    }

    inline static bool iSValidFolderPath(const std::string& folder_path)
    {
        if (contains_illegal_windows_chars(folder_path)) {
            std::cerr << "包含非法字符，不能用于文件/文件夹名。" << std::endl;
            return false;
        }

        try {
            fs::path path(utf8tolocal(folder_path));

            // 检查父目录是否存在（如果需要的话）
            if (path.has_parent_path() && !fs::exists(path.parent_path())) {
                std::cerr << "上级目录不存在" << std::endl;
                return false;
            }

            // 尝试创建目录（临时测试）
            if (!fs::exists(path)) {
                bool created = fs::create_directory(path);
                if (created) {
                    fs::remove(path);  // 清理测试用目录
                    return true;
                } else {
                    return false;
                }
            } else {
                // 已存在，不能重复创建
                return false;
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "路径异常：" << e.what() << std::endl;
            return false;
        }
    }

    inline static bool is_valid_folder_path(const std::string& folder_path) {
        if (contains_illegal_windows_chars(folder_path)) {
            std::cerr << "包含非法字符，不能用于文件/文件夹名。" << std::endl;
            return false;
        }

        try {
            fs::path path(folder_path);
            if (!fs::exists(path)) {
                fs::create_directory(path);
                fs::remove(path);  // 清理临时创建
            }
            return true;
        } catch (const fs::filesystem_error& e) {
            std::cerr << "文件系统异常: " << e.what() << std::endl;
            return false;
        }
    }

    inline static bool contains_illegal_windows_chars(const std::string& path) {
        const std::string illegal = "\\/:*?\"<>|";
        return path.find_first_of(illegal) != std::string::npos;
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
			std::filesystem::path fs_path = utf8tolocal(root_path);
			std::cout << __FILE__ << " " << __LINE__ << " " << "file path:" << fs_path << endl;
			
			KFile file(fs_path);
			dir.file_name     = file.file_name;
			dir.relative_path = file.relative_path;
			dir.absolute_path = file.absolute_path;
			dir.file_stem     = file.file_stem;
			dir.file_exten    = file.file_exten;
			dir.parent_path   = file.parent_path;
			dir.is_directory  = file.is_directory;
			dir.file_size     = file.file_size;
			if (isutf8(root_path))
			{
				file.UTF8();
			}
			
			for (auto& itr : std::filesystem::directory_iterator(fs_path))
			{
				if (itr.is_directory())
				{
					KDirectory sub_dirs;	
					string sub_root_path = itr.path().string();
					if (isutf8(root_path))
					{
						sub_root_path = localtoutf8(sub_root_path);
					}
					FindFiles(sub_root_path, sub_dirs);
					dir.sub_dirs.push_back(sub_dirs);
				}
				else if (itr.is_regular_file())
				{
					KFile file(itr.path());
					if (isutf8(root_path))
					{
						file.UTF8();
					}
					dir.sub_files.push_back(file);
				}
			}
		} while (false);
		return 0;
	}
	inline static int FindOnlyFiles(const string& root_path, list<KFile>& files)
	{
		std::filesystem::path fs_path = utf8tolocal(root_path);
		std::cout << __FILE__ << " " << __LINE__ << " " << "file path:" << fs_path << endl;
		
		for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(fs_path, std::filesystem::directory_options::skip_permission_denied))
		{
			if (dir_entry.is_regular_file())
			{
				KFile file;
				file.file_name     = dir_entry.path().filename().string();
				file.relative_path = dir_entry.path().string();
				file.absolute_path = std::filesystem::absolute(dir_entry.path()).string();
				file.file_stem     = dir_entry.path().stem().string();
				file.file_exten    = dir_entry.path().extension().string();
				file.parent_path   = dir_entry.path().parent_path().string();
				file.file_size     = std::filesystem::file_size(dir_entry.path());
				file.is_directory  = std::filesystem::is_directory(dir_entry.path());			
				if (isutf8(root_path))
				{
					file.UTF8();
				}		
				files.push_back(file);
			}
		}
		return 0;
	}
};

