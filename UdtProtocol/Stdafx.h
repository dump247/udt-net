// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef NULL
#define NULL 0
#endif

System::TimeSpan FromMicroseconds(__int64 us);
System::TimeSpan FromMilliseconds(__int64 ms);

__int64 ToMicroseconds(System::TimeSpan value);
