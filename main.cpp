#include <iostream>

#include "BetterWinAPI.h"

// int main()
// {
	// auto *file = new BetterWinAPI::HFile("Hello.txt",
	// 	BetterWinAPI::FileDesiredAccess::write,
	// 	BetterWinAPI::FileShareMode::exclusive,
	// 	BetterWinAPI::FileCreationDisposition::create_new,
	// 	BetterWinAPI::FileFlagsAndAttributes::normal_file
	// );
	// delete file;
	//
	// long long result = file->write("Hello World!", 20);
	// if (!(~result))
	// {
	// 	BetterWinAPI::ErrorType e = BetterWinAPI::getLastError();
	// 	std::cerr << "错误: " << e.getErrorCode() << "\n描述: " << e.getErrorMessage() << std::endl;
	//
	// 	return e.getErrorCode();
	//
	// 	// 输出:
	// 	// 错误: 6
	// 	// 描述: 这个句柄神秘消失了, 快找回来吧
	// 	// 进程已结束，退出代码为 6
	// }
	//
	// return 0;
// }

#include <cstring>

int main()
{
	BetterWinAPI::HFile file1("Hello.txt",
		BetterWinAPI::FileDesiredAccess::write,
		BetterWinAPI::FileShareMode::share_read_write,
		BetterWinAPI::FileCreationDisposition::always_create,
		BetterWinAPI::FileFlagsAndAttributes::normal_file
	);

	if (file1.accessFailed())
	{
		BetterWinAPI::ErrorType e = BetterWinAPI::getLastError();
		std::cerr << "代码: " << e.getErrorCode() << "\n描述: " << e.getErrorMessage() << std::endl;
	}

	BetterWinAPI::HFile file2("Hello.txt",
		BetterWinAPI::FileDesiredAccess::read,
		BetterWinAPI::FileShareMode::share_read_write,
		BetterWinAPI::FileCreationDisposition::open_if_exist,
		BetterWinAPI::FileFlagsAndAttributes::normal_file
	);
	if (file2.accessFailed())
	{
		BetterWinAPI::ErrorType e = BetterWinAPI::getLastError();
		std::cerr << "代码: " << e.getErrorCode() << "\n描述: " << e.getErrorMessage() << std::endl;
	}

	// 写入文件
	const char *content = "Hello World!";
	file1.write(content, strlen(content));
	if (file1.accessFailed())
	{
		BetterWinAPI::ErrorType e = BetterWinAPI::getLastError();
		std::cerr << "代码: " << e.getErrorCode() << "\n描述: " << e.getErrorMessage() << std::endl;
	}

	// 读取文件
	char buffer[10001] = {};
	file2.read(buffer, 10000);
	if (file2.accessFailed())
	{
		BetterWinAPI::ErrorType e = BetterWinAPI::getLastError();
		std::cerr << "代码: " << e.getErrorCode() << "\n描述: " << e.getErrorMessage() << std::endl;
	}
	std::cout << buffer << std::endl;

	return 0;
}