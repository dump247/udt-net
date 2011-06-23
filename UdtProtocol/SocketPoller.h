#pragma once
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

#include <udt.h>

namespace Udt
{
	ref class Socket;
	ref class SocketException;

	/// <summary>
	/// Used to poll IO events from multiple sockets.
	/// </summary>
	public ref class SocketPoller
	{
	private:
		int _epollId;
		System::Collections::Generic::Dictionary<UDTSOCKET, Udt::Socket^>^ _pollSockets;
		System::Collections::Generic::ICollection<Udt::Socket^>^ _readSockets;
		System::Collections::Generic::ICollection<Udt::Socket^>^ _writeSockets;

		static cli::array<Udt::Socket^>^ EmptySocketList = gcnew cli::array<Udt::Socket^>(0);

		void AssertNotDisposed();
		System::Collections::Generic::List<Udt::Socket^>^ GetSockets(std::set<UDTSOCKET>& handles);

	public:

		/// <summary>
		/// Initialize a new instance.
		/// </summary>
		SocketPoller(void);

		virtual ~SocketPoller(void);

		/// <summary>
		/// Add a socket to the poller.
		/// </summary>
		/// <remarks>
		/// If the <paramref name="socket"/> has already been added to the poller,
		/// it will be ignored if added again.
		/// </remarks>
		/// <param name="socket">Socket to add.</param>
		/// <exception cref="System::ArgumentNullException">If <paramref name="socket"/> is null.</exception>
		/// <exception cref="Udt::SocketException">If an error occurs adding the socket.</exception>
		/// <exception cref="System::ObjectDisposedException">If the object has been disposed.</exception>
		void AddSocket(Udt::Socket^ socket);

		/// <summary>
		/// Remove a socket from the poller.
		/// </summary>
		/// <param name="socket">Socket to remove.</param>
		/// <exception cref="System::ArgumentNullException">If <paramref name="socket"/> is null.</exception>
		/// <exception cref="Udt::SocketException">If an error occurs removing the socket.</exception>
		/// <exception cref="System::ObjectDisposedException">If the object has been disposed.</exception>
		void RemoveSocket(Udt::Socket^ socket);

		/// <summary>
		/// Wait indefinitely for a socket event to occur.
		/// </summary>
		/// <exception cref="Udt::SocketException">If an error occurs waiting.</exception>
		/// <exception cref="System::InvalidOperationException">If no sockets have been added to the poller.</exception>
		/// <exception cref="System::ObjectDisposedException">If the object has been disposed.</exception>
		void Wait();

		/// <summary>
		/// Wait for a socket event to occur.
		/// </summary>
		/// <remarks>
		/// Use <see cref="ReadSockets"/> and <see cref="WriteSockets"/> to get
		/// the sockets an event occurred on.
		/// </remarks>
		/// <param name="timeout">Maximum amount of time to wait for an event to occur or -1 milliseconds to wait indefinitely.</param>
		/// <returns>True if an event occurred before the timeout expired.</returns>
		/// <exception cref="Udt::SocketException">If an error occurs waiting.</exception>
		/// <exception cref="System::InvalidOperationException">If no sockets have been added to the poller.</exception>
		/// <exception cref="System::ObjectDisposedException">If the object has been disposed.</exception>
		bool Wait(System::TimeSpan timeout);

		/// <summary>
		/// Sockets that are ready to read or empty for none.
		/// By default the collection is empty.
		/// </summary>
		/// <remarks>
		/// The collection is read-only. A new collection instance is
		/// created each time <see cref="Wait()"/> is called.
		/// </remarks>
		property System::Collections::Generic::ICollection<Udt::Socket^>^ ReadSockets
		{
			System::Collections::Generic::ICollection<Udt::Socket^>^ get(void);
		}

		/// <summary>
		/// Sockets that are ready to write or broken or empty for none.
		/// By default the collection is empty.
		/// </summary>
		/// <remarks>
		/// The collection is read-only. A new collection instance is
		/// created each time <see cref="Wait()"/> is called.
		/// </remarks>
		property System::Collections::Generic::ICollection<Udt::Socket^>^ WriteSockets
		{
			System::Collections::Generic::ICollection<Udt::Socket^>^ get(void);
		}
	};
}
