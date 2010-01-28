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

namespace Udt
{
	public ref class Message
	{
	public:
		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Security",
			"CA2104:DoNotDeclareReadOnlyMutableReferenceTypes",
			Justification = "System.TimeSpan is immutable.")]
		static initonly System::TimeSpan^ Infinite = gcnew System::TimeSpan(0, 0, 0, 0, -1);

		[System::Diagnostics::CodeAnalysis::SuppressMessageAttribute(
			"Microsoft.Security",
			"CA2104:DoNotDeclareReadOnlyMutableReferenceTypes",
			Justification = "System.TimeSpan is immutable.")]
		static initonly System::TimeSpan^ Max = gcnew  System::TimeSpan(0, 0, 0, 0, System::Int32::MaxValue);

	private:
		System::ArraySegment<System::Byte> _buffer;
		System::TimeSpan^ _timeToLive;

	public:
		Message(cli::array<System::Byte>^ buffer);
		Message(cli::array<System::Byte>^ buffer, int offset, int count);
		Message(System::ArraySegment<System::Byte> buffer);

		property System::ArraySegment<System::Byte> Buffer
		{
			System::ArraySegment<System::Byte> get(void) { return _buffer; }
		}

		property bool InOrder;

		property System::TimeSpan^ TimeToLive
		{
			System::TimeSpan^ get(void) { return _timeToLive; }

			void set(System::TimeSpan^ value)
			{
				if (value == nullptr)
					throw gcnew System::ArgumentNullException("value");

				if (value != Message::Infinite && (value->CompareTo(System::TimeSpan::Zero) < 0 || value->CompareTo(Message::Max) > 0))
					throw gcnew System::ArgumentOutOfRangeException("value", value, System::String::Concat("Value must be ", Infinite, " or between ", System::TimeSpan::Zero, " and ", Max, "."));

				_timeToLive = value;
			}
		}
	};
}
