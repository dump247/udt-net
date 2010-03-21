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
#include "CCCWrapper.h"
#include "Packet.h"

using namespace Udt;
using namespace System;

CCCWrapper::CCCWrapper(Udt::CongestionControl^ wrapped)
{
	_wrapped = wrapped;
	_wrapped->_cccWrapper = this;
}

CCCWrapper::~CCCWrapper(void)
{
	_wrapped->_cccWrapper = NULL;
	delete _wrapped;
	_wrapped = nullptr;
}

void CCCWrapper::onPktReceived(const CPacket* packet)
{
	Packet^ managedPacket = gcnew Packet(packet);

	__try
	{
		_wrapped->OnPacketReceived(managedPacket);
	}
	__finally
	{
		delete managedPacket;
	}
}

void CCCWrapper::onPktSent(const CPacket* packet)
{
	Packet^ managedPacket = gcnew Packet(packet);

	__try
	{
		_wrapped->OnPacketSent(managedPacket);
	}
	__finally
	{
		delete managedPacket;
	}
}

void CCCWrapper::processCustomMsg(const CPacket* packet)
{
	Packet^ managedPacket = gcnew Packet(packet);

	__try
	{
		_wrapped->ProcessCustomMessage(managedPacket);
	}
	__finally
	{
		delete managedPacket;
	}
}

void CCCWrapper::onLoss(const int* losslist, const int& size)
{
	NativeIntArray^ list = gcnew NativeIntArray(losslist, size);

	__try
	{
		_wrapped->OnLoss(list);
	}
	__finally
	{
		delete list;
	}
}

void CCCWrapper::setACKTimer(TimeSpan value)
{
	if (value.CompareTo(TimeSpan::Zero) < 0)
		throw gcnew System::ArgumentOutOfRangeException("value", value, "Value must be greater than or equal to 0.");

	int ms = (int)value.TotalMilliseconds;
	this->CCC::setACKTimer(ms);
}

void CCCWrapper::setACKInterval(int packets)
{
	if (packets < 0)
		throw gcnew System::ArgumentOutOfRangeException("packets", packets, "Value must be greater than or equal to 0.");

	this->CCC::setACKInterval(packets);
}

void CCCWrapper::setRTO(TimeSpan value)
{
	if (value.CompareTo(TimeSpan::Zero) < 0)
		throw gcnew System::ArgumentOutOfRangeException("value", value, "Value must be greater than or equal to 0.");

	int us = (int)(value.Ticks / 10);
	this->CCC::setRTO(us);
}

TraceInfo^ CCCWrapper::getPerfInfo(void)
{
	const UDT::TRACEINFO* trace_info = this->CCC::getPerfInfo();
	return gcnew TraceInfo(*trace_info);
}

void CCCWrapper::setPacketSendPeriod(System::TimeSpan value)
{
	if (value.CompareTo(TimeSpan::Zero) < 0)
		throw gcnew System::ArgumentOutOfRangeException("value", value, "Value must be greater than or equal to 0.");

	m_dPktSndPeriod = (double)(value.Ticks / 10);
}

System::TimeSpan CCCWrapper::getPacketSendPeriod(void) const
{
	return System::TimeSpan((__int64)(m_dPktSndPeriod * 10));
}

void CCCWrapper::setWindowSize(int value)
{
	if (value < 0)
		throw gcnew System::ArgumentOutOfRangeException("value", value, "Value must be greater than or equal to 0.");

	m_dCWndSize = value;
}

int CCCWrapper::getWindowSize() const
{
	return (int)m_dCWndSize;
}

void CCCWrapper::setRoundTripTime(System::TimeSpan value)
{
	if (value.CompareTo(TimeSpan::Zero) < 0)
		throw gcnew System::ArgumentOutOfRangeException("value", value, "Value must be greater than or equal to 0.");

	m_iRTT = (int)(value.Ticks / 10);
}

System::TimeSpan CCCWrapper::getRoundTripTime(void) const
{
	return System::TimeSpan(m_iRTT * 10);
}

void CCCWrapper::setMaxPacketSize(int value)
{
	if (value < 0)
		throw gcnew System::ArgumentOutOfRangeException("value", value, "Value must be greater than or equal to 0.");

	m_iMSS = value;
}

int CCCWrapper::getMaxPacketSize() const
{
	return m_iMSS;
}
