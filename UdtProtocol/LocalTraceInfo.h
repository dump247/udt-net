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
	/// <summary>
	/// Performance trace information local since the last time they were recorded.
	/// </summary>
	public ref class LocalTraceInfo
	{
	internal:
		LocalTraceInfo(const UDT::TRACEINFO& copy);

	public:
		/// <summary>
		/// Initialize a new instance with default values.
		/// </summary>
		LocalTraceInfo(void);

		/// <summary>
		/// Number of sent packets, including retransmissions.
		/// </summary>
		property __int64 PacketsSent;

		/// <summary>
		/// Number of received packets.
		/// </summary>
		property __int64 PacketsReceived;

		/// <summary>
		/// Number of lost packets, measured in the sending side.
		/// </summary>
		property int SendPacketsLost;

		/// <summary>
		/// Number of lost packets, measured in the receiving side.
		/// </summary>
		property int ReceivePacketsLost;

		/// <summary>
		/// Number of retransmitted packets, measured in the sending side.
		/// </summary>
		property int PacketsRetransmitted;

		/// <summary>
		/// Number of sent ACK packets.
		/// </summary>
		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1704:IdentifiersShouldBeSpelledCorrectly",
			Justification = "ACK is the accepted abbreviation for acknowledgement in this context.")]
		property int AcksSent;

		/// <summary>
		/// Number of received ACK packets.
		/// </summary>
		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1704:IdentifiersShouldBeSpelledCorrectly",
			Justification = "ACK is the accepted abbreviation for acknowledgement in this context.")]
		property int AcksReceived;

		/// <summary>
		/// Number of sent NAK packets.
		/// </summary>
		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1704:IdentifiersShouldBeSpelledCorrectly",
			Justification = "NAK is the accepted abbreviation for negative acknowledgement in this context.")]
		property int NaksSent;

		/// <summary>
		/// Number of received NAK packets.
		/// </summary>
		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1704:IdentifiersShouldBeSpelledCorrectly",
			Justification = "NAK is the accepted abbreviation for negative acknowledgement in this context.")]
		property int NaksReceived;

		/// <summary>
		/// Sending rate in Mbps.
		/// </summary>
		property double SendMbps;

		/// <summary>
		/// Receiving rate in Mbps.
		/// </summary>
		property double ReceiveMbps;

		/// <summary>
		/// Busy sending time (i.e., idle time exclusive).
		/// </summary>
		property System::TimeSpan SendDuration;
	};
}
