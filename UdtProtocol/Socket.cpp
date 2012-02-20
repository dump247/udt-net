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

#include <winsock2.h>
#include <ws2tcpip.h>

#include "Socket.h"
#include "SocketException.h"
#include "CCCWrapperFactory.h"

#include <fstream>
#include <iostream>
#include <vcclr.h>

using namespace Udt;
using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Collections::Generic;
using namespace System::Globalization;

#ifdef _WIN64
#define INTPTR_TO_UDTSOCKET(ptr) (ptr.ToInt64())
#else
#define INTPTR_TO_UDTSOCKET(ptr) (ptr.ToInt32())
#endif

void ToTimeVal(TimeSpan ts, timeval& tv)
{
	__int64 ticks = ts.Ticks;
	tv.tv_sec = (long)(ticks / 10000000);
	tv.tv_usec = (ticks % 10000000) / 10;
}

int UdtSendMessage(UDTSOCKET socket, cli::array<System::Byte>^ buffer, int offset, int size, int ttl = -1, bool inorder = false)
{
	cli::pin_ptr<unsigned char> buffer_pin = &buffer[0];
	unsigned char* buffer_ptr = &buffer_pin[offset];

	int result = UDT::sendmsg(socket, (const char*)buffer_ptr, size, ttl, inorder);

	if (UDT::ERROR == result)
	{
		throw Udt::SocketException::GetLastError("Error sending message.");
	}

	return result;
}

IPEndPoint^ ToEndPoint(sockaddr_storage* addr)
{
	int port;
	System::Net::IPAddress^ address;

	if (addr->ss_family == AF_INET)
	{
		sockaddr_in* addr_in = (sockaddr_in*)addr;
		port = ntohs(addr_in->sin_port);
		address = gcnew System::Net::IPAddress(addr_in->sin_addr.s_addr);
	}
	else
	{
		sockaddr_in6* addr_in6 = (sockaddr_in6*)addr;
		port = ntohs(addr_in6->sin6_port);

		cli::array<unsigned char>^ address_bytes = gcnew cli::array<unsigned char>(16);
		cli::pin_ptr<unsigned char> address_bytes_pin = &address_bytes[0];
		unsigned char* address_bytes_ptr = address_bytes_pin;
		memcpy(address_bytes_ptr, addr_in6->sin6_addr.s6_addr, 16);
		
		address = gcnew System::Net::IPAddress(address_bytes, addr_in6->sin6_scope_id);
	}

	return gcnew System::Net::IPEndPoint(address, port);
}

void ToSockAddr(System::Net::IPAddress^ address, int port, sockaddr_storage& sockaddr, int& size)
{
	memset(&sockaddr, 0, sizeof(sockaddr_storage));

	if (address->AddressFamily == AddressFamily::InterNetworkV6)
	{
		size = sizeof(sockaddr_in6);

		sockaddr_in6* sockaddr6 = (sockaddr_in6*)&sockaddr;
		sockaddr6->sin6_family = AF_INET6;
		sockaddr6->sin6_port = htons(port);

		cli::array<unsigned char>^ address_bytes = address->GetAddressBytes();
		cli::pin_ptr<unsigned char> address_bytes_pin = &address_bytes[0];
		memcpy(sockaddr6->sin6_addr.s6_addr, address_bytes_pin, 16);
	}
	else
	{
		size = sizeof(sockaddr_in);

		sockaddr_in* sockaddr4 = (sockaddr_in*)&sockaddr;
		sockaddr4->sin_family = AF_INET;
		sockaddr4->sin_port = htons(port);
		sockaddr4->sin_addr.s_addr = (int)address->Address;
	}
}

Udt::Socket::Socket(UDTSOCKET socket, System::Net::Sockets::AddressFamily family, System::Net::Sockets::SocketType type, ICongestionControlFactory^ congestionControl)
{
	_socket = socket;
	_addressFamily = family;
	_socketType = type;
	_congestionControl = congestionControl;
}

Udt::Socket::Socket(System::Net::Sockets::AddressFamily family, System::Net::Sockets::SocketType type)
{
	_addressFamily = family;
	_socketType = type;

	int socketFamily;
	int socketType;

	switch (family)
	{
	case System::Net::Sockets::AddressFamily::InterNetwork:
		socketFamily = AF_INET;
		break;

	case System::Net::Sockets::AddressFamily::InterNetworkV6:
		socketFamily = AF_INET6;
		break;

	default:
		throw gcnew ArgumentException(String::Concat("Unsupported address family: ", family.ToString()), "family");
	}

	switch (type)
	{
	case System::Net::Sockets::SocketType::Dgram:
		socketType = SOCK_DGRAM;
		break;

	case System::Net::Sockets::SocketType::Stream:
		socketType = SOCK_STREAM;
		break;

	default:
		throw gcnew ArgumentException(String::Concat("Unsupported socket type: ", type.ToString()), "type");
	}

	_socket = UDT::socket(socketFamily, socketType, 0);

	if (_socket == UDT::INVALID_SOCK)
		throw Udt::SocketException::GetLastError(String::Concat("Error creating ", family.ToString(), "/", type.ToString(), " UDT socket"));
	
	// Windows UDP issue
	// For better performance, modify HKLM\System\CurrentControlSet\Services\Afd\Parameters\FastSendDatagramThreshold
	int mss = 1052;
	if (UDT::ERROR == UDT::setsockopt(_socket, 0, UDT_MSS, &mss, sizeof(int)))
	{
		throw Udt::SocketException::GetLastError("Error setting UDT_MSS socket option");
	}
}

Udt::Socket::~Socket(void)
{
	this->Close();
}

void Udt::Socket::Close(void)
{
	if (_socket != UDT::INVALID_SOCK)
	{
		CongestionControl = nullptr;

		if (UDT::ERROR == UDT::close(_socket))
		{
			_socket = UDT::INVALID_SOCK;
			throw Udt::SocketException::GetLastError("Error closing socket");
		}

		_socket = UDT::INVALID_SOCK;
	}
}

void Udt::Socket::Bind(IPAddress^ address, int port)
{
	if (address == nullptr)
		throw gcnew ArgumentNullException("address");

	if (address->AddressFamily != _addressFamily)
		throw gcnew ArgumentException(String::Concat("Value must be same as socket address family (", _addressFamily, ")."), "address");

	if (port < IPEndPoint::MinPort || port > IPEndPoint::MaxPort)
		throw gcnew ArgumentOutOfRangeException("port", port, String::Concat("Value must be between ", (Object^)IPEndPoint::MinPort, " and ", (Object^)IPEndPoint::MaxPort, "."));

	sockaddr_storage bind_addr;
	int size;

	ToSockAddr(address, port, bind_addr, size);

	if (UDT::ERROR == UDT::bind(_socket, (sockaddr*)&bind_addr, size))
	{
		if (address->AddressFamily == System::Net::Sockets::AddressFamily::InterNetworkV6)
			throw Udt::SocketException::GetLastError(String::Concat("Error binding to [", address, "]:", (Object^)port));
		else
			throw Udt::SocketException::GetLastError(String::Concat("Error binding to ", address, ":", (Object^)port));
	}
}

void Udt::Socket::Bind(IPEndPoint^ endPoint)
{
	if (endPoint == nullptr)
		throw gcnew ArgumentNullException("endPoint");

	if (endPoint->AddressFamily != _addressFamily)
		throw gcnew ArgumentException(String::Concat("Address must be same as socket address family (", _addressFamily, ")."), "endPoint");

	Bind(endPoint->Address, endPoint->Port);
}

void Udt::Socket::Bind(System::Net::Sockets::Socket^ udpSocket)
{
	if (udpSocket == nullptr)
		throw gcnew ArgumentNullException("udpSocket");
	
	if (udpSocket->ProtocolType != ProtocolType::Udp)
		throw gcnew ArgumentException(String::Concat("Socket must be a UDP Socket. Socket is ", udpSocket->ProtocolType), "udpSocket");

	if (UDT::ERROR == UDT::bind(_socket, INTPTR_TO_UDTSOCKET(udpSocket->Handle)))
	{
		throw Udt::SocketException::GetLastError("Error binding to existing UDP socket.");
	}
}

void Udt::Socket::Listen(int backlog)
{
	if (backlog < 1)
		throw gcnew ArgumentOutOfRangeException("backlog", backlog, "Value must be greater than 0.");

	if (UDT::ERROR == UDT::listen(_socket, backlog))
	{
		throw Udt::SocketException::GetLastError("Error entering listening state");
	}
}

Udt::Socket^ Udt::Socket::Accept()
{
	sockaddr_storage client_addr;
	int client_addr_len = sizeof(client_addr);
	UDTSOCKET client = UDT::accept(_socket, (sockaddr*)&client_addr, &client_addr_len);

	if (client == UDT::INVALID_SOCK)
		throw Udt::SocketException::GetLastError("Error accepting new connection.");

	return gcnew Socket(client, _addressFamily, _socketType, _congestionControl);
}

void Udt::Socket::Connect(System::String^ host, int port)
{
	if (host == nullptr)
		throw gcnew ArgumentNullException("host");

	Connect(Dns::GetHostAddresses(host), port);
}

void Udt::Socket::Connect(System::Net::IPAddress^ address, int port)
{
	if (address == nullptr)
		throw gcnew ArgumentNullException("address");

	if (port < IPEndPoint::MinPort || port > IPEndPoint::MaxPort)
		throw gcnew ArgumentOutOfRangeException("port", port, String::Concat("Value must be between ", (Object^)IPEndPoint::MinPort, " and ", (Object^)IPEndPoint::MaxPort, "."));

	sockaddr_storage connect_addr;
	int size;

	ToSockAddr(address, port, connect_addr, size);

	if (UDT::ERROR == UDT::connect(_socket, (sockaddr*)&connect_addr, size))
	{
		if (address->AddressFamily == System::Net::Sockets::AddressFamily::InterNetworkV6)
			throw Udt::SocketException::GetLastError(String::Concat("Error connecting to [", address, "]:", (Object^)port));
		else
			throw Udt::SocketException::GetLastError(String::Concat("Error connecting to ", address, ":", (Object^)port));
	}
}

void Udt::Socket::Connect(cli::array<System::Net::IPAddress^>^ addresses, int port)
{
	if (addresses == nullptr)
		throw gcnew ArgumentNullException("addresses");

	if (addresses->Length == 0)
		throw gcnew ArgumentException("Value can not be empty.", "addresses");

	Connect(addresses[0], port);
}

UDT::UDSET* Udt::Socket::CreateUDSet(String^ paramName, System::Collections::Generic::ICollection<Udt::Socket^>^ fds)
{
	if (fds == nullptr || fds->Count == 0)
		return NULL;

	std::auto_ptr<UDT::UDSET> set(new UDT::UDSET);
	UD_ZERO(set);

	for each (Udt::Socket^ socket in fds)
	{
		if (socket == nullptr)
			throw gcnew ArgumentException("Value can not contain null reference.", paramName);

		UDTSOCKET socketHandle = socket->_socket;
		UD_SET(socketHandle, set);
	}

	return set.release();
}

void Udt::Socket::FillSocketList(const std::vector<UDTSOCKET>* list, System::Collections::Generic::Dictionary<UDTSOCKET, Udt::Socket^>^ sockets, System::Collections::Generic::ICollection<Udt::Socket^>^ fds)
{
	if (list != NULL)
	{
		for (std::vector<UDTSOCKET>::const_iterator it = list->begin(); it != list->end(); ++it)
		{
			fds->Add(sockets[*it]);
		}
	}
}

bool IsEmpty(System::Collections::Generic::ICollection<Udt::Socket^>^ fds)
{
	return fds == nullptr || fds->Count == 0;
}

void Udt::Socket::Filter(UDT::UDSET* set, System::Collections::Generic::ICollection<Udt::Socket^>^ fds)
{
	if (!IsEmpty(fds))
	{
		HashSet<Udt::Socket^>^ sockets = gcnew HashSet<Udt::Socket^>(fds);

		fds->Clear();

		for each(Udt::Socket^ socket in sockets)
		{
			UDTSOCKET socketHandle = socket->_socket;
			if (UD_ISSET(socketHandle, set))
			{
				fds->Add(socket);
			}
		}
	}
}

void Udt::Socket::Select(
			System::Collections::Generic::ICollection<Socket^>^ checkSockets,
			System::Collections::Generic::ICollection<Socket^>^ readSockets,
			System::Collections::Generic::ICollection<Socket^>^ writeSockets,
			System::Collections::Generic::ICollection<Socket^>^ errorSockets,
			System::TimeSpan timeout)
{
	if (checkSockets == nullptr)
		throw gcnew ArgumentNullException("checkSockets");

	if (readSockets == nullptr && writeSockets == nullptr && errorSockets == nullptr)
		throw gcnew ArgumentException("At least one of readSockets, writeSockets, or errorSockets is required");

	if (timeout != Udt::Socket::InfiniteTimeout && timeout < System::TimeSpan::Zero)
		throw gcnew ArgumentOutOfRangeException("timeout", timeout, "Value must be infinite (-1 ticks) or greater than or equal to 0.");

	// Setup native function parameters
	std::vector<UDTSOCKET> fds;
	Dictionary<UDTSOCKET, Udt::Socket^>^ socketMap = gcnew Dictionary<UDTSOCKET, Udt::Socket^>();

	for each (Udt::Socket^ socket in checkSockets)
	{
		if (socket == nullptr)
			throw gcnew ArgumentException("Value can not contain null reference.", "checkSockets");

		UDTSOCKET socketHandle = socket->_socket;

		if (!socketMap->ContainsKey(socketHandle))
		{
			socketMap->Add(socketHandle, socket);
			fds.push_back(socketHandle);
		}
	}

	std::vector<UDTSOCKET> readFds;
	std::vector<UDTSOCKET>* readFdsPtr = readSockets == nullptr ? NULL : &readFds;
	std::vector<UDTSOCKET> writeFds;
	std::vector<UDTSOCKET>* writeFdsPtr = writeSockets == nullptr ? NULL : &writeFds;
	std::vector<UDTSOCKET> exceptFds;
	std::vector<UDTSOCKET>* exceptFdsPtr = errorSockets == nullptr ? NULL : &exceptFds;

	int64_t msTimeOut = Int64::MaxValue;

	if (timeout != Udt::Socket::InfiniteTimeout)
		msTimeOut = (int64_t)timeout.TotalMilliseconds;

	// Call native function
	if (UDT::ERROR == UDT::selectEx(fds, readFdsPtr, writeFdsPtr, exceptFdsPtr, msTimeOut))
	{
		throw Udt::SocketException::GetLastError("Error in socket selectEx.");
	}

	// Add the sockets to the provided socket collections
	FillSocketList(readFdsPtr, socketMap, readSockets);
	FillSocketList(writeFdsPtr, socketMap, writeSockets);
	FillSocketList(exceptFdsPtr, socketMap, errorSockets);
}

void Udt::Socket::Select(
			System::Collections::Generic::ICollection<Socket^>^ checkRead,
			System::Collections::Generic::ICollection<Socket^>^ checkWrite,
			System::Collections::Generic::ICollection<Socket^>^ checkError,
			System::TimeSpan timeout)
{
	if (IsEmpty(checkRead) && IsEmpty(checkWrite) && IsEmpty(checkError))
	{
		throw gcnew ArgumentException("At least one of checkRead, checkWrite, or checkError is required");
	}

	if (timeout != Udt::Socket::InfiniteTimeout && timeout < System::TimeSpan::Zero)
	{
		throw gcnew ArgumentOutOfRangeException("timeout", timeout, "Value must be infinite (-1 ticks) or greater than or equal to 0.");
	}

	timeval tv;

	if (timeout == Udt::Socket::InfiniteTimeout)
	{
		tv.tv_sec = Int32::MaxValue;
		tv.tv_usec = 0;
	}
	else
	{
		__int64 timeoutTicks = timeout.Ticks;
		tv.tv_sec = (long)(timeoutTicks / TimeSpan::TicksPerSecond);
		tv.tv_usec = (timeoutTicks % TimeSpan::TicksPerSecond) / 10;
	}

	std::auto_ptr<UDT::UDSET> readFds(CreateUDSet("checkRead", checkRead));
	std::auto_ptr<UDT::UDSET> writeFds(CreateUDSet("checkWrite", checkWrite));
	std::auto_ptr<UDT::UDSET> exceptFds(CreateUDSet("checkError", checkError));
	
	if (UDT::ERROR == UDT::select(0, readFds.get(), writeFds.get(), exceptFds.get(), &tv))
	{
		throw Udt::SocketException::GetLastError("Error in socket select.");
	}

	Filter(readFds.get(), checkRead);
	Filter(writeFds.get(), checkWrite);
	Filter(exceptFds.get(), checkError);
}

void Udt::Socket::Connect(System::Net::IPEndPoint^ endPoint)
{
	if (endPoint == nullptr)
		throw gcnew ArgumentNullException("endPoint");

	Connect(endPoint->Address, endPoint->Port);
}

int Udt::Socket::Receive(cli::array<System::Byte>^ buffer)
{
	if (buffer == nullptr)
		throw gcnew ArgumentNullException("buffer");

	return Receive(buffer, 0, buffer->Length);
}

int Udt::Socket::Receive(cli::array<System::Byte>^ buffer, int offset, int size)
{
	if (buffer == nullptr)
		throw gcnew ArgumentNullException("buffer");

	if (offset < 0)
		throw gcnew ArgumentOutOfRangeException("offset", offset, "Value must be greater than or equal to 0.");

	if (size < 0)
		throw gcnew ArgumentOutOfRangeException("size", size, "Value must be greater than or equal to 0.");

	if ((offset + size) > buffer->Length)
		throw gcnew ArgumentException("Buffer is smaller than specified segment (count + size).", "buffer");

	cli::pin_ptr<unsigned char> buffer_pin = &buffer[0];
	unsigned char* buffer_pin_ptr = &buffer_pin[offset];

	int received = UDT::recv(_socket, (char*)buffer_pin_ptr, size, 0);

	if (UDT::ERROR == received)
	{
		throw Udt::SocketException::GetLastError("Error receiving data.");
	}

	return received;
}

int Udt::Socket::Send(cli::array<System::Byte>^ buffer)
{
	if (buffer == nullptr)
		throw gcnew ArgumentNullException("buffer");

	return Send(buffer, 0, buffer->Length);
}

int Udt::Socket::Send(cli::array<System::Byte>^ buffer, int offset, int size)
{
	if (buffer == nullptr)
		throw gcnew ArgumentNullException("buffer");

	if (offset < 0)
		throw gcnew ArgumentOutOfRangeException("offset", offset, "Value must be greater than or equal to 0.");

	if (size < 0)
		throw gcnew ArgumentOutOfRangeException("size", size, "Value must be greater than or equal to 0.");

	if ((offset + size) > buffer->Length)
		throw gcnew ArgumentException("Buffer is smaller than specified segment (count + size).", "buffer");

	cli::pin_ptr<unsigned char> buffer_pin = &buffer[0];
	unsigned char* buffer_pin_ptr = &buffer_pin[offset];

	int sent = UDT::send(_socket, (char*)buffer_pin_ptr, size, 0);

	if (UDT::ERROR == sent)
	{
		throw Udt::SocketException::GetLastError("Error sending data.");
	}

	return sent;
}

__int64 Udt::Socket::SendFile(System::String^ fileName)
{
	return SendFile(fileName, 0, -1);
}

__int64 Udt::Socket::SendFile(System::String^ fileName, __int64 offset)
{
	return SendFile(fileName, offset, -1);
}

__int64 Udt::Socket::SendFile(System::String^ fileName, __int64 offset, __int64 count)
{
	if (fileName == nullptr) throw gcnew ArgumentNullException("fileName");
	if (offset < 0) throw gcnew ArgumentOutOfRangeException("offset", offset, "Value must be greater than or equal to 0.");
	if (count < -1) throw gcnew ArgumentOutOfRangeException("count", count, "Value must be greater than or equal to -1.");

	cli::pin_ptr<const wchar_t> file_name_pin = PtrToStringChars(fileName);
	const wchar_t* file_name_ptr = file_name_pin;

	// In VC10, fstream tellg has a bug. Should be fixed in VC11.
	// http://connect.microsoft.com/VisualStudio/feedback/details/627639/std-fstream-use-32-bit-int-as-pos-type-even-on-x64-platform
	//std::fstream ifs(file_name_ptr, std::ios::in | std::ios::binary);

	//if (count < 0)
	//{
	//	ifs.seekg(0, std::ios::end);
	//	count = ifs.tellg() - offset;
	//}

	FILE* streamPtr = _wfsopen(file_name_ptr, L"rbN", _SH_DENYRW); // ifs will close handle
	std::fstream ifs(streamPtr);
	
	if (count < 0)
	{
		if (_fseeki64(streamPtr, offset, SEEK_END))
		{
			throw gcnew System::IO::IOException("Seek failed. Unable to determine file length.");
		}

		count = _ftelli64(streamPtr) - offset;
	}

	int64_t sent = UDT::sendfile(_socket, ifs, offset, count);

	if (UDT::ERROR == sent)
	{
		throw Udt::SocketException::GetLastError(String::Concat("Error sending file ", fileName));
	}

	return sent;
}

__int64 Udt::Socket::SendFile(StdFileStream^ file)
{
	return SendFile(file, -1);
}

__int64 Udt::Socket::SendFile(StdFileStream^ file, __int64 count)
{
	if (file == nullptr) throw gcnew ArgumentNullException("file");
	if (!file->CanRead) throw gcnew ArgumentException("Stream does not support reading.", "file");

	__int64 pos = file->Position;

	if (count < 0)
	{
		count = file->Length - pos;
	}

	int64_t sent = UDT::sendfile(_socket, file->LoadStdStream(), pos, count);

	if (UDT::ERROR == sent)
	{
		throw Udt::SocketException::GetLastError("Error sending file.");
	}

	return sent;
}

__int64 Udt::Socket::ReceiveFile(System::String^ fileName, __int64 length)
{
	if (fileName == nullptr) throw gcnew ArgumentNullException("fileName");
	if (length < 0) throw gcnew ArgumentOutOfRangeException("length", length, "Value must be greater than or equal to 0.");

	cli::pin_ptr<const wchar_t> file_name_pin = PtrToStringChars(fileName);
	const wchar_t* file_name_ptr = file_name_pin;
	std::fstream ofs(file_name_ptr, std::ios::out | std::ios::binary | std::ios::trunc);

	int64_t offset = 0;
	int64_t received = UDT::recvfile(_socket, ofs, offset, length);

	if (received == UDT::ERROR)
	{
		throw Udt::SocketException::GetLastError(String::Concat("Error receiving file ", fileName));
	}

	return received;
}

__int64 Udt::Socket::ReceiveFile(StdFileStream^ file, __int64 length)
{
	if (file == nullptr) throw gcnew ArgumentNullException("file");
	if (!file->CanWrite) throw gcnew ArgumentException("Stream does not support writing.", "file");
	if (length < 0) throw gcnew ArgumentOutOfRangeException("length", length, "Value must be greater than or equal to 0.");

	__int64 offset = file->Position;
	int64_t received = UDT::recvfile(_socket, file->LoadStdStream(), offset, length);

	if (received == UDT::ERROR)
	{
		throw Udt::SocketException::GetLastError("Error receiving file.");
	}

	return received;
}

TraceInfo^ Udt::Socket::GetPerformanceInfo()
{
	return GetPerformanceInfo(true);
}

TraceInfo^ Udt::Socket::GetPerformanceInfo(bool clear)
{
	UDT::TRACEINFO trace_info;

	if (UDT::ERROR == UDT::perfmon(_socket, &trace_info, clear))
	{
		throw Udt::SocketException::GetLastError("Error getting socket performance information.");
	}

	return gcnew TraceInfo(trace_info);
}

System::Net::Sockets::AddressFamily Udt::Socket::AddressFamily::get(void)
{
	return _addressFamily;
}

System::Net::Sockets::SocketType Udt::Socket::SocketType::get(void)
{
	return _socketType;
}

System::Net::IPEndPoint^ Udt::Socket::LocalEndPoint::get(void)
{
	sockaddr_storage local_addr;
	int local_addr_len = sizeof(local_addr);

	if (UDT::ERROR == UDT::getsockname(_socket, (sockaddr*)&local_addr, &local_addr_len))
	{
		throw Udt::SocketException::GetLastError("Error getting local end point.");
	}

	return ToEndPoint(&local_addr);
}

System::Net::IPEndPoint^ Udt::Socket::RemoteEndPoint::get(void)
{
	sockaddr_storage remote_addr;
	int remote_addr_len = sizeof(remote_addr);

	if (UDT::ERROR == UDT::getpeername(_socket, (sockaddr*)&remote_addr, &remote_addr_len))
	{
		throw Udt::SocketException::GetLastError("Error getting remote end point.");
	}

	return ToEndPoint(&remote_addr);
}

int Udt::Socket::SendMessage(cli::array<System::Byte>^ buffer)
{
	if (buffer == nullptr)
		throw gcnew ArgumentNullException("buffer");

	return SendMessage(buffer, 0, buffer->Length);
}

int Udt::Socket::SendMessage(cli::array<System::Byte>^ buffer, int offset, int size)
{
	if (buffer == nullptr)
		throw gcnew ArgumentNullException("buffer");

	if (offset < 0)
		throw gcnew ArgumentOutOfRangeException("offset", offset, "Value must be greater than or equal to 0.");

	if (size < 0)
		throw gcnew ArgumentOutOfRangeException("size", size, "Value must be greater than or equal to 0.");

	if ((offset + size) > buffer->Length)
		throw gcnew ArgumentException("Buffer is smaller than specified segment (count + size).", "buffer");

	return UdtSendMessage(_socket, buffer, offset, size);
}

int Udt::Socket::SendMessage(Message^ message)
{
	if (message == nullptr)
		throw gcnew ArgumentNullException("message");

	ArraySegment<Byte> buffer = message->Buffer;
	int ttl = (int)message->TimeToLive.TotalMilliseconds;
	return UdtSendMessage(_socket, buffer.Array, buffer.Offset, buffer.Count, ttl, message->InOrder);
}

int Udt::Socket::ReceiveMessage(cli::array<System::Byte>^ buffer)
{
	if (buffer == nullptr)
		throw gcnew ArgumentNullException("buffer");

	return ReceiveMessage(buffer, 0, buffer->Length);
}

int Udt::Socket::ReceiveMessage(cli::array<System::Byte>^ buffer, int offset, int size)
{
	if (buffer == nullptr)
		throw gcnew ArgumentNullException("buffer");

	if (offset < 0)
		throw gcnew ArgumentOutOfRangeException("offset", offset, "Value must be greater than or equal to 0.");

	if (size < 0)
		throw gcnew ArgumentOutOfRangeException("size", size, "Value must be greater than or equal to 0.");

	if ((offset + size) > buffer->Length)
		throw gcnew ArgumentException("Buffer is smaller than specified segment (count + size).", "buffer");

	cli::pin_ptr<unsigned char> buffer_pin = &buffer[0];
	unsigned char* buffer_ptr = &buffer_pin[offset];

	int result = UDT::recvmsg(_socket, (char*)buffer_ptr, size);

	if (UDT::ERROR == result)
	{
		throw Udt::SocketException::GetLastError("Error receiving message.");
	}

	return result;
}

void Udt::Socket::SetSocketOptionInt32(Udt::SocketOptionName name, int value)
{
	if (UDT::ERROR == UDT::setsockopt(_socket, 0, (UDT::SOCKOPT)name, &value, sizeof(int)))
	{
		throw Udt::SocketException::GetLastError(String::Concat("Error setting socket option ", name.ToString(), " to ", (Object^)value, "."));
	}
}

void Udt::Socket::SetSocketOptionInt64(Udt::SocketOptionName name, __int64 value)
{
	if (UDT::ERROR == UDT::setsockopt(_socket, 0, (UDT::SOCKOPT)name, &value, sizeof(__int64)))
	{
		throw Udt::SocketException::GetLastError(String::Concat("Error setting socket option ", name.ToString(), " to ", (Object^)value, "."));
	}
}

void Udt::Socket::SetSocketOptionBoolean(Udt::SocketOptionName name, bool value)
{
	if (UDT::ERROR == UDT::setsockopt(_socket, 0, (UDT::SOCKOPT)name, &value, sizeof(bool)))
	{
		throw Udt::SocketException::GetLastError(String::Concat("Error setting socket option ", name.ToString(), " to ", (Object^)value, "."));
	}
}

void Udt::Socket::SetSocketOption(Udt::SocketOptionName name, int value)
{
	switch (name)
	{
	case Udt::SocketOptionName::BlockingReceive:
	case Udt::SocketOptionName::BlockingSend:
	case Udt::SocketOptionName::Rendezvous:
	case Udt::SocketOptionName::ReuseAddress:
		SetSocketOptionBoolean(name, value != 0);
		break;

	case Udt::SocketOptionName::MaxPacketSize:
	case Udt::SocketOptionName::MaxWindowSize:
	case Udt::SocketOptionName::SendBuffer:
	case Udt::SocketOptionName::ReceiveBuffer:
	case Udt::SocketOptionName::UdpReceiveBuffer:
	case Udt::SocketOptionName::UdpSendBuffer:
	case Udt::SocketOptionName::SendTimeout:
	case Udt::SocketOptionName::ReceiveTimeout:
		SetSocketOptionInt32(name, value);
		break;

	case Udt::SocketOptionName::MaxBandwidth:
		SetSocketOptionInt64(name, (long)value);
		break;

	case Udt::SocketOptionName::Linger:
	case Udt::SocketOptionName::CongestionControl:
		throw gcnew ArgumentException(System::String::Concat("Socket option ", name, " can not be set to Int32 value"), "value");

	default:
		throw gcnew ArgumentException("Unhandled socket option name.", "name");
	}
}

void Udt::Socket::SetSocketOption(Udt::SocketOptionName name, __int64 value)
{
	switch (name)
	{
	case Udt::SocketOptionName::BlockingReceive:
	case Udt::SocketOptionName::BlockingSend:
	case Udt::SocketOptionName::Rendezvous:
	case Udt::SocketOptionName::ReuseAddress:
		SetSocketOptionBoolean(name, value != 0);
		break;

	case Udt::SocketOptionName::MaxPacketSize:
	case Udt::SocketOptionName::MaxWindowSize:
	case Udt::SocketOptionName::SendBuffer:
	case Udt::SocketOptionName::ReceiveBuffer:
	case Udt::SocketOptionName::UdpReceiveBuffer:
	case Udt::SocketOptionName::UdpSendBuffer:
	case Udt::SocketOptionName::SendTimeout:
	case Udt::SocketOptionName::ReceiveTimeout:
		SetSocketOptionInt32(name, (int)value);
		break;

	case Udt::SocketOptionName::MaxBandwidth:
		SetSocketOptionInt64(name, value);
		break;

	case Udt::SocketOptionName::Linger:
	case Udt::SocketOptionName::CongestionControl:
		throw gcnew ArgumentException(System::String::Concat("Socket option ", name, " can not be set to Int64 value"), "value");

	default:
		throw gcnew ArgumentException("Unhandled socket option name.", "name");
	}
}

void Udt::Socket::SetSocketOption(Udt::SocketOptionName name, bool value)
{
	switch (name)
	{
	case Udt::SocketOptionName::BlockingReceive:
	case Udt::SocketOptionName::BlockingSend:
	case Udt::SocketOptionName::Rendezvous:
	case Udt::SocketOptionName::ReuseAddress:
		SetSocketOptionBoolean(name, value);
		break;

	case Udt::SocketOptionName::MaxPacketSize:
	case Udt::SocketOptionName::MaxWindowSize:
	case Udt::SocketOptionName::SendBuffer:
	case Udt::SocketOptionName::ReceiveBuffer:
	case Udt::SocketOptionName::UdpReceiveBuffer:
	case Udt::SocketOptionName::UdpSendBuffer:
	case Udt::SocketOptionName::SendTimeout:
	case Udt::SocketOptionName::ReceiveTimeout:
		SetSocketOptionInt32(name, value ? 1 : 0);
		break;

	case Udt::SocketOptionName::MaxBandwidth:
		SetSocketOptionInt64(name, value ? 1L : 0L);
		break;

	case Udt::SocketOptionName::Linger:
	case Udt::SocketOptionName::CongestionControl:
		throw gcnew ArgumentException(System::String::Concat("Socket option ", name, " can not be set to Boolean value"), "value");

	default:
		throw gcnew ArgumentException("Unhandled socket option name.", "name");
	}
}

void Udt::Socket::SetSocketOption(Udt::SocketOptionName name, System::Object^ value)
{
	if (name == Udt::SocketOptionName::Linger)
	{
		if (value == nullptr)
			throw gcnew ArgumentNullException("value");

		if (System::Net::Sockets::LingerOption::typeid->IsAssignableFrom(value->GetType()))
		{
			System::Net::Sockets::LingerOption^ lingerOpt = (System::Net::Sockets::LingerOption^)value;

			linger lingerValue;
			lingerValue.l_onoff = lingerOpt->Enabled ? 1 : 0;
			lingerValue.l_linger = lingerOpt->LingerTime;

			if (UDT::ERROR == UDT::setsockopt(_socket, 0, (UDT::SOCKOPT)name, &lingerValue, sizeof(linger)))
			{
				throw Udt::SocketException::GetLastError(String::Concat("Error setting socket option ", name.ToString(), " to ", value->ToString(), "."));
			}
		}
		else
		{
			throw gcnew ArgumentException("Linger socket option value must be of type System.Net.Sockets.LingerOption", "value");
		}
	}
	else if (name == Udt::SocketOptionName::CongestionControl)
	{
		if (value != _congestionControl)
		{
			if (value == nullptr)
			{
				CCCFactory<CUDTCC> factory;

				if (UDT::ERROR == UDT::setsockopt(_socket, 0, (UDT::SOCKOPT)name, &factory, sizeof(CCCVirtualFactory)))
				{
					throw Udt::SocketException::GetLastError(String::Concat("Error clearing socket option ", name.ToString(), "."));
				}

				_congestionControl = nullptr;
			}
			else if (ICongestionControlFactory::typeid->IsAssignableFrom(value->GetType()))
			{
				ICongestionControlFactory^ ccValue = (ICongestionControlFactory^)value;
				CCCWrapperFactory factory(ccValue);

				if (UDT::ERROR == UDT::setsockopt(_socket, 0, (UDT::SOCKOPT)name, &factory, sizeof(CCCWrapperFactory)))
				{
					throw Udt::SocketException::GetLastError(String::Concat("Error setting socket option ", name.ToString(), " to ", value->ToString(), "."));
				}

				_congestionControl = ccValue;
			}
			else
			{
				throw gcnew ArgumentException(System::String::Concat("Socket option ", name, " can not be set to ", value->GetType()->Name, " value"), "value");
			}
		}
	}
	else
	{
		if (value == nullptr)
			throw gcnew ArgumentNullException("value");

		switch (name)
		{
		case Udt::SocketOptionName::BlockingReceive:
		case Udt::SocketOptionName::BlockingSend:
		case Udt::SocketOptionName::Rendezvous:
		case Udt::SocketOptionName::ReuseAddress:
			SetSocketOptionBoolean(name, System::Convert::ToBoolean(value, CultureInfo::InvariantCulture));
			break;

		case Udt::SocketOptionName::MaxPacketSize:
		case Udt::SocketOptionName::MaxWindowSize:
		case Udt::SocketOptionName::SendBuffer:
		case Udt::SocketOptionName::ReceiveBuffer:
		case Udt::SocketOptionName::UdpReceiveBuffer:
		case Udt::SocketOptionName::UdpSendBuffer:
		case Udt::SocketOptionName::SendTimeout:
		case Udt::SocketOptionName::ReceiveTimeout:
			SetSocketOptionInt32(name, System::Convert::ToInt32(value, CultureInfo::InvariantCulture));
			break;

		case Udt::SocketOptionName::MaxBandwidth:
			SetSocketOptionInt64(name, System::Convert::ToInt64(value, CultureInfo::InvariantCulture));
			break;

		default:
			throw gcnew ArgumentException("Unhandled socket option name.", "name");
		}
	}
}

System::Object^ Udt::Socket::GetSocketOption(Udt::SocketOptionName name)
{
	switch (name)
	{
	case Udt::SocketOptionName::BlockingReceive:
	case Udt::SocketOptionName::BlockingSend:
	case Udt::SocketOptionName::Rendezvous:
	case Udt::SocketOptionName::ReuseAddress:
		return GetSocketOptionBoolean(name);

	case Udt::SocketOptionName::MaxPacketSize:
	case Udt::SocketOptionName::MaxWindowSize:
	case Udt::SocketOptionName::SendBuffer:
	case Udt::SocketOptionName::ReceiveBuffer:
	case Udt::SocketOptionName::UdpReceiveBuffer:
	case Udt::SocketOptionName::UdpSendBuffer:
	case Udt::SocketOptionName::SendTimeout:
	case Udt::SocketOptionName::ReceiveTimeout:
		return GetSocketOptionInt32(name);

	case Udt::SocketOptionName::MaxBandwidth:
		return GetSocketOptionInt64(name);

	case Udt::SocketOptionName::Linger:
		{
			linger value;
			int valueLen = sizeof(linger);

			if (UDT::ERROR == UDT::getsockopt(_socket, 0, (UDT::SOCKOPT)name, &value, &valueLen))
			{
				throw Udt::SocketException::GetLastError(String::Concat("Error getting socket option ", name.ToString(), "."));
			}

			return gcnew System::Net::Sockets::LingerOption(value.l_onoff != 0, value.l_linger);
		}

	case Udt::SocketOptionName::CongestionControl:
		return _congestionControl;

	default:
		throw gcnew ArgumentException("Unhandled socket option name.", "name");
	}
}

int Udt::Socket::GetSocketOptionInt32(Udt::SocketOptionName name)
{
	int value;
	int valueLen = sizeof(int);

	if (UDT::ERROR == UDT::getsockopt(_socket, 0, (UDT::SOCKOPT)name, &value, &valueLen))
	{
		throw Udt::SocketException::GetLastError(String::Concat("Error getting socket option ", name.ToString(), "."));
	}

	return value;
}

__int64 Udt::Socket::GetSocketOptionInt64(Udt::SocketOptionName name)
{
	__int64 value;
	int valueLen = sizeof(__int64);

	if (UDT::ERROR == UDT::getsockopt(_socket, 0, (UDT::SOCKOPT)name, &value, &valueLen))
	{
		throw Udt::SocketException::GetLastError(String::Concat("Error getting socket option ", name.ToString(), "."));
	}

	return value;
}

bool Udt::Socket::GetSocketOptionBoolean(Udt::SocketOptionName name)
{
	bool value;
	int valueLen = sizeof(bool);

	if (UDT::ERROR == UDT::getsockopt(_socket, 0, (UDT::SOCKOPT)name, &value, &valueLen))
	{
		throw Udt::SocketException::GetLastError(String::Concat("Error getting socket option ", name.ToString(), "."));
	}

	return value;
}
