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
#include "SocketException.h"

#include <udt.h>

using namespace System;
using namespace Udt;

SocketException::SocketException(System::Runtime::Serialization::SerializationInfo^ info, System::Runtime::Serialization::StreamingContext context)
	: System::Exception(info, context)
{
	_socketErrorCode = (Udt::SocketError)info->GetInt32("ErrorCode");
}

SocketException::SocketException(void)
{
	_socketErrorCode = Udt::SocketError::Error;
}

SocketException::SocketException(System::String^ message)
	: System::Exception(message)
{
	_socketErrorCode = Udt::SocketError::Error;
}

SocketException::SocketException(System::String^ message, System::Exception^ inner)
	: System::Exception(message, inner)
{
	_socketErrorCode = Udt::SocketError::Error;
}

SocketException::SocketException(System::String^ message, Udt::SocketError errorCode)
	: System::Exception(message)
{
	_socketErrorCode = errorCode;
}

SocketException^ SocketException::GetLastError(String^ message)
{
	UDT::ERRORINFO& lastError = UDT::getlasterror();
	int errorCode = lastError.getErrorCode();
	String^ udtMessage = (gcnew String(lastError.getErrorMessage()))->TrimEnd();
	String^ exMessage;

	if (String::IsNullOrEmpty(message))
		exMessage = String::Concat(udtMessage, Environment::NewLine, "UDT Error Code: ", (Object^)errorCode);
	else
		exMessage = String::Concat(message, Environment::NewLine, udtMessage, Environment::NewLine, "UDT Error Code: ", (Object^)errorCode);

	return gcnew SocketException(exMessage, (Udt::SocketError)errorCode);
}
