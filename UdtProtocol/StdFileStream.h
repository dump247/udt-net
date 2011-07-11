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

#include <iostream>
#include <fstream>

namespace Udt
{
	/// <summary>
	/// Interface to a UDT socket.
	/// </summary>
	public ref class StdFileStream : public System::IO::Stream
	{
	private:

		bool _canRead;
		bool _canWrite;
		bool _canSeek;
		std::fstream* _stream;

        void Init(System::String^ path, System::IO::FileMode mode, System::IO::FileAccess access);
		void Init(const char* path, System::IO::FileMode mode, System::IO::FileAccess access);

		void AssertNotDisposed();
		void CloseStream();

	internal:

		property std::fstream& Handle
		{
			std::fstream& get(void)
			{
				AssertNotDisposed();
				return *_stream;
			}
		}

	public:
        StdFileStream(System::String^ path, System::IO::FileMode mode);
        StdFileStream(System::String^ path, System::IO::FileMode mode, System::IO::FileAccess access);
		virtual ~StdFileStream(void);

		virtual property bool CanRead {
			bool get(void) override;
		}

		virtual property bool CanSeek {
			bool get(void) override;
		}

		virtual property bool CanWrite {
			bool get(void) override;
		}

		virtual property __int64 Length {
			__int64 get(void) override;
		}

		virtual property __int64 Position {
			__int64 get(void) override;
			void set(__int64 value) override;
		}

		virtual void Flush(void) override;

		virtual void SetLength(__int64 value) override;
		virtual __int64 Seek(__int64 offset, System::IO::SeekOrigin origin) override;

		virtual int Read(cli::array<unsigned char>^ buffer, int offset, int count) override;
		virtual void Write(cli::array<unsigned char>^ buffer, int offset, int count) override;
	};
}
