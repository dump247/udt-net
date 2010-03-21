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

#include "TraceInfo.h"

namespace Udt
{
	ref class Packet;
	class CCCWrapper;

	public ref class CongestionControl
	{
	internal:
		CCCWrapper* _cccWrapper;

	public:
		CongestionControl(void);

		virtual void Initialize() { }
		virtual void Close() { }

		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1704:IdentifiersShouldBeSpelledCorrectly",
			Justification = "ACK is the accepted abbreviation for acknowledgement in this context.")]
		virtual void OnAck(int ack) { }
		virtual void OnLoss(System::Collections::Generic::IList<int>^ lossList) { }
		virtual void OnTimeout() { }
		virtual void OnPacketSent(Packet^ packet) { }
		virtual void OnPacketReceived(Packet^ packet) { }
		virtual void ProcessCustomMessage(Packet^ packet) { }

	protected:

		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1704:IdentifiersShouldBeSpelledCorrectly",
			Justification = "ACK is the accepted abbreviation for acknowledgement in this context.")]
		void SetAckTimer(System::TimeSpan value);

		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Naming",
			"CA1704:IdentifiersShouldBeSpelledCorrectly",
			Justification = "ACK is the accepted abbreviation for acknowledgement in this context.")]
		void SetAckInterval(int value);

		void SetReadTimeout(System::TimeSpan value);

		property TraceInfo^ PerformanceInfo { TraceInfo^ get(void); }

		property System::TimeSpan PacketSendPeriod
		{
			System::TimeSpan get(void);
			void set(System::TimeSpan value);
		}

		property int WindowSize
		{
			int get(void);
			void set(int value);
		}

		property int MaxPacketSize
		{
			int get(void);
			void set(int value);
		}

		property System::TimeSpan RoundtripTime
		{
			System::TimeSpan get(void);
			void set(System::TimeSpan value);
		}
	};
}
