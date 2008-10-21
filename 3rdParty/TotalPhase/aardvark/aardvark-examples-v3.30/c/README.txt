                  Total Phase Aardvark Sample Code
                  --------------------------------

Introduction
------------
This directory contains examples that use the Aardvark Rosetta C/C++
language bindings.


Contents
--------
- Makefile for Linux and Windows GCC-MinGW32
- Visual C++ workspace for MSVC++ 6.0 and above

See top level README.txt for descriptions of each example.


Build Instructions
------------------
In each case, PROGRAM can be any of the sample programs, without the
'.c' extension.

Linux:
1) Make sure that 'SYSLIBS=-ldl' is uncommented in the Makefile
2) Type 'make' at the command line
3) _output/PROGRAM

Windows gcc-mingw32:
1) Install GCC MinGW32.
   The latest version can be downloaded from the MinGW website: 
   http://www.mingw.org/
2) Install MSYS
   The latest version can be downloaded from the MinGW website: 
   http://www.mingw.org/
3) Make sure that 'SYSLIBS=' is uncommented in the Makefile
4) Type 'make' at the command line
5) _output/PROGRAM

Windows Visual C++ 6.0:
1) Open MSVC/Makefile.dsw
2) Build | Batch Build... | Build
3) _output/PROGRAM


