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
vbasic/       - Visual Basic language binding files


Binaries
--------
Windows:
bin/win32/aardvark.dll  -  software dynamic link library
bin/win32/aainfo.exe    -  adapter information utility

Linux:
bin/linux/aardvark.so   -  software shared object
bin/linux/aainfo        -  adapter information utility


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
http://www.python.org/2.2.3/

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

The calling conventions for each Aardark Python API function is
clearly documented in the comments of aardvark_py.py.
