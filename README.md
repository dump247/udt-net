**Notice** It appears the underlying native udt library is no longer being maintained. There have been no new releases since Feb 2013. This library is simply a wrapper around the native library. This library has very little functionality on it's own. While I can make minor fixes to this wrapper library, I can not fix any issues discovered in the underlying native udt library, where most of the functionality resides.

# Overview

UDT is a reliable UDP based application level data transport protocol for
distributed data intensive applications over wide area high-speed networks.
UDT uses UDP to transfer bulk data with its own reliability control and
congestion control mechanisms. The new protocol can transfer data at a much
higher speed than TCP does. UDT is also a highly configurable framework that
can accommodate various congestion control algorithms.

[Release Notes](//github.com/dump247/udt-net/wiki/Release-Notes)

# Features

* .Net API on top of the native UDT API
* Support for custom congestion control algorithms written in managed code

# Usage

The code is in the Udt namespace and Udt.Socket is the interface to the native
UDT socket library. The Udt.Socket API matches closely with the
[System.Net.Sockets.Socket](http://msdn.microsoft.com/en-us/library/system.net.sockets.socket.aspx)
API.

[Example: Send Receive File](https://github.com/dump247/udt-net/wiki/Example:-Send-Receive-File)

[Socket Performance Info](https://github.com/dump247/udt-net/wiki/Socket-Performance-Info)

# TODO

* Missing some properties in Packet (from CPacket)
* CongestionControl.SendCustomMessage (CCC::sendCustomMsg)
* CongestionControl.SetUserParameter (CCC::setUserParam)
* Something similar to [TcpClient](http://msdn.microsoft.com/en-us/library/system.net.sockets.tcpclient.aspx) and [TcpListener](http://msdn.microsoft.com/en-us/library/system.net.sockets.tcplistener.aspx)
* Asynchronous API
* More Documentation

# Requirements

## Dependencies
* Microsoft .NET 4.0
* Visual C++ Redistributable Package (if you don't have Visual C++ installed)
 * This package contains the VS2010 C Runtime and is required to run applications developed with Visual C++ 2010
 * http://www.microsoft.com/download/en/details.aspx?displaylang=en&id=5555

## Supported Platforms
* Microsoft Windows (Win32 and x64)

## Developers

This project is implemented in Managed C++ and requires Microsoft Visual C++
2010. Building the demo apps and unit tests requires C# compiler.

[Build Project](//github.com/dump247/udt-net/wiki/Setup-Development-Environment)

# License

BSD LICENSE Copyright (c) 2015 Cory Thomas

See [LICENSE](LICENSE)
