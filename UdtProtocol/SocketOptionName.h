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
	/// Socket configuration option names.
	/// </summary>
	[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Design",
			"CA1027:MarkEnumsWithFlags",
			Justification = "This is a set of discrete values, not a set of flags.")]
	public enum class SocketOptionName
	{
		/// <summary>
		/// The maximum transfer unit.
		/// </summary>
		MaxPacketSize = UDT_MSS,

		/// <summary>
		/// If sending is blocking.
		/// </summary>
		BlockingSend = UDT_SNDSYN,

		/// <summary>
		/// If receiving is blocking.
		/// </summary>
		BlockingReceive = UDT_RCVSYN,

		/// <summary>
		/// Flight flag size (window size).
		/// </summary>
		MaxWindowSize = UDT_FC,

		/// <summary>
		/// Maximum buffer in sending queue.
		/// </summary>
		SendBuffer = UDT_SNDBUF,

		/// <summary>
		/// UDT receiving buffer size.
		/// </summary>
		ReceiveBuffer = UDT_RCVBUF,

		/// <summary>
		/// UDP sending buffer size.
		/// </summary>
		UdpSendBuffer = UDP_SNDBUF,

		/// <summary>
		/// UDP receiving buffer size.
		/// </summary>
		UdpReceiveBuffer = UDP_RCVBUF,

		/// <summary>
		/// Rendezvous connection mode.
		/// </summary>
		Rendezvous = UDT_RENDEZVOUS,

		/// <summary>
		/// Send timeout.
		/// </summary>
		SendTimeout = UDT_SNDTIMEO,

		/// <summary>
		/// Receive timeout.
		/// </summary>
		ReceiveTimeout = UDT_RCVTIMEO,

		/// <summary>
		/// Reuse an existing port or create a new one.
		/// </summary>
		ReuseAddress = UDT_REUSEADDR,

		/// <summary>
		/// Maximum bandwidth (bytes per second) that the connection can use.
		/// </summary>
		MaxBandwidth = UDT_MAXBW,

		/// <summary>
		/// Waiting for unsent data when closing.
		/// </summary>
		Linger = UDT_LINGER,

		/// <summary>
		/// Custom congestion control algorithm.
		/// </summary>
		CongestionControl = UDT_CC,
	};
}
