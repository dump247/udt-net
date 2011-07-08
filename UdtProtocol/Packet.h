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

class CPacket;

namespace Udt
{
	/// <summary>
	/// UDT data packet.
	/// </summary>
	public ref class Packet abstract
	{
	private:
		bool _deletePacket;

	protected:
		
		CPacket* _packet;
		void AssertNotDisposed();
		void AssertIsMutable();

	internal:

		static Packet^ Wrap(const CPacket* packet);

		/// <summary>
		/// Initialize a new instance.
		/// </summary>
		/// <param name="packet">Native packet to wrap.</param>
		/// <param name="deletePacket">
		/// True to delete the native <paramref name="packet"/> when this object
		/// is disposed.
		/// </param>
		Packet(CPacket* packet, bool deletePacket);

	public:
		virtual ~Packet(void);

		/// <summary>
		/// Get true if the packet can be modified.
		/// </summary>
		property bool IsEditable {
			bool get(void) { return _deletePacket; }
		}

		/// <summary>
		/// Get or set the time stamp associated with the packet.
		/// Default value is <see cref="System::TimeSpan::Zero"/>.
		/// </summary>
		/// <remarks>
		/// <para>
		/// The time stamp is generally the difference between when the
		/// packet was created and when the socket it was sent on was
		/// created.
		/// </para>
		/// <para>
		/// The resolution of this property is 1 microsecond
		/// (1000 nanoseconds). The resolution of <see cref="System::TimeSpan"/>
		/// is 100 nanoseconds. When setting the property, the value will
		/// be rounded down to the nearest microsecond.
		/// </para>
		/// </remarks>
		/// <exception cref="System::ArgumentOutOfRangeException">If <paramref name="value"/> is less than <see cref="System::TimeSpan::Zero"/> or greater than <see cref="MaxTimeStamp"/>.</exception>
		/// <exception cref="System::ObjectDisposedException">If the object has been disposed.</exception>
		/// <exception cref="System::InvalidOperationException">If attempting to set the value and <see cref="IsEditable"/> is false.</exception>
		property System::TimeSpan TimeStamp {
			System::TimeSpan get(void);
			void set(System::TimeSpan value);
		}

		/// <summary>
		/// Get or set ID of the destination socket for the packet.
		/// Default value is 0.
		/// </summary>
		/// <exception cref="System::ObjectDisposedException">If the object has been disposed.</exception>
		/// <exception cref="System::InvalidOperationException">If attempting to set the value and <see cref="IsEditable"/> is false.</exception>
		property int DestinationId {
			int get(void);
			void set(int value);
		}

		/// <summary>
		/// Maximum allowed value for <see cref="TimeStamp"/>.
		/// </summary>
		/// <value>4,294,967,295 microseconds (01:11:34.9672950).</value>
		static initonly System::TimeSpan MaxTimeStamp = FromMicroseconds(System::UInt32::MaxValue);
	};
}
