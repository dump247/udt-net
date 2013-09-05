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
	/// States the UDT socket can be in.
	/// </summary>
	public enum class SocketState
	{
		/// <summary>
		/// Invalid socket state. A socket will never be in this state.
		/// </summary>
		Invalid = 0,

		/// <summary>
		/// Socket is in initialized state.
		/// </summary>
		Initial = INIT,

		/// <summary>
		/// Socket is open.
		/// </summary>
		Open = OPENED,
		
		/// <summary>
		/// Socket is listening.
		/// </summary>
		Listening = LISTENING,
		
		/// <summary>
		/// Socket is establishing the connection.
		/// </summary>
		Connecting = CONNECTING,
		
		/// <summary>
		/// Socket is connected.
		/// </summary>
		Connected = CONNECTED,
		
		/// <summary>
		/// Socket is broken.
		/// </summary>
		Broken = BROKEN,
		
		/// <summary>
		/// Socket is closing.
		/// </summary>
		Closing = CLOSING,
		
		/// <summary>
		/// Socket is closed.
		/// </summary>
		Closed = CLOSED,
	};
}