// main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "..\CodeFormat.h"
#include"..\kfile.h"


int main(int argc, char* argv[]) 
{
	std::cout << "Hello World!\n";
	std::string utf8Str = "这是一个UTF-8编码的字符串";
	std::string nonUTF8Str = "This is not a UTF-8 encoded string";

	if (isUTF8(utf8Str))
	{
		std::cout << std::filesystem::u8path(utf8Str)    << " is " << "UTF-8 encoded" << std::endl;
	}
	else
	{
		std::cout << utf8Str    << " is " << "not UTF-8 encoded" << std::endl;
	}
	if (isUTF8(nonUTF8Str))
	{
		std::cout << std::filesystem::u8path(nonUTF8Str) << " is " << "UTF-8 encoded" << std::endl;
	}
	else
	{
		std::cout << nonUTF8Str << " is " << "not UTF-8 encoded" << std::endl;
	}

	if (is_str_utf8(utf8Str.c_str()))
	{
		std::cout << std::filesystem::u8path(utf8Str) << " is " << "UTF-8 encoded" << std::endl;
	}
	else
	{
		std::cout << utf8Str << " is " << "not UTF-8 encoded" << std::endl;
	}
	if (is_str_utf8(nonUTF8Str.c_str()))
	{
		std::cout << std::filesystem::u8path(nonUTF8Str) << " is " << "UTF-8 encoded" << std::endl;
	}
	else
	{
		std::cout << nonUTF8Str << " is " << "not UTF-8 encoded" << std::endl;
	}

	KFile kfile("随便一个中文路径");	
	KFile::CreateDirByFile("随便一个中文路径");
	KFile::CreateDir("随便一个中文路径");

	return 0;
}
