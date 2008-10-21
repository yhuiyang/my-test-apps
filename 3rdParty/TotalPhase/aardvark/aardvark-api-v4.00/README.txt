                      Aardvark I2C/SPI Software
                      -------------------------

Introduction
------------
This software is used to interface with the Aardvark I2C/SPI
Embedded Systems Interface.  It provides APIs in a number of
languages for maximum development flexibility.


Directory Structure
-------------------
bin/          - platform-specific binaries
c/            - C/C++ language binding files
python/       - Python language binding files
csharp/       - C# language binding files
net/          - .NET language binding files
vb.net/       - Visual Basic .NET language binding files
vb6/          - Visual Basic 6 language binding files


Binaries
--------
Windows:
bin/win32/aardvark.dll  -  software dynamic link library
bin/win32/aainfo.exe    -  adapter information utility

Linux:
bin/linux/aardvark.so   -  software shared object
bin/linux/aainfo        -  adapter information utility

Under Linux, the libusb library must be installed since the
Aardvark software library is dynamically linked to libusb.


C/C++ bindings
--------------
1) Create a new C/C++ project or open an existing project
2) Add aardvark.c and aardvark.h to the project
3) Place aardvark.dll (or aardvark.so for Linux) in the PATH
4) Place #include "aardvark.h" in any module that uses the API
5) Develop, compile, and run your project


Python bindings
---------------
If not already installed, download Python from:
http://www.python.org/2.5/

1) Copy aardvark_py.py to a new folder
2) Copy aardvark.dll (or aardvark.so for Linux) to the same folder
3) Create a new script (i.e. program.py)
4) Put the following line in your script file:
   from aardvark_py import *
5) Develop and run your script

There are two main difference between the Aardvark API documented in the
datasheet and the Aardvark Python API.

1) The "array" type is used for passing data to and from the
Aardvark Python API.  See the Python documentation for more
information on this type.

2) Aardvark Python API functions can return multiple arguments on
the left hand side.  This obviates the need to pass pointers to
output arguments on the right hand side of API functions.

The calling conventions for each Aardvark Python API function is
clearly documented in the comments of aardvark_py.py.


C# bindings
-----------
1) Create a new C/C++ project or open an existing project
2) Add aardvark.cs to the project
3) For Windows, place aardvark.dll in the PATH;
   For Linux rename aardvark.so to libaardvark.so and place in the 
   LD_LIBRARY_PATH
4) Develop, compile, and run your project


.NET and VB.NET bindings
------------------------
Copy aardvark.dll and aardvark_net.dll to your application development
environment.  The aardvark.dll contains the APIs themselves and the
aardvark_net.dll provides the .NET interface to the APIs.  For
detailed documentation of APIs refer to the datasheet and the comments
in the C# (aardvark.cs) binding.


Visual Basic 6
--------------
Copy aardvark.dll to your application development environment and
integrate the aardvark.bas interface layer into your application.
For detailed documentation of the APIs refer to the datasheet
and the comments in the aardvark.bas file.
