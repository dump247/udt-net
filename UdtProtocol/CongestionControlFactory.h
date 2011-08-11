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

#include "ICongestionControlFactory.h"

namespace Udt
{
	/// <summary>
	/// Factory that creates <see cref="CongestionControl"/> instances for a
	/// <see cref="Udt::Socket"/>.
	/// </summary>
	/// <remarks>
	/// <see cref="CreateCongestionControl"/> must return a new instance every
	/// time it is invoked. If the same instance returns the same instance more
	/// than once, the result is undefined behavior.
	/// </remarks>
	public ref class CongestionControlFactory : public ICongestionControlFactory
	{
	private:

		initonly System::Func<CongestionControl^>^ _callback;
		
	public:

		/// <summary>
		/// Initialize a new instance that creates instances of <typeparamref name="CongestionControl"/>
		/// using the provided callback.
		/// </summary>
		/// <param name="callback">Function to invoke to create new instances of <typeparamref name="T"/></param>
		/// <exception name="System::ArgumentNullException">If <paramref name="callback"/> is null.</exception>
		CongestionControlFactory(System::Func<CongestionControl^>^ callback)
			: _callback(callback)
		{
			if (callback == nullptr) throw gcnew System::ArgumentNullException("callback");
		}

		/// <summary>
		/// Create a new <typeparamref name="T"/> instance.
		/// </summary>
		/// <remarks>
		/// This method must return a new instance every time it is invoked.
		/// If the same instance returns the same instance more than once,
		/// the result is undefined behavior.
		/// </remarks>
		/// <returns>New congestion control object.</returns>
		virtual CongestionControl^ CreateCongestionControl(void)
		{
			return _callback();
		}
	};
}
