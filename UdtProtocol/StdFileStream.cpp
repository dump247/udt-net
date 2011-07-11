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

#include "StdAfx.h"
#include "StdFileStream.h"

#include <iostream>
#include <io.h>
#include <fcntl.h>

using namespace Udt;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::IO;
using namespace System::Security::AccessControl;
using namespace Microsoft::Win32::SafeHandles;

StdFileStream::StdFileStream(String^ path, FileMode mode)
{
	Init(gcnew FileStream(path, mode));
}

StdFileStream::StdFileStream(String^ path, FileMode mode, FileAccess access)
{
	Init(gcnew FileStream(path, mode, access));
}

StdFileStream::StdFileStream(String^ path, FileMode mode, FileAccess access, FileShare share)
{
	Init(gcnew FileStream(path, mode, access, share));
}

StdFileStream::StdFileStream(String^ path, FileMode mode, FileAccess access, FileShare share, int bufferSize)
{
	Init(gcnew FileStream(path, mode, access, share, bufferSize));
}

StdFileStream::StdFileStream(String^ path, FileMode mode, FileAccess access, FileShare share, int bufferSize, FileOptions options)
{
	Init(gcnew FileStream(path, mode, access, share, bufferSize, options));
}

StdFileStream::StdFileStream(String^ path, FileMode mode, FileAccess access, FileShare share, int bufferSize, bool useAsync)
{
	Init(gcnew FileStream(path, mode, access, share, bufferSize, useAsync));
}

StdFileStream::StdFileStream(String^ path, FileMode mode, FileSystemRights rights, FileShare share, int bufferSize, FileOptions options, FileSecurity^ fileSecurity)
{
	Init(gcnew FileStream(path, mode, rights, share, bufferSize, options, fileSecurity));
}

StdFileStream::StdFileStream(String^ path, FileMode mode, FileSystemRights rights, FileShare share, int bufferSize, FileOptions options)
{
	Init(gcnew FileStream(path, mode, rights, share, bufferSize, options));
}

StdFileStream::StdFileStream(SafeFileHandle^ handle, FileAccess access)
{
	Init(gcnew FileStream(handle, access));
}

StdFileStream::StdFileStream(SafeFileHandle^ handle, FileAccess access, int bufferSize)
{
	Init(gcnew FileStream(handle, access, bufferSize));
}

StdFileStream::StdFileStream(SafeFileHandle^ handle, FileAccess access, int bufferSize, bool isAsync)
{
	Init(gcnew FileStream(handle, access, bufferSize, isAsync));
}

void StdFileStream::Init(System::IO::FileStream^ fileStream)
{
	_fileStream = fileStream;
	_stdStream = NULL;
}

StdFileStream::~StdFileStream(void)
{
	_fileStream->Close();

	delete _stdStream;
	_stdStream = NULL;
}

std::fstream& StdFileStream::LoadStdStream(void)
{
	if (_stdStream == NULL)
	{
		// SafeFileHandle will flush the stream
		SafeFileHandle^ fileHandle = _fileStream->SafeFileHandle;

		// Get file descriptor
		int fileDescFlags = _O_RDONLY;

		if (_fileStream->CanWrite)
		{
			if (_fileStream->CanRead)
				fileDescFlags = _O_RDWR;
			else
				fileDescFlags = _O_WRONLY;
		}

		int fileDesc = _open_osfhandle((intptr_t)fileHandle->DangerousGetHandle(), fileDescFlags);

		if (fileDesc == -1) throw gcnew IOException("Error opening C run-time file descriptor.");

		// Get file stream
		FILE* filePtr;

		switch (fileDescFlags)
		{
		case _O_RDONLY:
			filePtr = _fdopen(fileDesc, "r");
			break;
			
		case _O_WRONLY:
			filePtr = _fdopen(fileDesc, "w");
			break;
			
		case _O_RDWR:
			filePtr = _fdopen(fileDesc, "r+");
			break;
		}

		// Get std::fstream
		_stdStream = new std::fstream(filePtr);
	}
	else
	{
		_fileStream->Flush();
	}

	_stdStream->seekg(_fileStream->Position);
	return *_stdStream;
}

void StdFileStream::Flush(void)
{
	_fileStream->Flush();
}

bool StdFileStream::CanRead::get(void)
{
	return _fileStream->CanRead;
}

bool StdFileStream::CanSeek::get(void)
{
	return _fileStream->CanSeek;
}

bool StdFileStream::CanWrite::get(void)
{
	return _fileStream->CanWrite;
}

__int64 StdFileStream::Length::get(void)
{
	return _fileStream->Length;
}

__int64 StdFileStream::Position::get(void)
{
	return _fileStream->Position;
}

void StdFileStream::Position::set(__int64 value)
{
	_fileStream->Position = value;
}

void StdFileStream::SetLength(__int64 value)
{
	_fileStream->SetLength(value);
}

__int64 StdFileStream::Seek(__int64 offset, SeekOrigin origin)
{
	return _fileStream->Seek(offset, origin);
}

int StdFileStream::Read(cli::array<unsigned char>^ buffer, int offset, int count)
{
	return _fileStream->Read(buffer, offset, count);
}

void StdFileStream::Write(cli::array<unsigned char>^ buffer, int offset, int count)
{
	_fileStream->Write(buffer, offset, count);
}
