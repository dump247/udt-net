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

#include "Packet.h"
#include "DataPacket.h"
#include "ControlPacket.h"
#include "KeepAlivePacket.h"
#include "ShutdownPacket.h"
#include "CongestionPacket.h"

#include <udt.h>
#include <packet.h>

using namespace Udt;
using namespace System;

Packet^ Packet::Wrap(const CPacket* packet)
{
	Packet^ wrapper;

	if (packet->getFlag())
	{
		switch (packet->getType())
		{
		case CongestionPacket::TypeCode:
			wrapper = gcnew CongestionPacket((CPacket*)packet, false);
			break;

		case ShutdownPacket::TypeCode:
			wrapper = gcnew ShutdownPacket((CPacket*)packet, false);
			break;

		case KeepAlivePacket::TypeCode:
			wrapper = gcnew KeepAlivePacket((CPacket*)packet, false);
			break;

		default:
			wrapper = gcnew ControlPacket((CPacket*)packet, false);
			break;
		}
	}
	else
	{
		wrapper = gcnew DataPacket((CPacket*)packet, false);
	}

	return wrapper;
}

Packet::Packet(CPacket* packet, bool deletePacket)
{
	_packet = packet;
	_deletePacket = deletePacket;
}

Packet::~Packet()
{
	if (_deletePacket)
	{
		FreePacketData();
		delete _packet;
	}

	_packet = NULL;
}

void Packet::AssertNotDisposed()
{
	if (_packet == NULL)
		throw gcnew ObjectDisposedException(this->ToString());
}

void Packet::AssertIsMutable()
{
	AssertNotDisposed();
	if (!IsEditable) throw gcnew InvalidOperationException("Packet can not be modified.");
}

TimeSpan Packet::TimeStamp::get(void)
{
	AssertNotDisposed();
	return FromMicroseconds((uint32_t)_packet->m_iTimeStamp);
}

void Packet::TimeStamp::set(TimeSpan value)
{
	AssertIsMutable();

	if (value < TimeSpan::Zero || value > MaxTimeStamp)
		throw gcnew ArgumentOutOfRangeException("value", value, String::Concat("Value must be between ", TimeSpan::Zero, " and ", MaxTimeStamp, "."));

	_packet->m_iTimeStamp = (int32_t)ToMicroseconds(value);
}

int Packet::DestinationId::get(void)
{
	AssertNotDisposed();
	return _packet->m_iID;
}

void Packet::DestinationId::set(int value)
{
	AssertIsMutable();
	_packet->m_iID = value;
}
