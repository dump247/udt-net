@echo off

if not defined NuGetExe set NuGetExe=NuGet.exe

%NuGetExe% install UdtProtocol-Test\packages.config -o Packages
