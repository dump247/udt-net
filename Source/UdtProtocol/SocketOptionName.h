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
	[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Design",
			"CA1027:MarkEnumsWithFlags",
			Justification = "This is a set of discrete values, not a set of flags.")]
	public enum class SocketOptionName
	{
		MaxPacketSize = UDT_MSS,
		BlockingSend = UDT_SNDSYN,
		BlockingReceive = UDT_RCVSYN,
		MaxWindowSize = UDT_FC,
		SendBuffer = UDT_SNDBUF,
		ReceiveBuffer = UDT_RCVBUF,

		UdpSendBuffer = UDP_SNDBUF,
		UdpReceiveBuffer = UDP_RCVBUF,

		Rendezvous = UDT_RENDEZVOUS,

		SendTimeout = UDT_SNDTIMEO,
		ReceiveTimeout = UDT_RCVTIMEO,

		ReuseAddress = UDT_REUSEADDR,

		MaxBandwidth = UDT_MAXBW,

		Linger = UDT_LINGER,

		CongestionControl = UDT_CC,
	};
}
