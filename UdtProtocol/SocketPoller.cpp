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
#include "SocketPoller.h"

#include "Socket.h"
#include "SocketException.h"

#include <udt.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;
using namespace Udt;

SocketPoller::SocketPoller(void)
{
	_epollId = UDT::epoll_create();

	if (_epollId < 0)
		throw Udt::SocketException::GetLastError("Error creating epoll id.");

	_pollSockets = gcnew Dictionary<UDTSOCKET, Socket^>();
	_writeSockets = _readSockets = (ICollection<Udt::Socket^>^)EmptySocketList;
}

SocketPoller::~SocketPoller(void)
{
	if (_epollId >= 0)
	{
		UDT::epoll_release(_epollId);
		_epollId = -1;
	}
}

void SocketPoller::AssertNotDisposed()
{
	if (_epollId < 0) throw gcnew ObjectDisposedException(this->ToString());
}

void SocketPoller::AddSocket(Udt::Socket^ socket)
{
	if (socket == nullptr) throw gcnew ArgumentNullException("socket");

	AssertNotDisposed();

	if (UDT::epoll_add_usock(_epollId, socket->Handle) < 0)
	{
		throw Udt::SocketException::GetLastError("Error adding UDT socket to epoll.");
	}

	_pollSockets[socket->Handle] = socket;
}

void SocketPoller::RemoveSocket(Udt::Socket^ socket)
{
	if (socket == nullptr) throw gcnew ArgumentNullException("socket");

	AssertNotDisposed();

	if (UDT::epoll_remove_usock(_epollId, socket->Handle) < 0)
		throw Udt::SocketException::GetLastError("Error removing UDT socket from epoll.");

	_pollSockets->Remove(socket->Handle);
}

void SocketPoller::Wait()
{
	Wait(Udt::Socket::InfiniteTimeout);
}

bool SocketPoller::Wait(System::TimeSpan timeout)
{
	AssertNotDisposed();

	if (_pollSockets->Count == 0) throw gcnew InvalidOperationException("No sockets have been added to the poller.");
	
	_writeSockets = _readSockets = (ICollection<Udt::Socket^>^)EmptySocketList;

	std::set<UDTSOCKET> readSockets;
	std::set<UDTSOCKET> writeSockets;

	int result = UDT::epoll_wait(_epollId, &readSockets, &writeSockets, (int64_t)timeout.TotalMilliseconds);

	if (result < 0)
		throw Udt::SocketException::GetLastError("Error waiting for socket epoll.");

	if (result == 0)
		return false;
	
	if (readSockets.size() > 0)
	{
		List<Socket^>^ socketList = GetSockets(readSockets);
	
		if (socketList->Count > 0)
		{
			_readSockets = gcnew ReadOnlyCollection<Socket^>(socketList);
		}
	}

	if (writeSockets.size() > 0)
	{
		List<Socket^>^ socketList = GetSockets(readSockets);
	
		if (socketList->Count > 0)
		{
			_writeSockets = gcnew ReadOnlyCollection<Socket^>(socketList);
		}
	}

	return true;
}

List<Udt::Socket^>^ SocketPoller::GetSockets(std::set<UDTSOCKET>& handles)
{
	List<Udt::Socket^>^ list = gcnew List<Udt::Socket^>((int)handles.size());

	for (std::set<UDTSOCKET>::iterator handleIter = handles.begin(); handleIter != handles.end(); ++handleIter)
	{
		Udt::Socket^ socket;

		if (_pollSockets->TryGetValue(*handleIter, socket))
			list->Add(socket);
	}

	return list;
}

ICollection<Udt::Socket^>^ SocketPoller::ReadSockets::get(void)
{
	return _readSockets;
}

ICollection<Udt::Socket^>^ SocketPoller::WriteSockets::get(void)
{
	return _writeSockets;
}
