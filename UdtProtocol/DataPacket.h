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

#include "Packet.h"
#include "MessageBoundary.h"

namespace Udt
{
	/// <summary>
	/// UDT protocol data packet.
	/// </summary>
	public ref class DataPacket : public Packet
	{
	private:

		int _capacity;

		void EnsureCapacity(int value);

	protected:

		virtual void FreePacketData() override;
		
	internal:

		DataPacket(const CPacket* packet);

	public:
		DataPacket(void);
		virtual ~DataPacket(void);

		/// <summary>
		/// Get or set the location of the packet in the stream.
		/// Default value is <b>None</b>.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">If <paramref name="value"/> is invalid.</exception>
		/// <exception cref="System::ObjectDisposedException">If the object has been disposed.</exception>
		/// <exception cref="System::InvalidOperationException">If attempting to set the value and <see cref="IsEditable"/> is false.</exception>
		property Udt::MessageBoundary MessageBoundary {
			Udt::MessageBoundary get(void);
			void set(Udt::MessageBoundary value);
		}

		/// <summary>
		/// Get or set true if the packet in-order delivery for the packet is
		/// required. Default value is false.
		/// </summary>
		/// <exception cref="System::ObjectDisposedException">If the object has been disposed.</exception>
		/// <exception cref="System::InvalidOperationException">If attempting to set the value and <see cref="IsEditable"/> is false.</exception>
		property bool InOrder {
			bool get(void);
			void set(bool value);
		}

		/// <summary>
		/// Get or set the message sequence number for the packet.
		/// Default value is 0.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">If <paramref name="value"/> is less than 0 or greater than <see cref="MaxMessageNumber"/>.</exception>
		/// <exception cref="System::ObjectDisposedException">If the object has been disposed.</exception>
		/// <exception cref="System::InvalidOperationException">If attempting to set the value and <see cref="IsEditable"/> is false.</exception>
		property int MessageNumber {
			int get(void);
			void set(int value);
		}

		/// <summary>
		/// Get or set the packet sequence number.
		/// Default value is 0.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">If <paramref name="value"/> is less than 0.</exception>
		/// <exception cref="System::ObjectDisposedException">If the object has been disposed.</exception>
		/// <exception cref="System::InvalidOperationException">If attempting to set the value and <see cref="IsEditable"/> is false.</exception>
		property int PacketNumber {
			int get(void);
			void set(int value);
		}

		/// <summary>
		/// Get or set the length of the packet payload, in bytes.
		/// Default value is 0.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">If <paramref name="value"/> is less than 0.</exception>
		/// <exception cref="System::ObjectDisposedException">If the object has been disposed.</exception>
		/// <exception cref="System::InvalidOperationException">If attempting to set the value and <see cref="IsEditable"/> is false.</exception>
		property int DataLength {
			int get(void);
			void set(int value);
		}

		/// <summary>
		/// Get the space allocated to the packet payload. This may be greater
		/// than or equal to <see cref="DataLength"/>.
		/// </summary>
		/// <exception cref="System::ObjectDisposedException">If the object has been disposed.</exception>
		property int DataCapacity {
			int get(void);
		}

		/// <summary>
		/// Read from the packet payload data.
		/// </summary>
		/// <param name="dataOffset">Offset into the packet data to start reading at.</param>
		/// <param name="buffer">Buffer to read packet data into.</param>
		/// <param name="bufferOffset">Offset into the <paramref name="buffer"/> to copy data to.</param>
		/// <param name="bufferCount">Maximum number of bytes to copy from the packet.</param>
		/// <returns>Number of bytes read into <paramref name="buffer"/>.</returns>
		/// <exception cref="System::ArgumentNullException">If <paramref name="buffer"/> is null.</exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// If <paramref name="dataOffset"/> is less than 0 or greater than <see cref="DataLength"/>.
		/// - or -
		/// If <paramref name="bufferOffset"/> or <paramref name="bufferCount"/> is less than 0
		/// </exception>
		/// <exception cref="System::ArgumentException">If the sum of <paramref name="bufferOffset"/> and <paramref name="bufferCount"/> is larger than the <paramref name="buffer"/> length.</exception>
		/// <exception cref="System::ObjectDisposedException">If the object has been disposed.</exception>
		int Read(int dataOffset, cli::array<unsigned char>^ buffer, int bufferOffset, int bufferCount);

		/// <summary>
		/// Write to the packet payload data.
		/// </summary>
		/// <remarks>
		/// If the sum of <paramref name="dataOffset"/> and <paramref name="bufferCount"/>
		/// is greater than <see cref="DataLength"/>, the packet's payload space
		/// is expanded as needed.
		/// </remarks>
		/// <param name="dataOffset">Offset into the packet data to start writing to.</param>
		/// <param name="buffer">Buffer to copy packet data from.</param>
		/// <param name="bufferOffset">Offset into the <paramref name="buffer"/> to start reading.</param>
		/// <param name="bufferCount">Maximum number of bytes to read from <paramref name="buffer"/>.</param>
		/// <exception cref="System::ArgumentNullException">If <paramref name="buffer"/> is null.</exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// If <paramref name="dataOffset"/> is less than 0.
		/// - or -
		/// If <paramref name="bufferOffset"/> or <paramref name="bufferCount"/> is less than 0
		/// </exception>
		/// <exception cref="System::ArgumentException">If the sum of <paramref name="bufferOffset"/> and <paramref name="bufferCount"/> is larger than the <paramref name="buffer"/> length.</exception>
		/// <exception cref="System::ObjectDisposedException">If the object has been disposed.</exception>
		/// <exception cref="System::InvalidOperationException">If attempting to set the value and <see cref="IsEditable"/> is false.</exception>
		void Write(int dataOffset, cli::array<unsigned char>^ buffer, int bufferOffset, int bufferCount);

		/// <summary>
		/// Maximum allowed value for <see cref="MessageNumber"/>.
		/// </summary>
		/// <value>536,870,911 (0x1FFFFFFF)</value>
		literal int MaxMessageNumber = 0x1FFFFFFF;
	};
}
