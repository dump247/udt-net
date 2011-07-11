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

		std::fstream* _stdStream;
		System::IO::FileStream^ _fileStream;

		void Init(System::IO::FileStream^ fileStream);
		
	internal:

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
		///   <paramref name="mode" /> contains an invalid value. </exception>
		StdFileStream(System::String^ path, System::IO::FileMode mode, System::IO::FileAccess access, System::IO::FileShare share);

		/// <summary>Initializes a new instance of the <see cref="StdFileStream" /> class with the specified path, creation mode, read/write and sharing permission, and buffer size.</summary>
		/// <param name="path">A relative or absolute path for the file that the current FileStream object will encapsulate. </param>
		/// <param name="mode">A <see cref="System::IO::FileMode" /> constant that determines how to open or create the file. </param>
		/// <param name="access">A <see cref="System::IO::FileAccess" /> constant that determines how the file can be accessed by the FileStream object. This gets the <see cref="StdFileStream::CanRead" /> and <see cref="StdFileStream::CanWrite" /> properties of the FileStream object. <see cref="StdFileStream::CanSeek" /> is true if <paramref name="path" /> specifies a disk file. </param>
		/// <param name="share">A <see cref="System::IO::FileShare" /> constant that determines how the file will be shared by processes. </param>
		/// <param name="bufferSize">A positive value greater than 0 indicating the buffer size. For <paramref name="bufferSize" /> values between one and eight, the actual buffer size is set to eight bytes. </param>
		/// <exception cref="System::ArgumentNullException">
		///   <paramref name="path" /> is null. </exception>
		/// <exception cref="System::ArgumentException">
		///   <paramref name="path" /> is an empty string (""), contains only white space, or contains one or more invalid characters. -or-<paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in an NTFS environment.</exception>
		/// <exception cref="System::NotSupportedException">
		///   <paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in a non-NTFS environment.</exception>
		/// <exception cref="System::ArgumentException">
		///   <paramref name="path" /> is an empty string (""), contains only white space, or contains one or more invalid characters. </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		///   <paramref name="bufferSize" /> is negative or zero.-or- <paramref name="mode" />, <paramref name="access" />, or <paramref name="share" /> contain an invalid value. </exception>
		/// <exception cref="System::IO::FileNotFoundException">The file cannot be found, such as when <paramref name="mode" /> is FileMode.Truncate or FileMode.Open, and the file specified by <paramref name="path" /> does not exist. The file must already exist in these modes. </exception>
		/// <exception cref="System::IO::IOException">An I/O error, such as specifying FileMode.CreateNew when the file specified by <paramref name="path" /> already exists, occurred. -or-The system is running Windows 98 or Windows 98 Second Edition and <paramref name="share" /> is set to FileShare.Delete.-or-The stream has been closed.</exception>
		/// <exception cref="System::Security::SecurityException">The caller does not have the required permission. </exception>
		/// <exception cref="System::IO::DirectoryNotFoundException">The specified path is invalid, such as being on an unmapped drive. </exception>
		/// <exception cref="System::UnauthorizedAccessException">The <paramref name="access" /> requested is not permitted by the operating system for the specified <paramref name="path" />, such as when <paramref name="access" /> is Write or ReadWrite and the file or directory is set for read-only access. </exception>
		/// <exception cref="System::IO::PathTooLongException">The specified path, file name, or both exceed the system-defined maximum length. For example, on Windows-based platforms, paths must be less than 248 characters, and file names must be less than 260 characters. </exception>
		StdFileStream(System::String^ path, System::IO::FileMode mode, System::IO::FileAccess access, System::IO::FileShare share, int bufferSize);

		/// <summary>Initializes a new instance of the <see cref="StdFileStream" /> class with the specified path, creation mode, read/write and sharing permission, the access other FileStreams can have to the same file, the buffer size, and additional file options.</summary>
		/// <param name="path">A relative or absolute path for the file that the current FileStream object will encapsulate. </param>
		/// <param name="mode">A <see cref="System::IO::FileMode" /> constant that determines how to open or create the file. </param>
		/// <param name="access">A <see cref="System::IO::FileAccess" /> constant that determines how the file can be accessed by the FileStream object. This gets the <see cref="StdFileStream::CanRead" /> and <see cref="StdFileStream::CanWrite" /> properties of the FileStream object. <see cref="StdFileStream::CanSeek" /> is true if <paramref name="path" /> specifies a disk file. </param>
		/// <param name="share">A <see cref="System::IO::FileShare" /> constant that determines how the file will be shared by processes. </param>
		/// <param name="bufferSize">A positive value greater than 0 indicating the buffer size. For <paramref name="bufferSize" /> values between one and eight, the actual buffer size is set to eight bytes. </param>
		/// <param name="options">A <see cref="System::IO::FileOptions" /> value that specifies additional file options.</param>
		/// <exception cref="System::ArgumentNullException">
		///   <paramref name="path" /> is null. </exception>
		/// <exception cref="System::ArgumentException">
		///   <paramref name="path" /> is an empty string (""), contains only white space, or contains one or more invalid characters. -or-<paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in an NTFS environment.</exception>
		/// <exception cref="System::NotSupportedException">
		///   <paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in a non-NTFS environment.</exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		///   <paramref name="bufferSize" /> is negative or zero.-or- <paramref name="mode" />, <paramref name="access" />, or <paramref name="share" /> contain an invalid value. </exception>
		/// <exception cref="System::IO::FileNotFoundException">The file cannot be found, such as when <paramref name="mode" /> is FileMode.Truncate or FileMode.Open, and the file specified by <paramref name="path" /> does not exist. The file must already exist in these modes. </exception>
		/// <exception cref="System::IO::IOException">An I/O error, such as specifying FileMode.CreateNew when the file specified by <paramref name="path" /> already exists, occurred.-or-The stream has been closed.</exception>
		/// <exception cref="System::Security::SecurityException">The caller does not have the required permission. </exception>
		/// <exception cref="System::IO::DirectoryNotFoundException">The specified path is invalid, such as being on an unmapped drive. </exception>
		/// <exception cref="System::UnauthorizedAccessException">The <paramref name="access" /> requested is not permitted by the operating system for the specified <paramref name="path" />, such as when <paramref name="access" /> is Write or ReadWrite and the file or directory is set for read-only access. -or- <b>Encrypted</b> is specified for <paramref name="options" />, but file encryption is not supported on the current platform.</exception>
		/// <exception cref="System::IO::PathTooLongException">The specified path, file name, or both exceed the system-defined maximum length. For example, on Windows-based platforms, paths must be less than 248 characters, and file names must be less than 260 characters. </exception>
		StdFileStream(System::String^ path, System::IO::FileMode mode, System::IO::FileAccess access, System::IO::FileShare share, int bufferSize, System::IO::FileOptions options);

		/// <summary>Initializes a new instance of the <see cref="StdFileStream" /> class with the specified path, creation mode, read/write and sharing permission, buffer size, and synchronous or asynchronous state.</summary>
		/// <param name="path">A relative or absolute path for the file that the current FileStream object will encapsulate. </param>
		/// <param name="mode">A <see cref="System::IO::FileMode" /> constant that determines how to open or create the file. </param>
		/// <param name="access">A <see cref="System::IO::FileAccess" /> constant that determines how the file can be accessed by the FileStream object. This gets the <see cref="StdFileStream::CanRead" /> and <see cref="StdFileStream::CanWrite" /> properties of the FileStream object. <see cref="StdFileStream::CanSeek" /> is true if <paramref name="path" /> specifies a disk file. </param>
		/// <param name="share">A <see cref="System::IO::FileShare" /> constant that determines how the file will be shared by processes. </param>
		/// <param name="bufferSize">A positive value greater than 0 indicating the buffer size. For <paramref name="bufferSize" /> values between one and eight, the actual buffer size is set to eight bytes. </param>
		/// <param name="useAsync">Specifies whether to use asynchronous I/O or synchronous I/O. However, note that the underlying operating system might not support asynchronous I/O, so when specifying true, the handle might be opened synchronously depending on the platform. When opened asynchronously, the <see cref="M:System.IO.FileStream.BeginRead(System.Byte[],System.Int32,System.Int32,System.AsyncCallback,System.Object)" /> and <see cref="M:System.IO.FileStream.BeginWrite(System.Byte[],System.Int32,System.Int32,System.AsyncCallback,System.Object)" /> methods perform better on large reads or writes, but they might be much slower for small reads or writes. If the application is designed to take advantage of asynchronous I/O, set the <paramref name="useAsync" /> parameter to true. Using asynchronous I/O correctly can speed up applications by as much as a factor of 10, but using it without redesigning the application for asynchronous I/O can decrease performance by as much as a factor of 10. </param>
		/// <exception cref="System::ArgumentNullException">
		///   <paramref name="path" /> is null. </exception>
		/// <exception cref="System::ArgumentException">
		///   <paramref name="path" /> is an empty string (""), contains only white space, or contains one or more invalid characters. -or-<paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in an NTFS environment.</exception>
		/// <exception cref="System::NotSupportedException">
		///   <paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in a non-NTFS environment.</exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		///   <paramref name="bufferSize" /> is negative or zero.-or- <paramref name="mode" />, <paramref name="access" />, or <paramref name="share" /> contain an invalid value. </exception>
		/// <exception cref="System::IO::FileNotFoundException">The file cannot be found, such as when <paramref name="mode" /> is FileMode.Truncate or FileMode.Open, and the file specified by <paramref name="path" /> does not exist. The file must already exist in these modes. </exception>
		/// <exception cref="System::IO::IOException">An I/O error, such as specifying FileMode.CreateNew when the file specified by <paramref name="path" /> already exists, occurred.-or- The system is running Windows 98 or Windows 98 Second Edition and <paramref name="share" /> is set to FileShare.Delete.-or-The stream has been closed.</exception>
		/// <exception cref="System::Security::SecurityException">The caller does not have the required permission. </exception>
		/// <exception cref="System::IO::DirectoryNotFoundException">The specified path is invalid, such as being on an unmapped drive. </exception>
		/// <exception cref="System::UnauthorizedAccessException">The <paramref name="access" /> requested is not permitted by the operating system for the specified <paramref name="path" />, such as when <paramref name="access" /> is Write or ReadWrite and the file or directory is set for read-only access. </exception>
		/// <exception cref="System::IO::PathTooLongException">The specified path, file name, or both exceed the system-defined maximum length. For example, on Windows-based platforms, paths must be less than 248 characters, and file names must be less than 260 characters. </exception>
		StdFileStream(System::String^ path, System::IO::FileMode mode, System::IO::FileAccess access, System::IO::FileShare share, int bufferSize, bool useAsync);

		/// <summary>Initializes a new instance of the <see cref="StdFileStream" /> class with the specified path, creation mode, access rights and sharing permission, the buffer size, additional file options, access control and audit security.</summary>
		/// <param name="path">A relative or absolute path for the file that the current <see cref="StdFileStream" /> object will encapsulate.</param>
		/// <param name="mode">A <see cref="System::IO::FileMode" /> constant that determines how to open or create the file.</param>
		/// <param name="rights">A <see cref="System::Security::AccessControl::FileSystemRights" /> constant that determines the access rights to use when creating access and audit rules for the file.</param>
		/// <param name="share">A <see cref="System::IO::FileShare" /> constant that determines how the file will be shared by processes.</param>
		/// <param name="bufferSize">A positive value greater than 0 indicating the buffer size. For <paramref name="bufferSize" /> values between one and eight, the actual buffer size is set to eight bytes.</param>
		/// <param name="options">A <see cref="System::IO::FileOptions" /> constant that specifies additional file options.</param>
		/// <param name="fileSecurity">A <see cref="System::Security::AccessControl::FileSecurity" /> constant that determines the access control and audit security for the file.</param>
		/// <exception cref="System::ArgumentNullException">
		///   <paramref name="path" /> is null. </exception>
		/// <exception cref="System::ArgumentException">
		///   <paramref name="path" /> is an empty string (""), contains only white space, or contains one or more invalid characters. -or-<paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in an NTFS environment.</exception>
		/// <exception cref="System::NotSupportedException">
		///   <paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in a non-NTFS environment.</exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		///   <paramref name="bufferSize" /> is negative or zero.-or- <paramref name="mode" />, <paramref name="access" />, or <paramref name="share" /> contain an invalid value. </exception>
		/// <exception cref="System::IO::FileNotFoundException">The file cannot be found, such as when <paramref name="mode" /> is FileMode.Truncate or FileMode.Open, and the file specified by <paramref name="path" /> does not exist. The file must already exist in these modes. </exception>
		/// <exception cref="System::IO::IOException">An I/O error, such as specifying FileMode.CreateNew when the file specified by <paramref name="path" /> already exists, occurred. -or-The stream has been closed.</exception>
		/// <exception cref="System::Security::SecurityException">The caller does not have the required permission. </exception>
		/// <exception cref="System::IO::DirectoryNotFoundException">The specified path is invalid, such as being on an unmapped drive. </exception>
		/// <exception cref="System::UnauthorizedAccessException">The <paramref name="access" /> requested is not permitted by the operating system for the specified <paramref name="path" />, such as when <paramref name="access" /> is Write or ReadWrite and the file or directory is set for read-only access. -or-<see cref="F:System.IO.FileOptions.Encrypted" /> is specified for <paramref name="options" />, but file encryption is not supported on the current platform.</exception>
		/// <exception cref="System::IO::PathTooLongException">The specified <paramref name="path" />, file name, or both exceed the system-defined maximum length. For example, on Windows-based platforms, paths must be less than 248 characters, and file names must be less than 260 characters. </exception>
		/// <exception cref="System::PlatformNotSupportedException">The current operating system is not Windows NT or later.</exception>
		StdFileStream(System::String^ path, System::IO::FileMode mode, System::Security::AccessControl::FileSystemRights rights, System::IO::FileShare share, int bufferSize, System::IO::FileOptions options, System::Security::AccessControl::FileSecurity^ fileSecurity);

		/// <summary>Initializes a new instance of the <see cref="StdFileStream" /> class with the specified path, creation mode, access rights and sharing permission, the buffer size, and additional file options.</summary>
		/// <param name="path">A relative or absolute path for the file that the current <see cref="StdFileStream" /> object will encapsulate.</param>
		/// <param name="mode">A <see cref="System::IO::FileMode" /> constant that determines how to open or create the file.</param>
		/// <param name="rights">A <see cref="System::Security::AccessControl::FileSystemRights" /> constant that determines the access rights to use when creating access and audit rules for the file.</param>
		/// <param name="share">A <see cref="System::IO::FileShare" /> constant that determines how the file will be shared by processes.</param>
		/// <param name="bufferSize">A positive value greater than 0 indicating the buffer size. For <paramref name="bufferSize" /> values between one and eight, the actual buffer size is set to eight bytes.</param>
		/// <param name="options">A <see cref="System::IO::FileOptions" /> constant that specifies additional file options.</param>
		/// <exception cref="System::ArgumentNullException">
		///   <paramref name="path" /> is null. </exception>
		/// <exception cref="System::ArgumentException">
		///   <paramref name="path" /> is an empty string (""), contains only white space, or contains one or more invalid characters. -or-<paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in an NTFS environment.</exception>
		/// <exception cref="System::NotSupportedException">
		///   <paramref name="path" /> refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in a non-NTFS environment.</exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		///   <paramref name="bufferSize" /> is negative or zero.-or- <paramref name="mode" />, <paramref name="access" />, or <paramref name="share" /> contain an invalid value. </exception>
		/// <exception cref="System::IO::FileNotFoundException">The file cannot be found, such as when <paramref name="mode" /> is FileMode.Truncate or FileMode.Open, and the file specified by <paramref name="path" /> does not exist. The file must already exist in these modes. </exception>
		/// <exception cref="System::PlatformNotSupportedException">The current operating system is not Windows NT or later.</exception>
		/// <exception cref="System::IO::IOException">An I/O error, such as specifying FileMode.CreateNew when the file specified by <paramref name="path" /> already exists, occurred. -or-The stream has been closed.</exception>
		/// <exception cref="System::Security::SecurityException">The caller does not have the required permission. </exception>
		/// <exception cref="System::IO::DirectoryNotFoundException">The specified path is invalid, such as being on an unmapped drive. </exception>
		/// <exception cref="System::UnauthorizedAccessException">The <paramref name="access" /> requested is not permitted by the operating system for the specified <paramref name="path" />, such as when <paramref name="access" /> is Write or ReadWrite and the file or directory is set for read-only access. -or-<see cref="F:System.IO.FileOptions.Encrypted" /> is specified for <paramref name="options" />, but file encryption is not supported on the current platform.</exception>
		/// <exception cref="System::IO::PathTooLongException">The specified <paramref name="path" />, file name, or both exceed the system-defined maximum length. For example, on Windows-based platforms, paths must be less than 248 characters, and file names must be less than 260 characters. </exception>
		StdFileStream(System::String^ path, System::IO::FileMode mode, System::Security::AccessControl::FileSystemRights rights, System::IO::FileShare share, int bufferSize, System::IO::FileOptions options);

		/// <summary>Initializes a new instance of the <see cref="StdFileStream" /> class for the specified file handle, with the specified read/write permission. </summary>
		/// <param name="handle">A file handle for the file that the current FileStream object will encapsulate. </param>
		/// <param name="access">A <see cref="System::IO::FileAccess" /> constant that sets the <see cref="StdFileStream::CanRead" /> and <see cref="StdFileStream::CanWrite" /> properties of the FileStream object. </param>
		/// <exception cref="System::ArgumentException">
		///   <paramref name="access" /> is not a field of <see cref="System::IO::FileAccess" />. </exception>
		/// <exception cref="System::Security::SecurityException">The caller does not have the required permission. </exception>
		/// <exception cref="System::IO::IOException">An I/O error, such as a disk error, occurred.-or-The stream has been closed. </exception>
		/// <exception cref="System::UnauthorizedAccessException">The <paramref name="access" /> requested is not permitted by the operating system for the specified file handle, such as when <paramref name="access" /> is Write or ReadWrite and the file handle is set for read-only access. </exception>
		StdFileStream(Microsoft::Win32::SafeHandles::SafeFileHandle^ handle, System::IO::FileAccess access);

		/// <summary>Initializes a new instance of the <see cref="StdFileStream" /> class for the specified file handle, with the specified read/write permission, and buffer size.</summary>
		/// <param name="handle">A file handle for the file that the current FileStream object will encapsulate. </param>
		/// <param name="access">A <see cref="System::IO::FileAccess" /> constant that gets the <see cref="StdFileStream::CanRead" /> and <see cref="StdFileStream::CanWrite" /> properties of the FileStream object. </param>
		/// <param name="bufferSize">A positive value greater than 0 indicating the buffer size. For <paramref name="bufferSize" /> values between one and eight, the actual buffer size is set to eight bytes.</param>
		/// <exception cref="System::ArgumentException">The <paramref name="handle" /> parameter is an invalid handle.-or-The <paramref name="handle" /> parameter is a synchronous handle and it was used asynchronously. </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">The <paramref name="bufferSize" /> parameter is negative. </exception>
		/// <exception cref="System::IO::IOException">An I/O error, such as a disk error, occurred.-or-The stream has been closed.  </exception>
		/// <exception cref="System::Security::SecurityException">The caller does not have the required permission. </exception>
		/// <exception cref="System::UnauthorizedAccessException">The <paramref name="access" /> requested is not permitted by the operating system for the specified file handle, such as when <paramref name="access" /> is Write or ReadWrite and the file handle is set for read-only access. </exception>
		StdFileStream(Microsoft::Win32::SafeHandles::SafeFileHandle^ handle, System::IO::FileAccess access, int bufferSize);

		/// <summary>Initializes a new instance of the <see cref="StdFileStream" /> class for the specified file handle, with the specified read/write permission, buffer size, and synchronous or asynchronous state.</summary>
		/// <param name="handle">A file handle for the file that this FileStream object will encapsulate. </param>
		/// <param name="access">A <see cref="System::IO::FileAccess" /> constant that gets the <see cref="StdFileStream::CanRead" /> and <see cref="StdFileStream::CanWrite" /> properties of the FileStream object. </param>
		/// <param name="bufferSize">A positive value greater than 0 indicating the buffer size. For <paramref name="bufferSize" /> values between one and eight, the actual buffer size is set to eight bytes.</param>
		/// <param name="isAsync">true if the handle was opened asynchronously (that is, in overlapped I/O mode); otherwise, false. </param>
		/// <exception cref="System::ArgumentException">The <paramref name="handle" /> parameter is an invalid handle.-or-The <paramref name="handle" /> parameter is a synchronous handle and it was used asynchronously. </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">The <paramref name="bufferSize" /> parameter is negative. </exception>
		/// <exception cref="System::IO::IOException">An I/O error, such as a disk error, occurred.-or-The stream has been closed.  </exception>
		/// <exception cref="System::Security::SecurityException">The caller does not have the required permission. </exception>
		/// <exception cref="System::UnauthorizedAccessException">The <paramref name="access" /> requested is not permitted by the operating system for the specified file handle, such as when <paramref name="access" /> is Write or ReadWrite and the file handle is set for read-only access. </exception>
		StdFileStream(Microsoft::Win32::SafeHandles::SafeFileHandle^ handle, System::IO::FileAccess access, int bufferSize, bool isAsync);

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
