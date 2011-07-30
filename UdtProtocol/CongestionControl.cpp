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
#include "CongestionControl.h"
#include "Packet.h"

#include "CCCWrapper.h"

using namespace Udt;
using namespace System;

CongestionControl::CongestionControl(void)
	: _cccWrapper(NULL), _isDisposed(false)
{
}

void CongestionControl::AssertNotDisposed()
{
	if (this->IsDisposed) throw gcnew ObjectDisposedException(this->ToString());
}

void CongestionControl::SetAckTimer(System::TimeSpan value)
{
	AssertNotDisposed();
	_cccWrapper->setACKTimer(value);
}

void CongestionControl::SetAckInterval(int value)
{
	AssertNotDisposed();
	_cccWrapper->setACKInterval(value);
}

void CongestionControl::SetReadTimeout(System::TimeSpan value)
{
	AssertNotDisposed();
	_cccWrapper->setRTO(value);
}

TraceInfo^ CongestionControl::PerformanceInfo::get(void)
{
	AssertNotDisposed();
	return _cccWrapper->getPerfInfo();
}

void CongestionControl::PacketSendPeriod::set(System::TimeSpan value)
{
	AssertNotDisposed();
	_cccWrapper->setPacketSendPeriod(value);
}

System::TimeSpan CongestionControl::PacketSendPeriod::get(void)
{
	AssertNotDisposed();
	return _cccWrapper->getPacketSendPeriod();
}

void CongestionControl::WindowSize::set(int value)
{
	AssertNotDisposed();
	_cccWrapper->setWindowSize(value);
}

int CongestionControl::WindowSize::get(void)
{
	AssertNotDisposed();
	return _cccWrapper->getWindowSize();
}

void CongestionControl::MaxPacketSize::set(int value)
{
	AssertNotDisposed();
	_cccWrapper->setMaxPacketSize(value);
}

int CongestionControl::MaxPacketSize::get(void)
{
	AssertNotDisposed();
	return _cccWrapper->getMaxPacketSize();
}

void CongestionControl::RoundtripTime::set(System::TimeSpan value)
{
	AssertNotDisposed();
	_cccWrapper->setRoundTripTime(value);
}

System::TimeSpan CongestionControl::RoundtripTime::get(void)
{
	AssertNotDisposed();
	return _cccWrapper->getRoundTripTime();
}
