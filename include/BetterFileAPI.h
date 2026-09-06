#ifndef BETTERFILEAPI_H
#define BETTERFILEAPI_H

#include "Error.h"

#include <stdexcept>
#include <string>
#include <windows.h>

namespace BetterWinAPI
{
	enum class FileDesiredAccess : DWORD
	{
		read = GENERIC_READ,
		write = GENERIC_WRITE,
		read_write = GENERIC_READ | GENERIC_WRITE
	};
	enum class FileShareMode : DWORD
	{
		exclusive = 0,
		share_read = FILE_SHARE_READ,
		share_write = FILE_SHARE_WRITE,
		share_read_write = FILE_SHARE_READ | FILE_SHARE_WRITE
	};
	enum class FileCreationDisposition : DWORD
	{
		create_new = CREATE_NEW,
		always_create = CREATE_ALWAYS,
		open_if_exist = OPEN_EXISTING,
		always_open = OPEN_ALWAYS,
		clear_and_open = TRUNCATE_EXISTING
	};
	enum class FileFlagsAndAttributes : DWORD
	{
		normal_file = FILE_ATTRIBUTE_NORMAL,
		readonly_file = FILE_ATTRIBUTE_READONLY,
		hidden_file = FILE_ATTRIBUTE_HIDDEN,
		directory = FILE_ATTRIBUTE_DIRECTORY,
		no_buffering = FILE_FLAG_NO_BUFFERING,
		sequential = FILE_FLAG_SEQUENTIAL_SCAN,
		random_access = FILE_FLAG_RANDOM_ACCESS,
		write_through = FILE_FLAG_WRITE_THROUGH,
		delete_on_close = FILE_FLAG_DELETE_ON_CLOSE,
	};

	class HFile
	{
	private:
		HANDLE handle_file_;
		std::string lp_filename_;
		FileDesiredAccess dw_desired_access_;
		FileShareMode dw_share_mode_;
		FileCreationDisposition dw_creation_disposition_;
		FileFlagsAndAttributes dw_flags_and_attributes_;
		bool access_failed_;
	public:
		HFile() = delete;

		HFile(const std::string &lp_filename,
			const FileDesiredAccess dw_desired_access = FileDesiredAccess::read_write,
			const FileShareMode dw_share_mode = FileShareMode::exclusive,
			const FileCreationDisposition dw_creation_disposition = FileCreationDisposition::create_new,
			const FileFlagsAndAttributes dw_flags_and_attributes = FileFlagsAndAttributes::normal_file)
			: lp_filename_(std::string(lp_filename)),
			  dw_desired_access_(dw_desired_access),
			  dw_share_mode_(dw_share_mode),
			  dw_creation_disposition_(dw_creation_disposition),
			  dw_flags_and_attributes_(dw_flags_and_attributes)
		{
			this->access_failed_ = false;
			SetLastError(ERROR_SUCCESS);
			this->handle_file_ = CreateFileA(
				this->lp_filename_.c_str(),
				static_cast<DWORD>(this->dw_desired_access_),
				static_cast<DWORD>(this->dw_share_mode_),
				NULL,
				static_cast<DWORD>(this->dw_creation_disposition_),
				static_cast<DWORD>(this->dw_flags_and_attributes_),
				NULL
			);
			this->access_failed_ = (this->handle_file_ == INVALID_HANDLE_VALUE);
		}

		HFile(const HFile& other)
			: lp_filename_(other.lp_filename_),
			  dw_desired_access_(other.dw_desired_access_),
			  dw_share_mode_(other.dw_share_mode_),
			  dw_creation_disposition_(other.dw_creation_disposition_),
			  dw_flags_and_attributes_(other.dw_flags_and_attributes_)
		{
			this->access_failed_ = false;
			SetLastError(ERROR_SUCCESS);

			this->handle_file_ = CreateFileA(
				this->lp_filename_.c_str(),
				static_cast<DWORD>(this->dw_desired_access_),
				static_cast<DWORD>(this->dw_share_mode_),
				NULL,
				static_cast<DWORD>(this->dw_creation_disposition_),
				static_cast<DWORD>(this->dw_flags_and_attributes_),
				NULL
			);
			this->access_failed_ = (this->handle_file_ == INVALID_HANDLE_VALUE);
		}

		HFile(HFile&& other) noexcept
			: handle_file_(other.handle_file_),
			  lp_filename_(std::move(other.lp_filename_)),
			  dw_desired_access_(other.dw_desired_access_),
			  dw_share_mode_(other.dw_share_mode_),
			  dw_creation_disposition_(other.dw_creation_disposition_),
			  dw_flags_and_attributes_(other.dw_flags_and_attributes_),
			  access_failed_(other.access_failed_)
		{
			other.handle_file_ = INVALID_HANDLE_VALUE;
			other.lp_filename_.clear();
		}

		HFile& operator=(const HFile& other)
		{
			this->access_failed_ = false;
			SetLastError(ERROR_SUCCESS);

			if (this == &other)
				return *this;
			lp_filename_ = other.lp_filename_;
			dw_desired_access_ = other.dw_desired_access_;
			dw_share_mode_ = other.dw_share_mode_;
			dw_creation_disposition_ = other.dw_creation_disposition_;
			dw_flags_and_attributes_ = other.dw_flags_and_attributes_;

			if (this->handle_file_ != INVALID_HANDLE_VALUE)
				CloseHandle(this->handle_file_);
			this->handle_file_ = CreateFileA(
				this->lp_filename_.c_str(),
				static_cast<DWORD>(this->dw_desired_access_),
				static_cast<DWORD>(this->dw_share_mode_),
				NULL,
				static_cast<DWORD>(this->dw_creation_disposition_),
				static_cast<DWORD>(this->dw_flags_and_attributes_),
				NULL
			);
			this->access_failed_ = (this->handle_file_ == INVALID_HANDLE_VALUE);
			return *this;
		}

		HFile& operator=(HFile&& other) noexcept
		{
			this->access_failed_ = false;
			SetLastError(ERROR_SUCCESS);
			if (this == &other)
				return *this;

			if (this->handle_file_ != INVALID_HANDLE_VALUE)
				CloseHandle(this->handle_file_);
			handle_file_ = other.handle_file_;
			lp_filename_ = std::move(other.lp_filename_);
			dw_desired_access_ = other.dw_desired_access_;
			dw_share_mode_ = other.dw_share_mode_;
			dw_creation_disposition_ = other.dw_creation_disposition_;
			dw_flags_and_attributes_ = other.dw_flags_and_attributes_;
			this->access_failed_ = other.access_failed_;

			other.handle_file_ = INVALID_HANDLE_VALUE;
			other.lp_filename_.clear();
			return *this;
		}

		~HFile()
		{
			if (this->handle_file_ != INVALID_HANDLE_VALUE)
				CloseHandle(this->handle_file_);
			this->handle_file_ = INVALID_HANDLE_VALUE;	// 依旧好习惯防止野指针飞我这来然后用已经被close的句柄
		}

		long long read(LPVOID buffer, DWORD num_of_byte_to_read)
		{
			this->access_failed_ = false;
			SetLastError(ERROR_SUCCESS);
			if (this->handle_file_ == INVALID_HANDLE_VALUE)
			{
				SetLastError(ERROR_INVALID_HANDLE);
				this->access_failed_ = true;
				return -1;
			}
			DWORD temp_num_of_bytes_read = 0;
			if (!ReadFile(
				this->handle_file_,
				buffer,
				num_of_byte_to_read,
				&temp_num_of_bytes_read,
				NULL
			))
			{
				this->access_failed_ = true;
				return -1;
			}

			return temp_num_of_bytes_read;
		}

		long long write(LPCVOID buffer, DWORD num_of_byte_to_write)
		{
			this->access_failed_ = false;
			SetLastError(ERROR_SUCCESS);
			if (this->handle_file_ == INVALID_HANDLE_VALUE)
			{
				SetLastError(ERROR_INVALID_HANDLE);
				this->access_failed_ = true;
				return -1;
			}
			DWORD temp_num_of_bytes_write = 0;
			if (!WriteFile(
				this->handle_file_,
				buffer,
				num_of_byte_to_write,
				&temp_num_of_bytes_write,
				NULL
			))
			{
				this->access_failed_ = true;
				return -1;
			}
			return temp_num_of_bytes_write;
		}

		[[nodiscard]] bool accessFailed() const noexcept
		{
			return this->access_failed_;
		}
	};
}

#endif