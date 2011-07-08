// stdafx.cpp : source file that includes just the standard includes
// UdtProtocol.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

System::TimeSpan FromMicroseconds(__int64 us)
{
	return System::TimeSpan(us * 10);
}

System::TimeSpan FromMilliseconds(__int64 ms)
{
	return System::TimeSpan(ms * 10000);
}

__int64 ToMicroseconds(System::TimeSpan value)
{
	return value.Ticks / 10;
}
