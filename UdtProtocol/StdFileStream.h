/*****************************************************************
 *
 * BSD LICENCE (http://www.opensource.org/licenses/bsd-license.php)
 *
 * Copyright (c) 2010, Cory Thomas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   * Neither the name of the <ORGANIZATION> nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************/

#pragma once

#include <iostream>
#include <fstream>

namespace Udt
{
	/// <summary>
	/// Interface to a UDT socket.
	/// </summary>
	public ref class StdFileStream : public System::IO::Stream
	{
	private:

		bool _canRead;
		bool _canWrite;
		bool _canSeek;
		std::fstream* _stdStream;
		FILE* _streamPtr;

		static void CheckFileNotExists(const wchar_t* path, int shareFlag);

		void Init(System::String^ path, System::IO::FileMode mode, System::IO::FileAccess access, System::IO::FileShare share);
		FILE* InitCreateNew(const wchar_t* path, System::IO::FileAccess access, int shareFlag);
		FILE* InitCreate(const wchar_t* path, System::IO::FileAccess access, int shareFlag);
		FILE* InitOpen(const wchar_t* path, System::IO::FileAccess access, int shareFlag);
		FILE* InitOpenOrCreate(const wchar_t* path, System::IO::FileAccess access, int shareFlag);
		FILE* InitTruncate(const wchar_t* path, System::IO::FileAccess access, int shareFlag);
		FILE* InitAppend(const wchar_t* path, System::IO::FileAccess access, int shareFlag);

		void AssertNotDisposed();
		
	internal:

		static void CheckLastError(const wchar_t* path);
		std::fstream& LoadStdStream(void);

	public:

        /// <summary>Initializes a new instance of the <see cref="StdFileStream" /> class with the specified path and creation mode.</summary>
		/// <param name="path">A relative or absolute path for the file that the current FileStream object will encapsulate. </param>
		/// <param name="mode">A <see cref="System::IO::FileMode" /> constant that determines how to open or create the file. </param>
		/// <exception cref="System::ArgumentException">
		///   <paramref name="path" /> is an empty string (""), contains only white space, or contains one or more invalid characters. -or-<paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in an NTFS environment.</exception>
		/// <exception cref="System::NotSupportedException">
		///   <paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in a non-NTFS environment.</exception>
		/// <exception cref="System::ArgumentNullException">
		///   <paramref name="path" /> is null. </exception>
		/// <exception cref="System::Security::SecurityException">The caller does not have the required permission. </exception>
		/// <exception cref="System::IO::FileNotFoundException">The file cannot be found, such as when <paramref name="mode" /> is FileMode.Truncate or FileMode.Open, and the file specified by <paramref name="path" /> does not exist. The file must already exist in these modes. </exception>
		/// <exception cref="System::IO::IOException">An I/O error, such as specifying FileMode.CreateNew when the file specified by <paramref name="path" /> already exists, occurred.-or-The stream has been closed. </exception>
		/// <exception cref="System::IO::DirectoryNotFoundException">The specified path is invalid, such as being on an unmapped drive. </exception>
		/// <exception cref="System::IO::PathTooLongException">The specified path, file name, or both exceed the system-defined maximum length. For example, on Windows-based platforms, paths must be less than 248 characters, and file names must be less than 260 characters. </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		///   <paramref name="mode" /> contains an invalid value. </exception>
		StdFileStream(System::String^ path, System::IO::FileMode mode);

		/// <summary>Initializes a new instance of the <see cref="StdFileStream" /> class with the specified path, creation mode, and read/write permission.</summary>
		/// <param name="path">A relative or absolute path for the file that the current FileStream object will encapsulate. </param>
		/// <param name="mode">A <see cref="System::IO::FileMode" /> constant that determines how to open or create the file. </param>
		/// <param name="access">A <see cref="System::IO::FileAccess" /> constant that determines how the file can be accessed by the FileStream object. This gets the <see cref="StdFileStream::CanRead" /> and <see cref="StdFileStream::CanWrite" /> properties of the FileStream object. <see cref="StdFileStream::CanSeek" /> is true if <paramref name="path" /> specifies a disk file. </param>
		/// <exception cref="System::ArgumentNullException">
		///   <paramref name="path" /> is null. </exception>
		/// <exception cref="System::ArgumentException">
		///   <paramref name="path" /> is an empty string (""), contains only white space, or contains one or more invalid characters. -or-<paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in an NTFS environment.</exception>
		/// <exception cref="System::NotSupportedException">
		///   <paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in a non-NTFS environment.</exception>
		/// <exception cref="System::ArgumentException">
		///   <paramref name="path" /> is an empty string (""), contains only white space, or contains one or more invalid characters. </exception>
		/// <exception cref="System::IO::FileNotFoundException">The file cannot be found, such as when <paramref name="mode" /> is FileMode.Truncate or FileMode.Open, and the file specified by <paramref name="path" /> does not exist. The file must already exist in these modes. </exception>
		/// <exception cref="System::IO::IOException">An I/O error, such as specifying FileMode.CreateNew when the file specified by <paramref name="path" /> already exists, occurred. -or-The stream has been closed.</exception>
		/// <exception cref="System::Security::SecurityException">The caller does not have the required permission. </exception>
		/// <exception cref="System::IO::DirectoryNotFoundException">The specified path is invalid, such as being on an unmapped drive. </exception>
		/// <exception cref="System::UnauthorizedAccessException">The <paramref name="access" /> requested is not permitted by the operating system for the specified <paramref name="path" />, such as when <paramref name="access" /> is Write or ReadWrite and the file or directory is set for read-only access. </exception>
		/// <exception cref="System::IO::PathTooLongException">The specified path, file name, or both exceed the system-defined maximum length. For example, on Windows-based platforms, paths must be less than 248 characters, and file names must be less than 260 characters. </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		///   <paramref name="mode" /> contains an invalid value. </exception>
		StdFileStream(System::String^ path, System::IO::FileMode mode, System::IO::FileAccess access);

		/// <summary>Initializes a new instance of the <see cref="StdFileStream" /> class with the specified path, creation mode, read/write permission, and sharing permission.</summary>
		/// <param name="path">A relative or absolute path for the file that the current FileStream object will encapsulate. </param>
		/// <param name="mode">A <see cref="System::IO::FileMode" /> constant that determines how to open or create the file. </param>
		/// <param name="access">A <see cref="System::IO::FileAccess" /> constant that determines how the file can be accessed by the FileStream object. This gets the <see cref="StdFileStream::CanRead" /> and <see cref="StdFileStream::CanWrite" /> properties of the FileStream object. <see cref="StdFileStream::CanSeek" /> is true if <paramref name="path" /> specifies a disk file. </param>
		/// <param name="share">A <see cref="System::IO::FileShare" /> constant that determines how the file will be shared by processes. </param>
		/// <exception cref="System::ArgumentNullException">
		///   <paramref name="path" /> is null. </exception>
		/// <exception cref="System::ArgumentException">
		///   <paramref name="path" /> is an empty string (""), contains only white space, or contains one or more invalid characters. -or-<paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in an NTFS environment.</exception>
		/// <exception cref="System::NotSupportedException">
		///   <paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in a non-NTFS environment.</exception>
		/// <exception cref="System::ArgumentException">
		///   <paramref name="path" /> is an empty string (""), contains only white space, or contains one or more invalid characters. </exception>
		/// <exception cref="System::IO::FileNotFoundException">The file cannot be found, such as when <paramref name="mode" /> is FileMode.Truncate or FileMode.Open, and the file specified by <paramref name="path" /> does not exist. The file must already exist in these modes. </exception>
		/// <exception cref="System::IO::IOException">An I/O error, such as specifying FileMode.CreateNew when the file specified by <paramref name="path" /> already exists, occurred. -or-The system is running Windows 98 or Windows 98 Second Edition and <paramref name="share" /> is set to FileShare.Delete.-or-The stream has been closed.</exception>
		/// <exception cref="System::Security::SecurityException">The caller does not have the required permission. </exception>
		/// <exception cref="System::IO::DirectoryNotFoundException">The specified path is invalid, such as being on an unmapped drive. </exception>
		/// <exception cref="System::UnauthorizedAccessException">The <paramref name="access" /> requested is not permitted by the operating system for the specified <paramref name="path" />, such as when <paramref name="access" /> is Write or ReadWrite and the file or directory is set for read-only access. </exception>
		/// <exception cref="System::IO::PathTooLongException">The specified path, file name, or both exceed the system-defined maximum length. For example, on Windows-based platforms, paths must be less than 248 characters, and file names must be less than 260 characters. </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// <paramref name="mode" /> contains an invalid value.
		/// - or -
		/// <paramref name="share" /> contains <b>FileShare::Inheritable</b> or <b>FileShare::Delete</b>
		/// </exception>
		StdFileStream(System::String^ path, System::IO::FileMode mode, System::IO::FileAccess access, System::IO::FileShare share);

		virtual ~StdFileStream(void);

		virtual property bool CanRead {
			bool get(void) override;
		}

		virtual property bool CanSeek {
			bool get(void) override;
		}

		virtual property bool CanWrite {
			bool get(void) override;
		}

		virtual property __int64 Length {
			__int64 get(void) override;
		}

		virtual property __int64 Position {
			__int64 get(void) override;
			void set(__int64 value) override;
		}

		virtual void Flush(void) override;

		virtual void SetLength(__int64 value) override;
		virtual __int64 Seek(__int64 offset, System::IO::SeekOrigin origin) override;

		virtual int Read(cli::array<unsigned char>^ buffer, int offset, int count) override;
		virtual void Write(cli::array<unsigned char>^ buffer, int offset, int count) override;
	};
}
