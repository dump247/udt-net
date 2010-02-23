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

#include "Message.h"
#include "TraceInfo.h"
#include "SocketOptionName.h"
#include "CongestionControl.h"
#include "SocketException.h"
#include <udt.h>

namespace Udt
{
	/// <summary>
	/// Interface to a UDT socket.
	/// </summary>
	public ref class Socket
	{
	private:
		UDTSOCKET _socket;
		System::Net::Sockets::AddressFamily _addressFamily;
		System::Net::Sockets::SocketType _socketType;
		CongestionControl^ _congestionControl;

		Socket(UDTSOCKET socket, System::Net::Sockets::AddressFamily family, System::Net::Sockets::SocketType type);

		static Socket(void)
		{
			if (UDT::ERROR == UDT::startup())
				throw SocketException::GetLastError("Error in UDT initialization");
		}

		int GetSocketOptionInt32(SocketOptionName name);
		__int64 GetSocketOptionInt64(SocketOptionName name);
		bool GetSocketOptionBoolean(SocketOptionName name);

	public:

		/// <summary>
		/// Initialize a new instance using the specified address family and
		/// socket type.
		/// </summary>
		/// <param name="family">Address family.</param>
		/// <param name="type">Socket type.</param>
		/// <exception cref="System::ArgumentException">
		/// <paramref name="family"/> is not either <c>InterNetwork</c> or <c>InterNetworkV6</c><br/>
		/// <b>- or -</b><br/>
		/// <paramref name="type"/> is not either <c>Dgram</c> or <c>Stream</c>
		/// </exception>
		/// <exception cref="Udt::SocketException">If an error occurs creating the socket.</exception>
		Socket(System::Net::Sockets::AddressFamily family, System::Net::Sockets::SocketType type);

		/// <summary>
		/// Closes the socket.
		/// </summary>
		~Socket(void);

		/// <summary>
		/// Close the socket and release any associated resources.
		/// </summary>
		void Close(void);

		/// <summary>
		/// Associate the socket with a local end point.
		/// </summary>
		/// <exception cref="System::ArgumentNullException">
		/// If <paramref name="address"/> is a null reference
		/// </exception>
		/// <exception cref="System::ArgumentException">
		/// If the type of <paramref name="address"/> is not compatible with the
		/// <b>AddressFamily</b> passed to the <b>Socket(AddressFamily,SocketType)</b>
		/// constructor.
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// If <paramref name="port"/> is less than <see cref="System::Net::IPEndPoint::MinPort"/>
		/// or greater than <see cref="System::Net::IPEndPoint::MaxPort"/>.
		/// </exception>
		/// <exception cref="Udt::SocketException">
		/// If an error occurs binding the socket (i.e. the socket is already bound, etc).
		/// </exception>
		void Bind(System::Net::IPAddress^ address, int port);

		/// <summary>
		/// Associate the socket with a local end point.
		/// </summary>
		/// <exception cref="System::ArgumentNullException">
		/// If <paramref name="endPoint"/> is a null reference
		/// </exception>
		/// <exception cref="System::ArgumentException">
		/// If the type of address in <paramref name="endPoint"/> is not compatible with the
		/// <b>AddressFamily</b> passed to the <b>Socket(AddressFamily,SocketType)</b>
		/// constructor.
		/// </exception>
		/// <exception cref="Udt::SocketException">
		/// If an error occurs binding the socket (i.e. the socket is already bound, etc).
		/// </exception>
		void Bind(System::Net::IPEndPoint^ endPoint);

		/// <summary>
		/// Places the socket in a listening state.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// If <paramref name="backlog"/> is less than 1.
		/// </exception>
		/// <exception cref="Udt::SocketException">If an error occurs.</exception>
		void Listen(int backlog);

		/// <summary>
		/// Creates a <see cref="Socket"/> for a newly created connection.
		/// </summary>
		/// <remarks>
		/// <b>Accept</b> synchronously extracts the first pending connection
		/// request from the connection request queue of the listening socket,
		/// and then creates and returns a new <see cref="Socket"/>.
		/// </remarks>
		/// <exception cref="Udt::SocketException">If an error occurs.</exception>
		Socket^ Accept();

		/// <summary>
		/// Establishes a connection to a remote host.
		/// </summary>
		/// <param name="host">Name of the host to connect to.</param>
		/// <param name="port">Port to connect to.</param>
		/// <exception cref="System::ArgumentNullException">
		/// If <paramref name="host"/> is a null reference
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// If <paramref name="port"/> is less than <see cref="System::Net::IPEndPoint::MinPort"/>
		/// or greater than <see cref="System::Net::IPEndPoint::MaxPort"/>.
		/// </exception>
		/// <exception cref="Udt::SocketException">If an error occurs.</exception>
		void Connect(System::String^ host, int port);

		/// <summary>
		/// Establishes a connection to a remote host.
		/// </summary>
		/// <param name="address">Address of the host to connect to.</param>
		/// <param name="port">Port to connect to.</param>
		/// <exception cref="System::ArgumentNullException">
		/// If <paramref name="address"/> is a null reference
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// If <paramref name="port"/> is less than <see cref="System::Net::IPEndPoint::MinPort"/>
		/// or greater than <see cref="System::Net::IPEndPoint::MaxPort"/>.
		/// </exception>
		/// <exception cref="Udt::SocketException">If an error occurs.</exception>
		void Connect(System::Net::IPAddress^ address, int port);

		/// <summary>
		/// Establishes a connection to a remote host.
		/// </summary>
		/// <param name="addresses">Addresses of the host to connect to.</param>
		/// <param name="port">Port to connect to.</param>
		/// <exception cref="System::ArgumentNullException">
		/// If <paramref name="addresses"/> is a null reference
		/// </exception>
		/// <exception cref="System::ArgumentNullException">
		/// If <paramref name="addresses"/> is empty
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// If <paramref name="port"/> is less than <see cref="System::Net::IPEndPoint::MinPort"/>
		/// or greater than <see cref="System::Net::IPEndPoint::MaxPort"/>.
		/// </exception>
		/// <exception cref="Udt::SocketException">If an error occurs.</exception>
		void Connect(cli::array<System::Net::IPAddress^>^ addresses, int port);

		/// <summary>
		/// Establishes a connection to a remote host.
		/// </summary>
		/// <param name="endPoint">Remote end point to connect to.</param>
		/// <exception cref="System::ArgumentNullException">
		/// If <paramref name="endPoint"/> is a null reference
		/// </exception>
		/// <exception cref="Udt::SocketException">If an error occurs.</exception>
		void Connect(System::Net::IPEndPoint^ endPoint);

		int Receive(cli::array<System::Byte>^ buffer);
		int Receive(cli::array<System::Byte>^ buffer, int offset, int size);
		int Send(cli::array<System::Byte>^ buffer);
		int Send(cli::array<System::Byte>^ buffer, int offset, int size);

		/// <summary>
		/// Send the contents of a file on this socket.
		/// </summary>
		/// <remarks>
		/// Does not send a file size.
		/// </remarks>
		/// <param name="fileName">Name of the local file to send.</param>
		/// <returns>The total number of bytes sent.</returns>
		/// <exception cref="Udt::SocketException">If an error occurs accessing the socket or the file.</exception>
		__int64 SendFile(System::String^ fileName);

		/// <summary>
		/// Receive data on this socket and store it in a local file.
		/// </summary>
		/// <param name="fileName">Name of the local file to write the data to.</param>
		/// <param name="length">Number of bytes to read from the socket into <paramref name="fileName"/></param>
		/// <returns>The total number of bytes received.</returns>
		/// <exception cref="Udt::SocketException">If an error occurs accessing the socket or the file.</exception>
		__int64 ReceiveFile(System::String^ fileName, __int64 length);

		int SendMessage(cli::array<System::Byte>^ buffer);
		int SendMessage(cli::array<System::Byte>^ buffer, int offset, int size);
		int SendMessage(Message^ message);

		int ReceiveMessage(cli::array<System::Byte>^ buffer);
		int ReceiveMessage(cli::array<System::Byte>^ buffer, int offset, int size);

		TraceInfo^ GetPerformanceInfo();
		TraceInfo^ GetPerformanceInfo(bool clear);

		void SetSocketOption(SocketOptionName name, int value);
		void SetSocketOption(SocketOptionName name, __int64 value);
		void SetSocketOption(SocketOptionName name, bool value);
		void SetSocketOption(SocketOptionName name, System::Object^ value);

		System::Object^ GetSocketOption(SocketOptionName name);

		/// <summary>
		/// Gets the local end point.
		/// </summary>
		/// <value>
		/// The local end point that the socket is using for communications.
		/// </value>
		/// <exception cref="Udt::SocketException">If an error occurs accessing the socket.</exception>
		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1702:CompoundWordsShouldBeCasedCorrectly",
			Justification = "EndPoint is the casing used in IPEndPoint")]
		property System::Net::IPEndPoint^ LocalEndPoint
		{
			System::Net::IPEndPoint^ get(void);
		}

		/// <summary>
		/// Gets the address family of the socket.
		/// </summary>
		property System::Net::Sockets::AddressFamily AddressFamily
		{
			System::Net::Sockets::AddressFamily get(void);
		}

		/// <summary>
		/// Gets the type of the socket.
		/// </summary>
		property System::Net::Sockets::SocketType SocketType
		{
			System::Net::Sockets::SocketType get(void);
		}

		/// <summary>
		/// Gets the remote end point.
		/// </summary>
		/// <value>
		/// The remote end point that the socket is using for communications.
		/// </value>
		/// <exception cref="Udt::SocketException">If an error occurs accessing the socket.</exception>
		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1702:CompoundWordsShouldBeCasedCorrectly",
			Justification = "EndPoint is the casing used in IPEndPoint")]
		property System::Net::IPEndPoint^ RemoteEndPoint
		{
			System::Net::IPEndPoint^ get(void);
		}

		property int SendBufferSize
		{
			int get(void) { return GetSocketOptionInt32(Udt::SocketOptionName::SendBuffer); }
			void set(int value) { SetSocketOption(Udt::SocketOptionName::SendBuffer, value); }
		}

		property int UdpSendBufferSize
		{
			int get(void) { return GetSocketOptionInt32(Udt::SocketOptionName::UdpSendBuffer); }
			void set(int value) { SetSocketOption(Udt::SocketOptionName::UdpSendBuffer, value); }
		}

		property int SendTimeout
		{
			int get(void) { return GetSocketOptionInt32(Udt::SocketOptionName::SendTimeout); }
			void set(int value) { SetSocketOption(Udt::SocketOptionName::SendTimeout, value); }
		}

		property bool BlockingSend
		{
			bool get(void) { return GetSocketOptionBoolean(Udt::SocketOptionName::BlockingSend); }
			void set(bool value) { SetSocketOption(Udt::SocketOptionName::BlockingSend, value); }
		}

		property int ReceiveBufferSize
		{
			int get(void) { return GetSocketOptionInt32(Udt::SocketOptionName::ReceiveBuffer); }
			void set(int value) { SetSocketOption(Udt::SocketOptionName::ReceiveBuffer, value); }
		}

		property int UdpReceiveBufferSize
		{
			int get(void) { return GetSocketOptionInt32(Udt::SocketOptionName::UdpReceiveBuffer); }
			void set(int value) { SetSocketOption(Udt::SocketOptionName::UdpReceiveBuffer, value); }
		}

		property int ReceiveTimeout
		{
			int get(void) { return GetSocketOptionInt32(Udt::SocketOptionName::ReceiveTimeout); }
			void set(int value) { SetSocketOption(Udt::SocketOptionName::ReceiveTimeout, value); }
		}

		property bool BlockingReceive
		{
			bool get(void) { return GetSocketOptionBoolean(Udt::SocketOptionName::BlockingReceive); }
			void set(bool value) { SetSocketOption(Udt::SocketOptionName::BlockingReceive, value); }
		}

		property bool Rendezvous
		{
			bool get(void) { return GetSocketOptionBoolean(Udt::SocketOptionName::Rendezvous); }
			void set(bool value) { SetSocketOption(Udt::SocketOptionName::Rendezvous, value); }
		}

		property bool ReuseAddress
		{
			bool get(void) { return GetSocketOptionBoolean(Udt::SocketOptionName::ReuseAddress); }
			void set(bool value) { SetSocketOption(Udt::SocketOptionName::ReuseAddress, value); }
		}

		property __int64 MaxBandwidth
		{
			__int64 get(void) { return GetSocketOptionInt64(Udt::SocketOptionName::MaxBandwidth); }
			void set(__int64 value) { SetSocketOption(Udt::SocketOptionName::MaxBandwidth, value); }
		}

		property System::Net::Sockets::LingerOption^ LingerState
		{
			System::Net::Sockets::LingerOption^ get(void)
			{
				return (System::Net::Sockets::LingerOption^)GetSocketOption(Udt::SocketOptionName::Linger);
			}

			void set(System::Net::Sockets::LingerOption^ value)
			{
				SetSocketOption(Udt::SocketOptionName::Linger, value);
			}
		}

		property int MaxPacketSize
		{
			int get(void) { return GetSocketOptionInt32(Udt::SocketOptionName::MaxPacketSize); }
			void set(int value) { SetSocketOption(Udt::SocketOptionName::MaxPacketSize, value); }
		}

		property int MaxWindowSize
		{
			int get(void) { return GetSocketOptionInt32(Udt::SocketOptionName::MaxWindowSize); }
			void set(int value) { SetSocketOption(Udt::SocketOptionName::MaxWindowSize, value); }
		}

		property Udt::CongestionControl^ CongestionControl
		{
			Udt::CongestionControl^ get(void)
			{
				return (Udt::CongestionControl^)GetSocketOption(Udt::SocketOptionName::CongestionControl);
			}

			void set(Udt::CongestionControl^ value)
			{
				SetSocketOption(Udt::SocketOptionName::CongestionControl, value);
			}
		}
	};
}
