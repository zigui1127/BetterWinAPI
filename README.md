# BetterWinAPI
BetterWinAPI 为一个以优化 MinGW WinAPI(即 MinGW windows.h 头文件)为目的而编写的库，其内部目前仅编写了HFile, ErrorType 两个内容, 目前已实现读、写功能

## 用法
``` c++
#include "BetterWinAPI.h"

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

```

## 以及一些说明
### ErrorType
`ErrorType` 为该库内置的一个类，用于存储 `ErrorValue_` 枚举对象. 在一般状态下建议使用 ErrorType 而非 ErrorValue_ 这个供内部使用的枚举类.

ErrorType 可以被拷贝、移动, 和正常的对象一样.

``` c++
BetterWinAPI::ErrorType e = BetterWinAPI::getLastError() // 用于获取最新的错误, 类似 WinAPI 内置的 GetLastError()
std::cerr << e.getErrorCode() << " " << e.getErrorMessage();
/*
 * getErrorCode(): 用于获取 ErrorType 对象内存储的错误代码, 1:1 对照WinAPI
 * getErrorMessage(): 获取错误信息(该库对一些错误信息进行了改编, 那些被改编的信息和原信息相比, 变得不再那么严肃了)
 */
```

### HFile
`HFile` 为内部的一个类, 用于存储文件 `HANDLE` 对象, 已实现拷贝、移动.

注: HFile在拷贝的时候会重新 CreateFile, 而不是使用原句柄, 所以如果需要移动句柄, 请使用移动语义(出于安全着想, 目前没有任何方法实现了复制句柄的功能[^1])

### 另
这个库里面藏着一个彩蛋, 有兴趣的话, 可以看看 Error.h 头文件, 里面有好东西(虽然是废案)

[^1]: 注: 此处的复制句柄是指复制一个指向同一句柄的指针，而非将句柄复制一个备份赋值给另一个指针(但是复制备份的方法我确实也没写进去)