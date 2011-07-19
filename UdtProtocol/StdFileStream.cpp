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
#include <vcclr.h>

using namespace Udt;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::IO;
using namespace System::Security::AccessControl;
using namespace Microsoft::Win32::SafeHandles;

StdFileStream::StdFileStream(String^ path, FileMode mode)
{
	Init(path, mode, mode == FileMode::Append ? FileAccess::Write : FileAccess::ReadWrite, FileShare::None);
}

StdFileStream::StdFileStream(String^ path, FileMode mode, FileAccess access)
{
	Init(path, mode, access, FileShare::None);
}

StdFileStream::StdFileStream(String^ path, FileMode mode, FileAccess access, FileShare share)
{
	Init(path, mode, access, share);
}

void StdFileStream::AssertNotDisposed()
{
	if (_stdStream == NULL) throw gcnew ObjectDisposedException(this->ToString());
}

void StdFileStream::Init(String^ path, FileMode mode, FileAccess access, FileShare share)
{
	if (path == nullptr) throw gcnew ArgumentNullException("path");
	if (path->Length == 0) throw gcnew ArgumentException("Value can not be empty.", "path");
	
	_canRead = false;
	_canWrite = false;
	_canSeek = false;

	int shareFlag;
	pin_ptr<const wchar_t> pathPin = PtrToStringChars(path);
	FILE* streamPtr = NULL;

	switch (share)
	{
	case FileShare::Read:
		shareFlag = _SH_DENYWR;
		break;

	case FileShare::Write:
		shareFlag = _SH_DENYRD;
		break;

	case FileShare::ReadWrite:
		shareFlag = _SH_DENYNO;
		break;

	case FileShare::None:
		shareFlag = _SH_DENYRW;
		break;

	default:
		throw gcnew ArgumentOutOfRangeException("share", share, "Share mode is not supported.");
	}

	switch (mode)
	{
	case FileMode::CreateNew:
		streamPtr = InitCreateNew(pathPin, access, shareFlag);
		break;
		
	case FileMode::Create:
		streamPtr = InitCreate(pathPin, access, shareFlag);
		break;

	case FileMode::Open:
		streamPtr = InitOpen(pathPin, access, shareFlag);
		break;
		
	case FileMode::OpenOrCreate:
		InitOpenOrCreate(pathPin, access, shareFlag);
		break;
		
	case FileMode::Truncate:
		streamPtr = InitTruncate(pathPin, access, shareFlag);
		break;
		
	case FileMode::Append:
		streamPtr = InitAppend(pathPin, access, shareFlag);
		break;

	default:
		throw gcnew ArgumentOutOfRangeException("mode", mode, "File mode is not supported.");
	}

	_stdStream = new std::fstream(streamPtr);
	_stdStream->exceptions(std::ios::failbit);
}

StdFileStream::~StdFileStream(void)
{
	_canRead = false;
	_canWrite = false;
	_canSeek = false;

	if (_stdStream != NULL)
	{
		_stdStream->close();
		delete _stdStream;
		_stdStream = NULL;
	}
}

void StdFileStream::CheckLastError(const wchar_t* path)
{
	if (errno == ENOENT)
	{
		throw gcnew FileNotFoundException("File not found.", gcnew String(path));
	}
	else if (errno = EINVAL)
	{
		throw gcnew ArgumentException(String::Format("Invalid path value.{0}Value: {1}", Environment::NewLine, gcnew String(path)), "path");
	}
	else
	{
		char buffer[1024];
		strerror_s(buffer, 1024, errno);
		throw gcnew IOException(gcnew String(buffer));
	}
}

void StdFileStream::CheckFileNotExists(const wchar_t* path, int shareFlag)
{
	struct _stat statInfo;

	// Throw an error if anything other than file not found occurs

	if (_wstat(path, &statInfo) == 0)
	{
		throw gcnew IOException("File already exists");
	}
	else if (errno != ENOENT)
	{
		char buffer[1024];
		strerror_s(buffer, 1024, errno);
		throw gcnew IOException(gcnew String(buffer));
	}
}

FILE* StdFileStream::InitCreateNew(const wchar_t* path, FileAccess access, int shareFlag)
{
	FILE* streamPtr;

	// There is a race condition here in that the first operation is to
	// check if the file exists before attempting to open for reading.
	// It is possible for another process to create the file between these
	// two operations. Risk is small; not sure how to handle that scenario, though.

	switch (access)
	{
	case FileAccess::ReadWrite:
		CheckFileNotExists(path, shareFlag);

		// w+: write+read (truncate file if exists)
		// b: binary
		// N: not inherited
		streamPtr = _wfsopen(path, L"wbN+", shareFlag);
		if (streamPtr == NULL) CheckLastError(path);

		_canRead = true;
		_canWrite = true;
		_canSeek = true;
		break;

	case FileAccess::Write:
		CheckFileNotExists(path, shareFlag);

		// w: write only (truncate file if exists)
		// b: binary
		// N: not inherited
		streamPtr = _wfsopen(path, L"wbN", shareFlag);
		if (streamPtr == NULL) CheckLastError(path);

		_canRead = false;
		_canWrite = true;
		_canSeek = true;
		break;

	default:
		throw gcnew ArgumentOutOfRangeException("access", access, "Invalid value in combination with FileMode.Create");
	}

	return streamPtr;
}
		
FILE* StdFileStream::InitCreate(const wchar_t* path, FileAccess access, int shareFlag)
{
	FILE* streamPtr;

	switch (access)
	{
	case FileAccess::ReadWrite:
		// w+: write+read (truncate file if exists)
		// b: binary
		// N: not inherited
		streamPtr = _wfsopen(path, L"wbN+", shareFlag);
		if (streamPtr == NULL) CheckLastError(path);

		_canRead = true;
		_canWrite = true;
		_canSeek = true;
		break;

	case FileAccess::Write:
		// w: write only (truncate file if exists)
		// b: binary
		// N: not inherited
		streamPtr = _wfsopen(path, L"wbN", shareFlag);
		if (streamPtr == NULL) CheckLastError(path);

		_canRead = false;
		_canWrite = true;
		_canSeek = true;
		break;

	default:
		throw gcnew ArgumentOutOfRangeException("access", access, "Invalid value in combination with FileMode.Create");
	}

	return streamPtr;
}

FILE* StdFileStream::InitOpen(const wchar_t* path, FileAccess access, int shareFlag)
{
	FILE* streamPtr;

	switch (access)
	{
	case FileAccess::ReadWrite:
	case FileAccess::Write:
		// r+: read+write (error file if does not exist)
		// b: binary
		// N: not inherited
		streamPtr = _wfsopen(path, L"rbN+", shareFlag);
		if (streamPtr == NULL) CheckLastError(path);

		_canRead = (access == FileAccess::ReadWrite);
		_canWrite = true;
		_canSeek = true;
		break;

	case FileAccess::Read:
		// r: read only (error file if does not exist)
		// b: binary
		// N: not inherited
		streamPtr = _wfsopen(path, L"rbN", shareFlag);
		if (streamPtr == NULL) CheckLastError(path);

		_canRead = true;
		_canWrite = false;
		_canSeek = true;
		break;

	default:
		throw gcnew ArgumentOutOfRangeException("access", access, "Invalid value in combination with FileMode.Open");
	}

	return streamPtr;
}
		
FILE* StdFileStream::InitOpenOrCreate(const wchar_t* path, FileAccess access, int shareFlag)
{
	FILE* streamPtr;

	switch (access)
	{
	case FileAccess::ReadWrite:
	case FileAccess::Write:
		// r+: read+write (error file if does not exist)
		// b: binary
		// N: not inherited
		streamPtr = _wfsopen(path, L"rbN+", shareFlag);

		if (streamPtr == NULL)
		{
			if (errno == ENOENT) streamPtr = _wfsopen(path, L"wbN+", shareFlag);
			if (streamPtr == NULL) CheckLastError(path);
		}

		_canRead = (access == FileAccess::ReadWrite);
		_canWrite = true;
		_canSeek = true;
		break;

	case FileAccess::Read:
		// r: read only (error file if does not exist)
		// b: binary
		// N: not inherited
		streamPtr = _wfsopen(path, L"rbN", shareFlag);

		if (streamPtr == NULL)
		{
			if (errno == ENOENT) streamPtr = _wfsopen(path, L"wbN", shareFlag);
			if (streamPtr == NULL) CheckLastError(path);
		}

		_canRead = true;
		_canWrite = false;
		_canSeek = true;
		break;

	default:
		throw gcnew ArgumentOutOfRangeException("access", access, "Invalid value in combination with FileMode.OpenOrCreate");
	}

	return streamPtr;
}
		
FILE* StdFileStream::InitTruncate(const wchar_t* path, FileAccess access, int shareFlag)
{
	FILE* streamPtr;

	switch (access)
	{
	case FileAccess::ReadWrite:
	case FileAccess::Write:
		// r+: read+write (error file if does not exist)
		// b: binary
		// N: not inherited
		streamPtr = _wfsopen(path, L"rbN+", shareFlag);

		if (streamPtr == NULL)
			CheckLastError(path);

		if (_chsize(_fileno(streamPtr), 0) != 0)
		{
			fclose(streamPtr);
			CheckLastError(path);
		}

		_canRead = (access == FileAccess::ReadWrite);
		_canWrite = true;
		_canSeek = true;
		break;

	default:
		throw gcnew ArgumentOutOfRangeException("access", access, "Invalid value in combination with FileMode.Truncate");
	}

	return streamPtr;
}
		
FILE* StdFileStream::InitAppend(const wchar_t* path, FileAccess access, int shareFlag)
{
	FILE* streamPtr;

	switch (access)
	{
	case FileAccess::Write:
		// a: append only (error file if does not exist)
		// b: binary
		// N: not inherited
		streamPtr = _wfsopen(path, L"abN", shareFlag);
		if (streamPtr == NULL) CheckLastError(path);

		_canRead = false;
		_canWrite = true;
		_canSeek = false;
		break;

	default:
		throw gcnew ArgumentOutOfRangeException("access", access, "Invalid value in combination with FileMode.Append");
	}

	return streamPtr;
}

std::fstream& StdFileStream::LoadStdStream(void)
{
	AssertNotDisposed();
	return *_stdStream;
}

void StdFileStream::Flush(void)
{
	AssertNotDisposed();
	if (CanWrite) _stdStream->flush();
}

bool StdFileStream::CanRead::get(void)
{
	return _canRead;
}

bool StdFileStream::CanSeek::get(void)
{
	return _canSeek;
}

bool StdFileStream::CanWrite::get(void)
{
	return _canWrite;
}

__int64 StdFileStream::Length::get(void)
{
	AssertNotDisposed();

	try
	{
		std::streampos currentPos = _stdStream->tellg();
		_stdStream->seekg(0, std::ios_base::end);
		std::streampos length = _stdStream->tellg();
		_stdStream->seekg(currentPos);

		return length;
	}
	catch (const std::exception& ex)
	{
		throw gcnew IOException(gcnew String(ex.what()));
	}
}

__int64 StdFileStream::Position::get(void)
{
	AssertNotDisposed();

	try
	{
		return _stdStream->tellg();
	}
	catch (const std::exception& ex)
	{
		throw gcnew IOException(gcnew String(ex.what()));
	}
}

void StdFileStream::Position::set(__int64 value)
{
	AssertNotDisposed();
	
	// The documentation for FileMode::Append states that attempting to seek
	// results in IOException while attempting to read results in NotSupportedException
	if (!CanSeek) throw gcnew IOException("Stream does not support seeking.");

	try
	{
		_stdStream->seekg(value);
	}
	catch (const std::exception& ex)
	{
		throw gcnew IOException(gcnew String(ex.what()));
	}
}

void StdFileStream::SetLength(__int64 value)
{
	throw gcnew NotImplementedException();
}

__int64 StdFileStream::Seek(__int64 offset, SeekOrigin origin)
{
	AssertNotDisposed();

	// The documentation for FileMode::Append states that attempting to seek
	// results in IOException while attempting to read results in NotSupportedException
	if (!CanSeek) throw gcnew IOException("Stream does not support seeking.");

	try
	{
		switch (origin)
		{
		case SeekOrigin::Begin:
			_stdStream->seekg(offset, std::ios_base::beg);
			break;
		
		case SeekOrigin::End:
			_stdStream->seekg(offset, std::ios_base::end);
			break;

		case SeekOrigin::Current:
			_stdStream->seekg(offset, std::ios_base::cur);
			break;

		default:
			throw gcnew ArgumentOutOfRangeException("origin", origin, "Unsupported value.");
		}
	}
	catch (const std::exception& ex)
	{
		throw gcnew IOException(gcnew String(ex.what()));
	}

	return Position;
}

int StdFileStream::Read(cli::array<unsigned char>^ buffer, int offset, int count)
{
	if (buffer == nullptr) throw gcnew ArgumentNullException("buffer");
	if (offset < 0) throw gcnew ArgumentOutOfRangeException("offset", offset, "Value must be greater than or equal to 0.");
	if (count < 0) throw gcnew ArgumentOutOfRangeException("count", count, "Value must be greater than or equal to 0.");
	if (buffer->Length - offset < count) throw gcnew ArgumentException("Invalid array offset/count.");
	
	AssertNotDisposed();

	if (!CanRead) throw gcnew NotSupportedException("Stream does not support reading.");

	if (_stdStream->eof()) return 0;

	pin_ptr<unsigned char> bufferPin = &buffer[0];
    unsigned char* bufferPtr = bufferPin;

    try
    {
		_stdStream->read((char*)bufferPtr + offset, count);
    }
    catch (const std::exception& ex)
    {
		if (_stdStream->bad()) throw gcnew IOException(gcnew String(ex.what()));
    }

    return (int)_stdStream->gcount();
}

void StdFileStream::Write(cli::array<unsigned char>^ buffer, int offset, int count)
{
	if (buffer == nullptr) throw gcnew ArgumentNullException("buffer");
	if (offset < 0) throw gcnew ArgumentOutOfRangeException("offset", offset, "Value must be greater than or equal to 0.");
	if (count < 0) throw gcnew ArgumentOutOfRangeException("count", count, "Value must be greater than or equal to 0.");
	if (buffer->Length - offset < count) throw gcnew ArgumentException("Invalid array offset/count.");
	
	AssertNotDisposed();

	if (!CanWrite) throw gcnew NotSupportedException("Stream does not support writing.");

	pin_ptr<unsigned char> bufferPin = &buffer[0];
    unsigned char* bufferPtr = bufferPin;

    try
    {
		_stdStream->write((char*)bufferPtr + offset, count);
    }
    catch (const std::exception& ex)
    {
		throw gcnew IOException(gcnew String(ex.what()));
    }
}
