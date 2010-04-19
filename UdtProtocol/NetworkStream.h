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

namespace Udt
{
	ref class Socket; // Forward declaration

	/// <summary>
	/// Provides a <see cref="System::IO::Stream"/> interface to a UDT socket.
	/// </summary>
	public ref class NetworkStream : System::IO::Stream
	{
	private:
		Socket^ _socket;
		initonly bool _ownsSocket;
		initonly System::IO::FileAccess _access;

		void Initialize();

	protected:

		/// <summary>
		/// Get the underlying socket.
		/// </summary>
		property Udt::Socket^ Socket
		{
			Udt::Socket^ get(void);
		}

		void AssertNotDisposed(void);
		
	public:

		/// <summary>
		/// Initialize a new instance.
		/// </summary>
		/// <param name="socket">Socket this stream will use to send/receive data.</param>
		/// <exception cref="System::ArgumentNullException">If <paramref name="socket"/> is a null reference.</exception>
		/// <exception cref="System::ArgumentException">
		/// If <paramref name="socket"/>.SocketType is not <see cref="System::Net::Sockets::SocketType::Stream"/><br/>
		/// <b>- or -</b><br/>
		/// <paramref name="socket"/> is in non-blocking mode
		/// </exception>
		NetworkStream(Udt::Socket^ socket);

		/// <summary>
		/// Initialize a new instance.
		/// </summary>
		/// <param name="socket">Socket this stream will use to send/receive data.</param>
		/// <param name="ownsSocket">True if this stream will assume ownership of the <paramref name="socket"/>.</param>
		/// <exception cref="System::ArgumentNullException">If <paramref name="socket"/> is a null reference.</exception>
		/// <exception cref="System::ArgumentException">
		/// If <paramref name="socket"/>.SocketType is not <see cref="System::Net::Sockets::SocketType::Stream"/><br/>
		/// <b>- or -</b><br/>
		/// <paramref name="socket"/> is in non-blocking mode
		/// </exception>
		NetworkStream(Udt::Socket^ socket, bool ownsSocket);

		/// <summary>
		/// Initialize a new instance.
		/// </summary>
		/// <param name="socket">Socket this stream will use to send/receive data.</param>
		/// <param name="access">Type of access to the <paramref name="socket"/> given to the stream.</param>
		/// <exception cref="System::ArgumentNullException">If <paramref name="socket"/> is a null reference.</exception>
		/// <exception cref="System::ArgumentException">
		/// If <paramref name="socket"/>.SocketType is not <see cref="System::Net::Sockets::SocketType::Stream"/><br/>
		/// <b>- or -</b><br/>
		/// <paramref name="socket"/> is in non-blocking mode
		/// </exception>
		NetworkStream(Udt::Socket^ socket, System::IO::FileAccess access);

		/// <summary>
		/// Initialize a new instance.
		/// </summary>
		/// <param name="socket">Socket this stream will use to send/receive data.</param>
		/// <param name="access">Type of access to the <paramref name="socket"/> given to the stream.</param>
		/// <param name="ownsSocket">True if this stream will assume ownership of the <paramref name="socket"/>.</param>
		/// <exception cref="System::ArgumentNullException">If <paramref name="socket"/> is a null reference.</exception>
		/// <exception cref="System::ArgumentException">
		/// If <paramref name="socket"/>.SocketType is not <see cref="System::Net::Sockets::SocketType::Stream"/><br/>
		/// <b>- or -</b><br/>
		/// <paramref name="socket"/> is in non-blocking mode
		/// </exception>
		NetworkStream(Udt::Socket^ socket, System::IO::FileAccess access, bool ownsSocket);

		/// <summary>
		/// Dispose of the stream.
		/// </summary>
		virtual ~NetworkStream();

		virtual property bool CanWrite { bool get(void) override; }
		virtual property bool CanRead { bool get(void) override; }
		virtual property bool CanSeek { bool get(void) override { return false; } }
		virtual property __int64 Length { __int64 get(void) override; }
		virtual property __int64 Position { __int64 get(void) override; void set(__int64 value) override; }

		virtual void Flush() override;
		virtual int Read(cli::array<System::Byte>^ buffer, int offset, int count) override;
		virtual void Write(cli::array<System::Byte>^ buffer, int offset, int count) override;
		virtual __int64 Seek(__int64 offset, System::IO::SeekOrigin origin) override;
		virtual void SetLength(__int64 value) override;
	};
}
