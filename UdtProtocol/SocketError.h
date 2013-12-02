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
	/// UDT socket error codes.
	/// </summary>
	[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Design",
			"CA1027:MarkEnumsWithFlags",
			Justification = "This is a set of discrete values, not a set of flags.")]
	public enum class SocketError
	{
		/// <summary>
		/// Unspecified socket error.
		/// </summary>
		/// <value>-1</value>
		Error = -1,

		/// <summary>
		/// The operation succeeded.
		/// </summary>
		/// <value>0</value>
		Success = 0,

		/// <summary>
		/// Connection setup failure.
		/// </summary>
		/// <value>1000</value>
		ConnectionSetup = 1000,

		/// <summary>
		/// Server does not exist.
		/// </summary>
		/// <value>1001</value>
		NoServer = 1001,

		/// <summary>
		/// Connection request was rejected by server.
		/// </summary>
		/// <value>1002</value>
		ConnectionRejected = 1002,

		/// <summary>
		/// Could not create/configure UDP socket.
		/// </summary>
		/// <value>1003</value>
		SocketFail = 1003,

		/// <summary>
		/// Connection request was aborted due to security reasons.
		/// </summary>
		/// <value>1004</value>
		SecurityFail = 1004,

		/// <summary>
		/// Connection failure.
		/// </summary>
		/// <value>2000</value>
		ConnectionFail = 2000,

		/// <summary>
		/// Connection was broken.
		/// </summary>
		/// <value>2001</value>
		ConnectionLost = 2001,

		/// <summary>
		/// Connection does not exist.
		/// </summary>
		/// <value>2002</value>
		NoConnection = 2002,

		/// <summary>
		/// System resource failure.
		/// </summary>
		/// <value>3000</value>
		Resource = 3000,

		/// <summary>
		/// Could not create new thread.
		/// </summary>
		/// <value>3001</value>
		Thread = 3001,

		/// <summary>
		/// No memory space.
		/// </summary>
		/// <value>3002</value>
		NoBuffer = 3002,

		/// <summary>
		/// File access error.
		/// </summary>
		/// <value>4000</value>
		File = 4000,

		/// <summary>
		/// Invalid read offset.
		/// </summary>
		/// <value>4001</value>
		InvalidReadOffset = 4001,

		/// <summary>
		/// No read permission.
		/// </summary>
		/// <value>4002</value>
		ReadPermission = 4002,

		/// <summary>
		/// Invalid write offset.
		/// </summary>
		/// <value>4003</value>
		InvalidWriteOffset = 4003,

		/// <summary>
		/// No write permission.
		/// </summary>
		/// <value>4004</value>
		WritePermission = 4004,

		/// <summary>
		/// Invalid operation.
		/// </summary>
		/// <value>5000</value>
		InvalidOperation = 5000,

		/// <summary>
		/// Cannot execute the operation on a bound socket.
		/// </summary>
		/// <value>5001</value>
		BoundSocket = 5001,

		/// <summary>
		/// Cannot execute the operation on a connected socket.
		/// </summary>
		/// <value>5002</value>
		ConnectedSocket = 5002,

		/// <summary>
		/// Bad parameters.
		/// </summary>
		/// <value>5003</value>
		InvalidParameter = 5003,

		/// <summary>
		/// Invalid UDT socket.
		/// </summary>
		/// <value>5004</value>
		InvalidSocket = 5004,

		/// <summary>
		/// Cannot listen on unbound socket.
		/// </summary>
		/// <value>5005</value>
		UnboundSocket = 5005,

		/// <summary>
		///	Socket is not in listening state (accept).
		/// </summary>
		/// <value>5006</value>
		NotListening = 5006,

		/// <summary>
		///	Rendezvous connection process does not allow listen and accept call.
		/// </summary>
		/// <value>5007</value>
		RendezvousNoServer = 5007,

		/// <summary>
		///	Rendezvous connection setup is enabled, but bind has not been called
		/// before connect.
		/// </summary>
		/// <value>5008</value>
		RendezvousUnbound = 5008,

		/// <summary>
		///	Operation not supported in SOCK_STREAM mode.
		/// </summary>
		/// <value>5009</value>
		OperationNotSupportedForStream = 5009,

		/// <summary>
		///	Operation not supported in SOCK_DGRAM mode.
		/// </summary>
		/// <value>5010</value>
		OperationNotSupportedForDGram = 5010,

		/// <summary>
		///	Another socket is already listening on the same UDP port.
		/// </summary>
		/// <value>5011</value>
		SocketInUse = 5011,

		/// <summary>
		///	Message is too large to be hold in the sending buffer.
		/// </summary>
		/// <value>5012</value>
		MessageTooBig = 5012,

		/// <summary>
		///	Non-blocking call failure.
		/// </summary>
		/// <value>6000</value>
		NonBlockCallFail = 6000,

		/// <summary>
		///	No buffer available for sending.
		/// </summary>
		/// <value>6001</value>
		NoSendBuffer = 6001,

		/// <summary>
		///	No data available for read.
		/// </summary>
		/// <value>6002</value>
		NoDataAvailable = 6002,
	};
}
