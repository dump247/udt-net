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

#include "NetworkStream.h"
#include "Socket.h"

using namespace Udt;
using namespace System;
using namespace System::IO;

bool Readable(FileAccess access)
{
	return (int)((access & FileAccess::Read)) != 0;
}

bool Writeable(FileAccess access)
{
	return (int)((access & FileAccess::Write)) != 0;
}

NetworkStream::NetworkStream(Udt::Socket^ socket)
{
	_socket = socket;
	_access = FileAccess::ReadWrite;
	_ownsSocket = true;

	Initialize(socket);
}

NetworkStream::NetworkStream(Udt::Socket^ socket, bool ownsSocket)
{
	_socket = socket;
	_access = FileAccess::ReadWrite;
	_ownsSocket = ownsSocket;

	Initialize(socket);
}

NetworkStream::NetworkStream(Udt::Socket^ socket, FileAccess access)
{
	_socket = socket;
	_access = access;
	_ownsSocket = true;

	Initialize(socket);
}

NetworkStream::NetworkStream(Udt::Socket^ socket, FileAccess access, bool ownsSocket)
{
	_socket = socket;
	_access = access;
	_ownsSocket = ownsSocket;

	Initialize(socket);
}

void NetworkStream::Initialize(Udt::Socket^ socket)
{
	if (socket == nullptr)
		throw gcnew ArgumentNullException("socket");

	if (socket->SocketType != System::Net::Sockets::SocketType::Stream)
		throw gcnew ArgumentException("Socket type must be Stream.", "socket");

	if ((Readable(_access) && !socket->BlockingReceive) ||
		(Writeable(_access) && !socket->BlockingSend))
	{
		throw gcnew ArgumentException("Socket must be in blocking state.", "socket");
	}
}

NetworkStream::~NetworkStream()
{
	Udt::Socket^ socket = _socket;
	_socket = nullptr;

	if (_ownsSocket) delete socket;
}

void NetworkStream::AssertNotDisposed(void)
{
	if (_socket == nullptr)
		throw gcnew ObjectDisposedException(this->ToString());
}

Udt::Socket^ NetworkStream::Socket::get(void)
{
	AssertNotDisposed();
	return _socket;
}

bool NetworkStream::CanRead::get(void)
{
	return _socket != nullptr && Readable(_access);
}

bool NetworkStream::CanWrite::get(void)
{
	return _socket != nullptr && Writeable(_access);
}

void NetworkStream::Flush(void)
{
}

int NetworkStream::Read(cli::array<System::Byte>^ buffer, int offset, int count)
{
	AssertNotDisposed();

	if (!CanRead)
		throw gcnew NotSupportedException("Stream does not support reading.");

	return _socket->Receive(buffer, offset, count);
}

void NetworkStream::Write(cli::array<System::Byte>^ buffer, int offset, int count)
{
	AssertNotDisposed();

	if (!CanWrite)
		throw gcnew NotSupportedException("Stream does not support writing.");

	_socket->Send(buffer, offset, count);
}

__int64 NetworkStream::Seek(__int64 offset, System::IO::SeekOrigin origin)
{
	throw gcnew NotSupportedException("Stream does not support seeking.");
}

void NetworkStream::SetLength(__int64 value)
{
	throw gcnew NotSupportedException("Stream does not support seeking.");
}

__int64 NetworkStream::Length::get(void)
{
	throw gcnew NotSupportedException("Stream does not support seeking.");
}

__int64 NetworkStream::Position::get(void)
{
	throw gcnew NotSupportedException("Stream does not support seeking.");
}

void NetworkStream::Position::set(__int64 value)
{
	throw gcnew NotSupportedException("Stream does not support seeking.");
}
