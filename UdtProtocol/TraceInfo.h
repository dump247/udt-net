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
	public ref class TraceInfo
	{
	internal:
		TraceInfo(const UDT::TRACEINFO& copy);

	public:
		TraceInfo(void);

		property System::TimeSpan SocketCreated;
		property __int64 TotalPacketsSent;
		property __int64 TotalPacketsReceived;
		property int TotalSendPacketsLost;
		property int TotalReceivePacketsLost;
		property int TotalPacketsRetransmitted;

		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1704:IdentifiersShouldBeSpelledCorrectly",
			Justification = "ACK is the accepted abbreviation for acknowledgement in this context.")]
		property int TotalAcksSent;

		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1704:IdentifiersShouldBeSpelledCorrectly",
			Justification = "ACK is the accepted abbreviation for acknowledgement in this context.")]
		property int TotalAcksReceived;

		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1704:IdentifiersShouldBeSpelledCorrectly",
			Justification = "NAK is the accepted abbreviation for negative acknowledgement in this context.")]
		property int TotalNaksSent;

		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1704:IdentifiersShouldBeSpelledCorrectly",
			Justification = "NAK is the accepted abbreviation for negative acknowledgement in this context.")]
		property int TotalNaksReceived;

		property System::TimeSpan TotalSendDuration;

		property __int64 PacketsSent;
		property __int64 PacketsReceived;
		property int SendPacketsLost;
		property int ReceivePacketsLost;
		property int PacketsRetransmitted;

		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1704:IdentifiersShouldBeSpelledCorrectly",
			Justification = "ACK is the accepted abbreviation for acknowledgement in this context.")]
		property int AcksSent;

		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1704:IdentifiersShouldBeSpelledCorrectly",
			Justification = "ACK is the accepted abbreviation for acknowledgement in this context.")]
		property int AcksReceived;

		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1704:IdentifiersShouldBeSpelledCorrectly",
			Justification = "NAK is the accepted abbreviation for negative acknowledgement in this context.")]
		property int NaksSent;

		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1704:IdentifiersShouldBeSpelledCorrectly",
			Justification = "NAK is the accepted abbreviation for negative acknowledgement in this context.")]
		property int NaksReceived;
		property double SendMbps;
		property double ReceiveMbps;
		property System::TimeSpan SendDuration;

		property System::TimeSpan PacketSendPeriod;
		property int FlowWindow;
		property int CongestionWindow;
		property int FlightSize;
		property System::TimeSpan RoundtripTime;
		property double BandwidthMbps;
		property int AvailableSendBuffer;
		property int AvailableReceiveBuffer;
	};
}
