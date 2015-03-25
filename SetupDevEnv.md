# Requirements #

  * Microsoft Visual C++ 2010
  * UDT version 4.11
  * NuGet (http://nuget.org)

# Steps #

  1. Download the latest [UDT version 4.11](http://sourceforge.net/projects/udt/files/udt/4.11/) build
  1. Extract the UDT zip
    * _e.g. C:\Users\**Username**\Documents\Visual Studio 2010\Projects\udt.sdk.4.11.win32\udt4_
  1. Open the udt solution
    * The solution and project files are in the win directory of the zip file
    * Convert the solution to VS2010 as necessary
  1. Change the project to build a static lib. This makes deployment simpler versus a dll.
    * Open udt project properties
    * Select All Configurations
    * Under Configuration Properties => General, set Configuration Type to Static library
    * In udt.h, comment out the UDT\_API definition
```
      //#ifdef UDT_EXPORTS
      //   #define UDT_API __declspec(dllexport)
      //#else
      //   #define UDT_API __declspec(dllimport)
      //#endif
      #define UDT_API
```
    * Add an x64 configuration
  1. Pull the Udt.Net source code
    * _hg clone https://udt-net.googlecode.com/hg/ "C:\Users\**Username**\Documents\Visual Studio 2010\Projects\UdtNet"_
  1. Set the path to udt in UdtProtocol.vcxproj
```
  <PropertyGroup>
    <UdtHome>..\..\udt.sdk.4.11.win32\udt4</UdtHome>
  </PropertyGroup>
```
  1. Update the path to the udt.vcxproj project file in UdtProtocol.sln
  1. Install the NuGet dependency packages
    * In the solution directory, run NuGet-Install.bat

# Building Signed Libs #

  * Due to a bug in VS2010 SP1, Win32 C++/CLI projects can not be signed without patching the CPP targets file.
  * See http://blogs.msdn.com/b/vcblog/archive/2011/03/11/10140139.aspx

# Dynamic Library #
If building udt as a dynamic library, add the UDT\_API tag to the classes CPacket and CUDTCC.