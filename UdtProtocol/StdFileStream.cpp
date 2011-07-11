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

using namespace Udt;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::IO;

StdFileStream::StdFileStream(System::String^ path, FileMode mode)
	: _stream(NULL), _canRead(false), _canWrite(false), _canSeek(false)
{
	Init(path, mode, mode == FileMode::Append ? FileAccess::Write : FileAccess::ReadWrite);
}

StdFileStream::StdFileStream(System::String^ path, FileMode mode, FileAccess access)
	: _stream(NULL), _canRead(false), _canWrite(false), _canSeek(false)
{
	Init(path, mode, access);
}

void StdFileStream::Init(System::String^ path, FileMode mode, FileAccess access)
{
	if (path == nullptr) throw gcnew ArgumentNullException("path");
	if (path->Length == 0) throw gcnew ArgumentException("Value can not be empty.", "path");

	char* pathPtr = (char*)(void*)Marshal::StringToHGlobalAnsi(path);
	
	__try
	{
		Init(pathPtr, mode, access);
	}
	__finally
	{
		Marshal::FreeHGlobal(IntPtr(pathPtr));
	}
}

void StdFileStream::Init(const char* path, FileMode mode, FileAccess access)
{
	std::ios_base::openmode stdMode = std::ios_base::binary;

	if (mode == FileMode::Append)
		stdMode |= std::ios_base::app;
	if (mode == FileMode::Create || mode == FileMode::Truncate)
		stdMode |= std::ios_base::trunc;

	if ((access & FileAccess::Read) != (FileAccess)0)
	{
		stdMode |= std::ios_base::in;
		_canRead = true;
		_canSeek = true;
	}

	if ((access & FileAccess::Write) != (FileAccess)0)
	{
		stdMode |= std::ios_base::out;
		_canWrite = true;
	}

	_stream = new std::fstream();
	_stream->exceptions(std::ios::failbit);

	try
	{
		_stream->open(path, stdMode);
	}
	catch (const std::exception& ex)
	{
		CloseStream();
		throw gcnew IOException(gcnew String(ex.what()));
	}
}

StdFileStream::~StdFileStream(void)
{
	CloseStream();
}

void StdFileStream::CloseStream()
{
	try
	{
		std::fstream* stream = _stream;

		_stream = NULL;
		_canRead = false;
		_canWrite = false;
		_canSeek = false;

		delete stream;
	}
	catch (const std::exception& ex)
	{
		throw gcnew IOException(gcnew String(ex.what()));
	}
}

void StdFileStream::AssertNotDisposed()
{
	if (_stream == NULL) throw gcnew ObjectDisposedException(this->ToString());
}

void StdFileStream::Flush(void)
{
	AssertNotDisposed();

	try
	{
		if (CanWrite) _stream->flush();
	}
	catch (const std::exception& ex)
	{
		throw gcnew IOException(gcnew String(ex.what()));
	}
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
		std::streampos currentPos = _stream->tellg();
		_stream->seekg(0, std::ios::end);
		std::streampos len = _stream->tellg();
		_stream->seekg(currentPos);

		return len;
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
		return _stream->tellg();
	}
	catch (const std::exception& ex)
	{
		throw gcnew IOException(gcnew String(ex.what()));
	}
}

void StdFileStream::Position::set(__int64 value)
{
	AssertNotDisposed();

	try
	{
		_stream->seekg(value);
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

	try
	{
		switch (origin)
		{
		case SeekOrigin::Begin:
			_stream->seekg(offset, std::ios_base::beg);
			break;
		
		case SeekOrigin::Current:
			_stream->seekg(offset, std::ios_base::cur);
			break;
		
		case SeekOrigin::End:
			_stream->seekg(offset, std::ios_base::end);
			break;

		default:
			throw gcnew ArgumentException(String::Concat("Unknown value: ", origin), "origin");
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

	if (_stream->eof()) return 0;

	pin_ptr<unsigned char> bufferPin = &buffer[0];
	unsigned char* bufferPtr = bufferPin;

	try
	{
		_stream->read((char*)bufferPtr + offset, count);
	}
	catch (const std::exception& ex)
	{
		if (_stream->bad()) throw gcnew IOException(gcnew String(ex.what()));
	}

	return (int)_stream->gcount();
}

void StdFileStream::Write(cli::array<unsigned char>^ buffer, int offset, int count)
{
	if (buffer == nullptr) throw gcnew ArgumentNullException("buffer");
	if (offset < 0) throw gcnew ArgumentOutOfRangeException("offset", offset, "Value must be greater than or equal to 0.");
	if (count < 0) throw gcnew ArgumentOutOfRangeException("count", count, "Value must be greater than or equal to 0.");
	if (buffer->Length - offset < count) throw gcnew ArgumentException("Invalid array offset/count.");

	AssertNotDisposed();

	pin_ptr<unsigned char> bufferPin = &buffer[0];
	unsigned char* bufferPtr = bufferPin;

	try
	{
		_stream->write((char*)bufferPtr + offset, count);
	}
	catch (const std::exception& ex)
	{
		throw gcnew IOException(gcnew String(ex.what()));
	}
}
