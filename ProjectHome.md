# UDT.Net #

.Net wrapper around the native [UDT](http://udt.sourceforge.net) protocol library.

UDT is a reliable UDP based application level data transport protocol for distributed data intensive applications over wide area high-speed networks. UDT uses UDP to transfer bulk data with its own reliability control and congestion control mechanisms. The new protocol can transfer data at a much higher speed than TCP does. UDT is also a highly configurable framework that can accommodate various congestion control algorithms.

# Overview #

## Features ##

  * .Net API on top of the native UDT API
  * Support for custom congestion control algorithms written in managed code

## TODO ##

  * Missing some properties in Packet (from CPacket)
  * CongestionControl.SendCustomMessage (CCC::sendCustomMsg)
  * CongestionControl.SetUserParameter (CCC::setUserParam)
  * Something similar to [TcpClient](http://msdn.microsoft.com/en-us/library/system.net.sockets.tcpclient.aspx) and [TcpListener](http://msdn.microsoft.com/en-us/library/system.net.sockets.tcplistener.aspx)
  * Asynchronous API
  * More Documentation

## Release Notes ##

### 0.10.0.0 (2013-12-01) ###

  * Fixed [issue #18](https://code.google.com/p/udt-net/issues/detail?id=#18): Calling close on a socket with a broken connection throws an exception (unknown socket id)
    * Invalid socket id errors are now ignored on close

### 0.9.0.1 (2013-12-01) ###

  * Fixed [issue #19](https://code.google.com/p/udt-net/issues/detail?id=#19): Win32 assembly in the "Signed" download is not signed
    * Due to a bug in VS2010 SP1
    * Building locally requires patching the CPP Win32 targets file
    * http://blogs.msdn.com/b/vcblog/archive/2011/03/11/10140139.aspx

### 0.9.0.0 (2013-09-04) ###

  * Added support for SendData, ReceiveData, Events, and State socket options
    * Resolves [issue #12](https://code.google.com/p/udt-net/issues/detail?id=#12), [issue #13](https://code.google.com/p/udt-net/issues/detail?id=#13), [issue #14](https://code.google.com/p/udt-net/issues/detail?id=#14), and [issue #15](https://code.google.com/p/udt-net/issues/detail?id=#15)
    * Thanks to [Neeraj.Sharma.in](https://code.google.com/u/103373145305864883309/)

### 0.8.0.1 (2013-08-12) ###

  * No functionality change. Added signed/strongly named assembly build.

### 0.8.0.0 (2013-08-06) ###

  * Resolved [issue #11](https://code.google.com/p/udt-net/issues/detail?id=#11)
  * This release makes a small change to the way that Socket::Send works
    * If the socket is in blocking send mode, Send will now block until the entire buffer has been sent
    * If the socket is in non-blocking send mode, Send will send as many bytes as possible and will return the number of bytes sent, possibly zero
    * The change makes Socket operation more inline with System.Net.Sockets.Socket, except that the .NET Socket class doesn't allow Send on a non-blocking socket

### 0.7.0.0 (2013-05-27) ###

  * Updated to udt 4.11

### 0.6.0.0 (2012-02-25) ###

  * Updated to udt 4.10
    * There are some functionality changes in the native udt layer:
      1. SocketPoller will report all listening sockets in WriteSockets
      1. Removing a closed socket from SocketPoller no longer raises an exception
  * Added Bind overload that accepts an existing UDP System.Net.Sockets.Socket
    * Only use this method if you understand the potential issues
    * Thanks to Stefano Guerrini
  * Resolved [issue #7](https://code.google.com/p/udt-net/issues/detail?id=#7): StdFileStream Length/Seek fails on files larger than 2GB
    * Thanks to falqsfalqs for reporting
  * Closing a Udt.Socket now invalidates the object
    * Sockets are no longer reusable

### 0.5.0.0 (2011-12-08) ###

  * Fixed 2 bugs
    * SocketPoller.Wait was putting the read sockets into WriteSockets
    * StdFileStream had a condition bug (= instead of ==)
  * Refactored custom congestion control API so it actually works
  * Added Packet implementations
    * Still in progress

### 0.4.0.0 (2011-07-19) ###

  * Added Socket.SendFile/ReceiveFile overloads that accept a stream
    * The methods accept Udt.StdFileStream, as opposed to System.IO.FileStream, due to the requirements of the native UDT library
  * Added Socket.SendFile overloads with offset and count parameters
    * Allows sending segments or parts of a file instead of the entire file
  * Restructuring to Packet class hierarchy
    * Packet, ControlPacket : Packet, DataPacket : Packet
    * The support is still incomplete, however

# Requirements #

## Dependencies ##
  * Microsoft .NET 4.0
  * Visual C++ Redistributable Package (if you don't have Visual C++ installed)
    * This package contains the VS2010 C Runtime and is required to run applications developed with Visual C++ 2010
    * http://www.microsoft.com/download/en/details.aspx?displaylang=en&id=5555

## Supported Platforms ##
  * Microsoft Windows (Win32 and x64)

# Documentation #

## Usage ##

Download the latest release and add a reference to UdtProtocol.dll.

The code is in the Udt namespace and Udt.Socket is the interface to the native UDT socket library. The Udt.Socket API matches closely with the [System.Net.Sockets.Socket](http://msdn.microsoft.com/en-us/library/system.net.sockets.socket.aspx) API.

## Developers ##
This project is implemented in Managed C++ and requires Microsoft Visual C++ 2010. Building the demo apps and unit tests requires C# compiler.

[Setup Environment](SetupDevEnv.md)