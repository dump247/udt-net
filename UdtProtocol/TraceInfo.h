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
#include "TotalTraceInfo.h"
#include "LocalTraceInfo.h"
#include "ProbeTraceInfo.h"

namespace Udt
{
	/// <summary>
	/// UDT socket performance trace information.
	/// </summary>
	public ref class TraceInfo
	{
	private:
		Udt::TotalTraceInfo^ _total;
		Udt::LocalTraceInfo^ _local;
		Udt::ProbeTraceInfo^ _probe;

	internal:
		TraceInfo(const UDT::TRACEINFO& copy);

	public:
		/// <summary>
		/// Initialize a new instance with default values.
		/// </summary>
		TraceInfo(void);

		/// <summary>
		/// Aggregate values since the UDT socket is created.
		/// </summary>
		property Udt::TotalTraceInfo^ Total { Udt::TotalTraceInfo^ get(void) { return _total; } }

		/// <summary>
		/// Local values since the last time they are recorded.
		/// </summary>
		/// <remarks>
		/// The local attributes are reset when <c>true</c> is passed to
		/// <b>Udt.Socket.GetPerformanceInfo(bool)</b>.
		/// </remarks>
		property Udt::LocalTraceInfo^ Local { Udt::LocalTraceInfo^ get(void) { return _local; } }

		/// <summary>
		/// Instant values at the time they are observed.
		/// </summary>
		property Udt::ProbeTraceInfo^ Probe { Udt::ProbeTraceInfo^ get(void) { return _probe; } }
	};
}
