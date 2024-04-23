// main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CodeFormat.h"
#include "kfile.h"
#include "ktimer.h"
#include "Coroutine.h"
#include "../kprojecthpp/kerrorcode.h"
#include "../kprojecthpp/kmacro.h"

void kfileTest()
{
	std::string utf8Str = "这是一个UTF-8编码的字符串";
	std::string nonUTF8Str = "This is not a UTF-8 encoded string";

	if (isutf8(utf8Str))
	{
		std::cout << std::filesystem::u8path(utf8Str) << " is " << "UTF-8 encoded" << std::endl;
		std::cout << std::filesystem::u8path(utf8Str).string() << " is " << "UTF-8 encoded" << std::endl;
		std::cout <<             utf8tolocal(utf8Str) << " is " << "UTF-8 encoded" << std::endl;
		std::cout <<                         utf8Str << " is " << "UTF-8 encoded" << std::endl;
	}
	else
	{
		std::cout << utf8Str << " is " << "not UTF-8 encoded" << std::endl;
	}
	if (isutf8(nonUTF8Str))
	{
		std::cout << std::filesystem::u8path(nonUTF8Str).string() << " is " << "UTF-8 encoded" << std::endl;
		std::cout <<             utf8tolocal(nonUTF8Str) << " is " << "UTF-8 encoded" << std::endl;
		std::cout <<                         nonUTF8Str << " is " << "UTF-8 encoded" << std::endl;
	}
	else
	{
		std::cout << nonUTF8Str << " is " << "not UTF-8 encoded" << std::endl;
	}

	if (is_str_utf8(utf8Str.c_str()))
	{
		std::cout << utf8tolocal(utf8Str) << " is " << "UTF-8 encoded" << std::endl;
	}
	else
	{
		std::cout << utf8Str << " is " << "not UTF-8 encoded" << std::endl;
	}
	if (is_str_utf8(nonUTF8Str.c_str()))
	{
		std::cout << utf8tolocal(nonUTF8Str) << " is " << "UTF-8 encoded" << std::endl;
	}
	else
	{
		std::cout << nonUTF8Str << " is " << "not UTF-8 encoded" << std::endl;
	}

	KFile kfile("随便一个中文路径");
	KFile::CreateDirByFile("随便一个中文路径");
	KFile::CreateDir("随便一个中文路径");

	list<KFile> files;
	string folder_path = "E:/KFile测试文件夹";	
	std::cout << "folder_path:" << utf8tolocal(folder_path) << endl;
	KDirectory::FindOnlyFiles(folder_path, files);
	for (auto file : files)
	{
		std::cout << "absolute_path:" << utf8tolocal(file.absolute_path) << endl;
		std::cout << "  parent_path:" << utf8tolocal(file.parent_path) << endl;
		std::cout << "    file_name:" << utf8tolocal(file.file_name) << endl;
	}
}

void ktimerTest()
{
	KMilliTimer ktimer;
	std::cout << " first elapsed time is " << ktimer.ClockElapsed() << std::endl;
	ktimer.Sleep(100);
	std::cout << " second elapsed time is " << ktimer.ClockElapsed() << std::endl;
	ktimer.Sleep(1000);
	std::cout << " third elapsed time is " << ktimer.ClockElapsed() << std::endl;
}

void CoroutineTest()
{
	Coroutine::test();
	std::cout << "10.come back to caller becuase of co_await awaiter\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

void ErrorCodeTest()
{

	std::cout << MACRO_NAME_STR(ERRORCODE_FAIL) << " value: "<< MACRO_VALUE_STR(ERRORCODE_FAIL) << endl;
	std::cout << MACRO_NAME_VALUE_STR(ERRORCODE_FAIL)<< endl;
	std::cout << ErrorCodeStr(ERRORCODE_FAIL) << " error level is: " << ErrorLevelStr(ErrorLevel(ERRORCODE_FAIL)) << endl;
	int errorcode = ERRORCODE_FAIL;
	std::cout << ENUM_NAME_STR(errorcode) << " value: " << ENUM_VALUE_STR(errorcode) << endl;
	std::cout << ENUM_NAME_VALUE_STR(errorcode) << endl;
	std::cout << ErrorCodeStr(errorcode) << " error level is: " << ErrorLevelStr(ErrorLevel(errorcode)) << endl;

	//std::cout << ENUM_VALUE_STR(ERROR_LEVEL_ERROR) << endl;
	//std::cout << ENUM_NAME_VALUE_STR(ERROR_LEVEL_ERROR) << endl;
}

void MacroTest()
{
	std::cout << "macro name:" << MACRO_NAME_STR(PI) << endl;
	std::cout << "macro value:" << MACRO_VALUE_STR(PI) << endl;
	std::cout << "macro name value:" << MACRO_NAME_VALUE_STR(PI) << endl;
	
	std::cout << MACRO_VALUE_STR(MP3) << endl;
	std::cout << MACRO_NAME_STR(MP3, MP3) << endl;

	std::cout << MacroStr(YEARDAYS) << endl;
	std::cout << EnumStr(ENUM_STATE_MACHINE_1) << endl;
	std::cout << EnumStr(ENUM_STATE_MACHINE_2) << endl;
	std::cout << EnumStr(ENUM_STATE_MACHINE_3) << endl;
	int value = YEARDAYS;
	std::cout << MacroStr(value) << endl;
	value = ENUM_STATE_MACHINE_1;
	std::cout << EnumStr(value) << endl;
	value = ENUM_STATE_MACHINE_2;
	std::cout << EnumStr(value) << endl;
	value = ENUM_STATE_MACHINE_3;
	std::cout << EnumStr(value) << endl;
}

int main(int argc, char* argv[]) 
{
	std::cout << "Hello World!\n";

#if 0
	std::cout << "kfileTest!\n";
	kfileTest();
#endif

#if 0
	std::cout << "ktimerTest!\n";
	ktimerTest();
#endif

#if 0
	std::cout << "CoroutineTest!\n";
	CoroutineTest();
#endif

#if 1
	std::cout << "ErrorCodeTest!\n";
	ErrorCodeTest();
#endif

#if 1
	std::cout << "MacroTest!\n";
	MacroTest();
#endif

	return 0;
}
