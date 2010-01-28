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

#include "CongestionControl.h"
#include "NativeIntArray.h"
#include "TraceInfo.h"
#include <ccc.h>
#include <vcclr.h>

namespace Udt
{
	class CCCWrapper : public CCC
	{
	private:
		gcroot<CongestionControl^> _wrapped;

	public:

		CCCWrapper(CongestionControl^ wrapped);
		virtual ~CCCWrapper(void);

		virtual void init() { _wrapped->Initialize(); }
		virtual void close() { _wrapped->Close(); }
		virtual void onTimeout() { _wrapped->OnTimeout(); }
		virtual void onACK(const int& ack) { _wrapped->OnAck(ack); }
		virtual void onLoss(const int* losslist, const int& size);

		void setACKTimer(System::TimeSpan^ value);
		void setACKInterval(int packets);
		void setRTO(System::TimeSpan^ value);
		TraceInfo^ getPerfInfo(void);

		void setPacketSendPeriod(System::TimeSpan^ value);
		System::TimeSpan^ getPacketSendPeriod(void) const;

		void setWindowSize(int value);
		int getWindowSize() const;

		void setRoundTripTime(System::TimeSpan^ value);
		System::TimeSpan^ getRoundTripTime(void) const;

		void setMaxPacketSize(int value);
		int getMaxPacketSize() const;
	};
}
