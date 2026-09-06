#ifndef BWA_ERROR_H
#define BWA_ERROR_H

#include <string>
#include <unordered_map>
#include <windows.h>

namespace BetterWinAPI
{
	enum class ErrorValue_ : DWORD
	{
		no_error = ERROR_SUCCESS,
		error_file_not_found = ERROR_FILE_NOT_FOUND,
		error_access_denied = ERROR_ACCESS_DENIED,
		error_sharing_violation = ERROR_SHARING_VIOLATION,
		error_service_not_active = ERROR_SERVICE_NOT_ACTIVE,
		error_file_exists = ERROR_FILE_EXISTS,
		error_invalid_handle = ERROR_INVALID_HANDLE,
		error_handle_eof = ERROR_HANDLE_EOF,
		error_too_many_open_files = ERROR_TOO_MANY_OPEN_FILES,
		error_write_protect = ERROR_WRITE_PROTECT,
		error_write_fault = ERROR_WRITE_FAULT,
		error_read_fault = ERROR_READ_FAULT,
		error_handle_disk_full = ERROR_HANDLE_DISK_FULL,
		error_lock_violation = ERROR_LOCK_VIOLATION,
		error_sharing_buffer_exceeded = ERROR_SHARING_BUFFER_EXCEEDED,
		// TODO: 找更多的Error code, 回头一起添在这玩意里面(我太懒了, 懒得一次性全写好放里面)
	};

	class ErrorType
	{
	private:
		ErrorValue_ err_code_;
		std::string err_msg_;
		static std::unordered_map<ErrorValue_, std::string> error_map_;
	public:
		ErrorType() = delete;
		ErrorType(DWORD err_code_)
		{
			this->err_code_ = static_cast<ErrorValue_>(err_code_);
			auto it = ErrorType::error_map_.find(this->err_code_);
			if (it != ErrorType::error_map_.end())
				this->err_msg_ = it->second;
			else
			{
				char *buffer = nullptr;
				DWORD size = FormatMessageA(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					static_cast<DWORD>(this->err_code_),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					reinterpret_cast<LPSTR>(&buffer),
					0,
					NULL
				);

				if (size > 0)
				{
					this->err_msg_ = std::string(buffer, size);
					LocalFree(buffer);

					while (!this->err_msg_.empty() && (this->err_msg_.back() == '\n' || this->err_msg_.back() == '\r')) {
						this->err_msg_.pop_back();
					}
				}
				else
					this->err_msg_ = std::to_string(static_cast<DWORD>(this->err_code_));
			}
		}

		ErrorType(const ErrorType& other)
			: err_code_(other.err_code_),
			  err_msg_(other.err_msg_)
		{
		}

		ErrorType(ErrorType&& other) noexcept
			: err_code_(other.err_code_),
			  err_msg_(std::move(other.err_msg_))
		{
		}

		ErrorType& operator=(const ErrorType& other)
		{
			if (this == &other)
				return *this;
			err_code_ = other.err_code_;
			err_msg_ = other.err_msg_;
			return *this;
		}

		ErrorType& operator=(ErrorType&& other) noexcept
		{
			if (this == &other)
				return *this;
			err_code_ = other.err_code_;
			err_msg_ = std::move(other.err_msg_);
			return *this;
		}

		[[nodiscard]] DWORD getErrorCode() const noexcept
		{
			return static_cast<DWORD>(this->err_code_);
		}

		[[nodiscard]] ErrorValue_ getErrorEnum() const noexcept
		{
			return this->err_code_;
		}

		[[nodiscard]] const std::string &getErrorMessage() const noexcept
		{
			return this->err_msg_;
		}
		friend std::string codeToMessage(DWORD code);
	};

#ifdef BWA_JOKE // 废案, 当然, 你也可以手动#define BWA_JOKE, 但是这样的话，需要你自己优化代码
	[[nodiscard]] inline std::string codeToMessage(DWORD code)
	{
		std::string err_msg_;
		auto it = ErrorType::error_map_.find(static_cast<ErrorValue_>(code));
		if (it != ErrorType::error_map_.end())
			return it->second;
		char *buffer = nullptr;
		DWORD size = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			static_cast<DWORD>(code),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&buffer),
			0,
			NULL
		);
		if (size > 0)
		{
			err_msg_ = std::string(buffer, size);
			LocalFree(buffer);

			while (!err_msg_.empty() && (err_msg_.back() == '\n' || err_msg_.back() == '\r')) {
				err_msg_.pop_back();
			}
		}
		else
			err_msg_ = std::to_string(code);

		return err_msg_;
	}
#endif

	inline std::unordered_map<ErrorValue_, std::string> ErrorType::error_map_ = {
		{ErrorValue_::no_error, "亲, 这边没有任何错误，不要多疑哦~"},
		{ErrorValue_::error_file_not_found, "文件跑路了, 我找不到了QWQ"},
		{ErrorValue_::error_file_exists, "已经存在的那个文件: 世界孤立我, 任他奚落~"},
		{ErrorValue_::error_access_denied, "系统说你没实力就别瞎掺和, 搁这看你没权限访问的文件"},
		{ErrorValue_::error_sharing_violation, "你先让其他进程别他妈搁这占用这个文件再说吧"},
		{ErrorValue_::error_service_not_active, "你先等等再开始, 我还没准备好呢"},
		{ErrorValue_::error_invalid_handle, "这个句柄神秘消失了, 快找回来吧"},
		{ErrorValue_::error_handle_eof, "啊哦, 已经到文件末尾了, 设置一下句柄重头开始读吧"},
		{ErrorValue_::error_too_many_open_files, "你打开的文件有那么亿点点多啊, 关掉点再来吧"},
		{ErrorValue_::error_write_protect, "主人说了这里不准你写东西喵~(细节猫娘语doge)"},
		{ErrorValue_::error_write_fault, "又是写数据的一天啊... 等一下, 为什么我数据写不进去了?!(恼"},
		{ErrorValue_::error_read_fault, "元气满满的一天从读取数据开始... 不是哥们，我硬盘怎么坏了?!!!!!!!"},
		{ErrorValue_::error_handle_disk_full, "你硬盘满了塞不下东西了, 赶紧删掉一些\"学习资料\"吧(doge)"},
		{ErrorValue_::error_lock_violation, "阿巴阿巴你文件怎么被锁了? 反正我不知道"},
		{ErrorValue_::error_sharing_buffer_exceeded, "不是哥们, 你他妈的是不是把硬盘里所有文件都开共享模式了啊?!"}
	};

	inline ErrorType getLastError() noexcept
	{
		return {GetLastError()};
	}
}

#endif