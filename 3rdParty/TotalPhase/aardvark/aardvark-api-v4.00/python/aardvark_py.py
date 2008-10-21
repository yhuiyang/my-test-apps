#==========================================================================
# Aardvark Interface Library
#--------------------------------------------------------------------------
# Copyright (c) 2002-2007 Total Phase, Inc.
# All rights reserved.
# www.totalphase.com
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# - Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
# - Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
#
# - Neither the name of Total Phase, Inc. nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#--------------------------------------------------------------------------
# To access Aardvark devices through the API:
#
# 1) Use one of the following shared objects:
#      aardvark.so      --  Linux shared object
#      aardvark.dll     --  Windows dynamic link library
#
# 2) Along with one of the following language modules:
#      aardvark.c/h     --  C/C++ API header file and interface module
#      aardvark_py.py   --  Python API
#      aardvark.bas     --  Visual Basic 6 API
#      aardvark.cs      --  C# .NET source
#      aardvark_net.dll --  Compiled .NET binding
#==========================================================================


#==========================================================================
# VERSION
#==========================================================================
AA_API_VERSION    = 0x0400   # v4.00
AA_REQ_SW_VERSION = 0x0400   # v4.00

import os
import sys
import imp
if os.name != 'nt' or (sys.version_info[0] == 2 and sys.version_info[1] < 5):
    import aardvark as api
else:
    api = imp.load_dynamic('aardvark', 'aardvark.dll')

AA_SW_VERSION      = api.py_version() & 0xffff
AA_REQ_API_VERSION = (api.py_version() >> 16) & 0xffff
AA_LIBRARY_LOADED  = \
    ((AA_SW_VERSION >= AA_REQ_SW_VERSION) and \
     (AA_API_VERSION >= AA_REQ_API_VERSION))

from array import array, ArrayType
import struct


#==========================================================================
# STATUS CODES
#==========================================================================
# All API functions return an integer which is the result of the
# transaction, or a status code if negative.  The status codes are
# defined as follows:
# enum AardvarkStatus
# General codes (0 to -99)
AA_OK                        =    0
AA_UNABLE_TO_LOAD_LIBRARY    =   -1
AA_UNABLE_TO_LOAD_DRIVER     =   -2
AA_UNABLE_TO_LOAD_FUNCTION   =   -3
AA_INCOMPATIBLE_LIBRARY      =   -4
AA_INCOMPATIBLE_DEVICE       =   -5
AA_COMMUNICATION_ERROR       =   -6
AA_UNABLE_TO_OPEN            =   -7
AA_UNABLE_TO_CLOSE           =   -8
AA_INVALID_HANDLE            =   -9
AA_CONFIG_ERROR              =  -10

# I2C codes (-100 to -199)
AA_I2C_NOT_AVAILABLE         = -100
AA_I2C_NOT_ENABLED           = -101
AA_I2C_READ_ERROR            = -102
AA_I2C_WRITE_ERROR           = -103
AA_I2C_SLAVE_BAD_CONFIG      = -104
AA_I2C_SLAVE_READ_ERROR      = -105
AA_I2C_SLAVE_TIMEOUT         = -106
AA_I2C_DROPPED_EXCESS_BYTES  = -107
AA_I2C_BUS_ALREADY_FREE      = -108

# SPI codes (-200 to -299)
AA_SPI_NOT_AVAILABLE         = -200
AA_SPI_NOT_ENABLED           = -201
AA_SPI_WRITE_ERROR           = -202
AA_SPI_SLAVE_READ_ERROR      = -203
AA_SPI_SLAVE_TIMEOUT         = -204
AA_SPI_DROPPED_EXCESS_BYTES  = -205

# GPIO codes (-400 to -499)
AA_GPIO_NOT_AVAILABLE        = -400

# I2C bus monitor codes (-500 to -599)
AA_I2C_MONITOR_NOT_AVAILABLE = -500
AA_I2C_MONITOR_NOT_ENABLED   = -501


#==========================================================================
# GENERAL TYPE DEFINITIONS
#==========================================================================
# Aardvark handle type definition
# typedef Aardvark => integer

# Deprecated type definitions.
#
# These are only for use with legacy code and
# should not be used for new development.
# typedef aa_u08 => integer

# typedef aa_u16 => integer

# typedef aa_u32 => integer

# typedef aa_s08 => integer

# typedef aa_s16 => integer

# typedef aa_s32 => integer

# Aardvark version matrix.
# 
# This matrix describes the various version dependencies
# of Aardvark components.  It can be used to determine
# which component caused an incompatibility error.
# 
# All version numbers are of the format:
#   (major << 8) | minor
# 
# ex. v1.20 would be encoded as:  0x0114
class AardvarkVersion:
    def __init__ (self):
        # Software, firmware, and hardware versions.
        self.software      = 0
        self.firmware      = 0
        self.hardware      = 0

        # Firmware requires that software must be >= this version.
        self.sw_req_by_fw  = 0

        # Software requires that firmware must be >= this version.
        self.fw_req_by_sw  = 0

        # Software requires that the API interface must be >= this version.
        self.api_req_by_sw = 0


#==========================================================================
# GENERAL API
#==========================================================================
# Get a list of ports to which Aardvark devices are attached.
# 
# nelem   = maximum number of elements to return
# devices = array into which the port numbers are returned
# 
# Each element of the array is written with the port number.
# Devices that are in-use are ORed with AA_PORT_NOT_FREE (0x8000).
#
# ex.  devices are attached to ports 0, 1, 2
#      ports 0 and 2 are available, and port 1 is in-use.
#      array => 0x0000, 0x8001, 0x0002
# 
# If the array is NULL, it is not filled with any values.
# If there are more devices than the array size, only the
# first nmemb port numbers will be written into the array.
# 
# Returns the number of devices found, regardless of the
# array size.
AA_PORT_NOT_FREE = 0x8000
def aa_find_devices (nelem):
    "usage: (int, u16[]) = aa_find_devices(int)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    devices = array('H', [ 0 for i in range(nelem) ])
    (_ret_, devices) = api.py_aa_find_devices(nelem, devices)
    del devices[max(0, min(_ret_, len(devices))):]
    return (_ret_, devices)


# Get a list of ports to which Aardvark devices are attached.
#
# This function is the same as aa_find_devices() except that
# it returns the unique IDs of each Aardvark device.  The IDs
# are guaranteed to be non-zero if valid.
#
# The IDs are the unsigned integer representation of the 10-digit
# serial numbers.
def aa_find_devices_ext (nelem):
    "usage: (int, u16[], u32[]) = aa_find_devices_ext(int)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    devices = array('H', [ 0 for i in range(nelem) ])
    unique_ids = array('I', [ 0 for i in range(nelem) ])
    (_ret_, devices, unique_ids) = api.py_aa_find_devices_ext(nelem, devices, unique_ids)
    del devices[max(0, min(_ret_, len(devices))):]
    del unique_ids[max(0, min(_ret_, len(unique_ids))):]
    return (_ret_, devices, unique_ids)


# Open the Aardvark port.
# 
# The port number is a zero-indexed integer.
#
# The port number is the same as that obtained from the
# aa_find_devices() function above.
# 
# Returns an Aardvark handle, which is guaranteed to be
# greater than zero if it is valid.
# 
# This function is recommended for use in simple applications
# where extended information is not required.  For more complex
# applications, the use of aa_open_ext() is recommended.
def aa_open (port_number):
    "usage: Aardvark = aa_open(int)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_open(port_number)


# Open the Aardvark port, returning extended information
# in the supplied structure.  Behavior is otherwise identical
# to aa_open() above.  If 0 is passed as the pointer to the
# structure, this function is exactly equivalent to aa_open().
# 
# The structure is zeroed before the open is attempted.
# It is filled with whatever information is available.
# 
# For example, if the firmware version is not filled, then
# the device could not be queried for its version number.
# 
# This function is recommended for use in complex applications
# where extended information is required.  For more simple
# applications, the use of aa_open() is recommended.
class AardvarkExt:
    def __init__ (self):
        # Version matrix
        self.version  = AardvarkVersion()

        # Features of this device.
        self.features = 0

def aa_open_ext (port_number):
    "usage: (Aardvark, AardvarkExt) = aa_open_ext(int)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    (_ret_, c_aa_ext) = api.py_aa_open_ext(port_number)
    aa_ext = AardvarkExt()
    (aa_ext.version.software, aa_ext.version.firmware, aa_ext.version.hardware, aa_ext.version.sw_req_by_fw, aa_ext.version.fw_req_by_sw, aa_ext.version.api_req_by_sw, aa_ext.features) = struct.unpack("HHHHHHi", c_aa_ext)
    return (_ret_, aa_ext)


# Close the Aardvark port.
def aa_close (aardvark):
    "usage: int = aa_close(Aardvark)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_close(aardvark)


# Return the port for this Aardvark handle.
# 
# The port number is a zero-indexed integer.
def aa_port (aardvark):
    "usage: int = aa_port(Aardvark)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_port(aardvark)


# Return the device features as a bit-mask of values, or
# an error code if the handle is not valid.
AA_FEATURE_SPI = 0x00000001
AA_FEATURE_I2C = 0x00000002
AA_FEATURE_GPIO = 0x00000008
AA_FEATURE_I2C_MONITOR = 0x00000010
def aa_features (aardvark):
    "usage: int = aa_features(Aardvark)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_features(aardvark)


# Return the unique ID for this Aardvark adapter.
# IDs are guaranteed to be non-zero if valid.
# The ID is the unsigned integer representation of the
# 10-digit serial number.
def aa_unique_id (aardvark):
    "usage: u32 = aa_unique_id(Aardvark)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_unique_id(aardvark) & 0xffffffffL


# Return the status string for the given status code.
# If the code is not valid or the library function cannot
# be loaded, return a NULL string.
def aa_status_string (status):
    "usage: str = aa_status_string(int)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_status_string(status)


# Enable logging to a file.  The handle must be standard file
# descriptor.  In C, a file descriptor can be obtained by using
# the ANSI C function "open" or by using the function "fileno"
# on a FILE* stream.  A FILE* stream can be obtained using "fopen"
# or can correspond to the common "stdout" or "stderr" --
# available when including stdlib.h
def aa_log (aardvark, level, handle):
    "usage: int = aa_log(Aardvark, int, int)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_log(aardvark, level, handle)


# Return the version matrix for the device attached to the
# given handle.  If the handle is 0 or invalid, only the
# software and required api versions are set.
def aa_version (aardvark):
    "usage: (int, AardvarkVersion) = aa_version(Aardvark)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    (_ret_, c_version) = api.py_aa_version(aardvark)
    version = AardvarkVersion()
    (version.software, version.firmware, version.hardware, version.sw_req_by_fw, version.fw_req_by_sw, version.api_req_by_sw) = struct.unpack("HHHHHH", c_version)
    return (_ret_, version)


# Configure the device by enabling/disabling I2C, SPI, and
# GPIO functions.
# enum AardvarkConfig
AA_CONFIG_GPIO_ONLY = 0x00
AA_CONFIG_SPI_GPIO  = 0x01
AA_CONFIG_GPIO_I2C  = 0x02
AA_CONFIG_SPI_I2C   = 0x03
AA_CONFIG_QUERY     = 0x80

AA_CONFIG_SPI_MASK = 0x00000001
AA_CONFIG_I2C_MASK = 0x00000002
def aa_configure (aardvark, config):
    "usage: int = aa_configure(Aardvark, AardvarkConfig)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_configure(aardvark, config)


# Configure the target power pins.
# This is only supported on hardware versions >= 2.00
AA_TARGET_POWER_NONE = 0x00
AA_TARGET_POWER_BOTH = 0x03
AA_TARGET_POWER_QUERY = 0x80
def aa_target_power (aardvark, power_mask):
    "usage: int = aa_target_power(Aardvark, u08)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_target_power(aardvark, power_mask)


# Sleep for the specified number of milliseconds
# Accuracy depends on the operating system scheduler
# Returns the number of milliseconds slept
def aa_sleep_ms (milliseconds):
    "usage: u32 = aa_sleep_ms(u32)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_sleep_ms(milliseconds) & 0xffffffffL



#==========================================================================
# ASYNC MESSAGE POLLING
#==========================================================================
# Polling function to check if there are any asynchronous
# messages pending for processing. The function takes a timeout
# value in units of milliseconds.  If the timeout is < 0, the
# function will block until data is received.  If the timeout is 0,
# the function will perform a non-blocking check.
AA_ASYNC_NO_DATA = 0x00000000
AA_ASYNC_I2C_READ = 0x00000001
AA_ASYNC_I2C_WRITE = 0x00000002
AA_ASYNC_SPI = 0x00000004
AA_ASYNC_I2C_MONITOR = 0x00000008
def aa_async_poll (aardvark, timeout):
    "usage: int = aa_async_poll(Aardvark, int)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_async_poll(aardvark, timeout)



#==========================================================================
# I2C API
#==========================================================================
# Free the I2C bus.
def aa_i2c_free_bus (aardvark):
    "usage: int = aa_i2c_free_bus(Aardvark)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_i2c_free_bus(aardvark)


# Set the I2C bit rate in kilohertz.  If a zero is passed as the
# bitrate, the bitrate is unchanged and the current bitrate is
# returned.
def aa_i2c_bitrate (aardvark, bitrate_khz):
    "usage: int = aa_i2c_bitrate(Aardvark, int)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_i2c_bitrate(aardvark, bitrate_khz)


# enum AardvarkI2cFlags
AA_I2C_NO_FLAGS     = 0x00
AA_I2C_10_BIT_ADDR  = 0x01
AA_I2C_COMBINED_FMT = 0x02
AA_I2C_NO_STOP      = 0x04

# Read a stream of bytes from the I2C slave device.
def aa_i2c_read (aardvark, slave_addr, flags, num_bytes):
    "usage: (int, u08[]) = aa_i2c_read(Aardvark, u16, AardvarkI2cFlags, u16)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    data_in = array('B', [ 0 for i in range(num_bytes) ])
    (_ret_, data_in) = api.py_aa_i2c_read(aardvark, slave_addr, flags, num_bytes, data_in)
    del data_in[max(0, min(_ret_, len(data_in))):]
    return (_ret_, data_in)


# enum AardvarkI2cStatus
AA_I2C_STATUS_OK            = 0
AA_I2C_STATUS_BUS_ERROR     = 1
AA_I2C_STATUS_SLA_ACK       = 2
AA_I2C_STATUS_SLA_NACK      = 3
AA_I2C_STATUS_DATA_NACK     = 4
AA_I2C_STATUS_ARB_LOST      = 5
AA_I2C_STATUS_BUS_LOCKED    = 6
AA_I2C_STATUS_LAST_DATA_ACK = 7

# Read a stream of bytes from the I2C slave device.
# This API function returns the number of bytes read into
# the num_read variable.  The return value of the function
# is a status code.
def aa_i2c_read_ext (aardvark, slave_addr, flags, num_bytes):
    "usage: (int, u08[], u16) = aa_i2c_read_ext(Aardvark, u16, AardvarkI2cFlags, u16)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    data_in = array('B', [ 0 for i in range(num_bytes) ])
    (_ret_, data_in, num_read) = api.py_aa_i2c_read_ext(aardvark, slave_addr, flags, num_bytes, data_in)
    del data_in[max(0, min(num_read, len(data_in))):]
    return (_ret_, data_in, num_read)


# Write a stream of bytes to the I2C slave device.
def aa_i2c_write (aardvark, slave_addr, flags, data_out):
    "usage: int = aa_i2c_write(Aardvark, u16, AardvarkI2cFlags, u08[])"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    if not isinstance(data_out, ArrayType) or data_out.typecode != 'B': raise TypeError("type for 'data_out' must be array('B')")
    num_bytes = len(data_out)
    return api.py_aa_i2c_write(aardvark, slave_addr, flags, num_bytes, data_out)


# Write a stream of bytes to the I2C slave device.
# This API function returns the number of bytes written into
# the num_written variable.  The return value of the function
# is a status code.
def aa_i2c_write_ext (aardvark, slave_addr, flags, data_out):
    "usage: (int, u16) = aa_i2c_write_ext(Aardvark, u16, AardvarkI2cFlags, u08[])"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    if not isinstance(data_out, ArrayType) or data_out.typecode != 'B': raise TypeError("type for 'data_out' must be array('B')")
    num_bytes = len(data_out)
    return api.py_aa_i2c_write_ext(aardvark, slave_addr, flags, num_bytes, data_out)


# Enable/Disable the Aardvark as an I2C slave device
def aa_i2c_slave_enable (aardvark, addr, maxTxBytes, maxRxBytes):
    "usage: int = aa_i2c_slave_enable(Aardvark, u08, u16, u16)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_i2c_slave_enable(aardvark, addr, maxTxBytes, maxRxBytes)


def aa_i2c_slave_disable (aardvark):
    "usage: int = aa_i2c_slave_disable(Aardvark)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_i2c_slave_disable(aardvark)


# Set the slave response in the event the Aardvark is put
# into slave mode and contacted by a Master.
def aa_i2c_slave_set_response (aardvark, data_out):
    "usage: int = aa_i2c_slave_set_response(Aardvark, u08[])"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    if not isinstance(data_out, ArrayType) or data_out.typecode != 'B': raise TypeError("type for 'data_out' must be array('B')")
    num_bytes = len(data_out)
    return api.py_aa_i2c_slave_set_response(aardvark, num_bytes, data_out)


# Return number of bytes written from a previous
# Aardvark->I2C_master transmission.  Since the transmission is
# happening asynchronously with respect to the PC host
# software, there could be responses queued up from many
# previous write transactions.
def aa_i2c_slave_write_stats (aardvark):
    "usage: int = aa_i2c_slave_write_stats(Aardvark)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_i2c_slave_write_stats(aardvark)


# Read the bytes from an I2C slave reception
def aa_i2c_slave_read (aardvark, num_bytes):
    "usage: (int, u08, u08[]) = aa_i2c_slave_read(Aardvark, u16)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    data_in = array('B', [ 0 for i in range(num_bytes) ])
    (_ret_, addr, data_in) = api.py_aa_i2c_slave_read(aardvark, num_bytes, data_in)
    del data_in[max(0, min(_ret_, len(data_in))):]
    return (_ret_, addr, data_in)


# Extended functions that return status code    
def aa_i2c_slave_write_stats_ext (aardvark):
    "usage: (int, u16) = aa_i2c_slave_write_stats_ext(Aardvark)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_i2c_slave_write_stats_ext(aardvark)


def aa_i2c_slave_read_ext (aardvark, num_bytes):
    "usage: (int, u08, u08[], u16) = aa_i2c_slave_read_ext(Aardvark, u16)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    data_in = array('B', [ 0 for i in range(num_bytes) ])
    (_ret_, addr, data_in, num_read) = api.py_aa_i2c_slave_read_ext(aardvark, num_bytes, data_in)
    del data_in[max(0, min(num_read, len(data_in))):]
    return (_ret_, addr, data_in, num_read)


# Enable the I2C bus monitor
# This disables all other functions on the Aardvark adapter
def aa_i2c_monitor_enable (aardvark):
    "usage: int = aa_i2c_monitor_enable(Aardvark)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_i2c_monitor_enable(aardvark)


# Disable the I2C bus monitor
def aa_i2c_monitor_disable (aardvark):
    "usage: int = aa_i2c_monitor_disable(Aardvark)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_i2c_monitor_disable(aardvark)


# Read the data collected by the bus monitor
AA_I2C_MONITOR_DATA = 0x00ff
AA_I2C_MONITOR_NACK = 0x0100
AA_I2C_MONITOR_CMD_START = 0xff00
AA_I2C_MONITOR_CMD_STOP = 0xff01
def aa_i2c_monitor_read (aardvark, num_bytes):
    "usage: (int, u16[]) = aa_i2c_monitor_read(Aardvark, u16)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    data = array('H', [ 0 for i in range(num_bytes) ])
    (_ret_, data) = api.py_aa_i2c_monitor_read(aardvark, num_bytes, data)
    del data[max(0, min(_ret_, len(data))):]
    return (_ret_, data)


# Configure the I2C pullup resistors.
# This is only supported on hardware versions >= 2.00
AA_I2C_PULLUP_NONE = 0x00
AA_I2C_PULLUP_BOTH = 0x03
AA_I2C_PULLUP_QUERY = 0x80
def aa_i2c_pullup (aardvark, pullup_mask):
    "usage: int = aa_i2c_pullup(Aardvark, u08)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_i2c_pullup(aardvark, pullup_mask)



#==========================================================================
# SPI API
#==========================================================================
# Set the SPI bit rate in kilohertz.  If a zero is passed as the
# bitrate, the bitrate is unchanged and the current bitrate is
# returned.
def aa_spi_bitrate (aardvark, bitrate_khz):
    "usage: int = aa_spi_bitrate(Aardvark, int)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_spi_bitrate(aardvark, bitrate_khz)


# These configuration parameters specify how to clock the
# bits that are sent and received on the Aardvark SPI
# interface.
# 
#   The polarity option specifies which transition
#   constitutes the leading edge and which transition is the
#   falling edge.  For example, AA_SPI_POL_RISING_FALLING
#   would configure the SPI to idle the SCK clock line low.
#   The clock would then transition low-to-high on the
#   leading edge and high-to-low on the trailing edge.
# 
#   The phase option determines whether to sample or setup on
#   the leading edge.  For example, AA_SPI_PHASE_SAMPLE_SETUP
#   would configure the SPI to sample on the leading edge and
#   setup on the trailing edge.
# 
#   The bitorder option is used to indicate whether LSB or
#   MSB is shifted first.
# 
# See the diagrams in the Aardvark datasheet for
# more details.
# enum AardvarkSpiPolarity
AA_SPI_POL_RISING_FALLING = 0
AA_SPI_POL_FALLING_RISING = 1

# enum AardvarkSpiPhase
AA_SPI_PHASE_SAMPLE_SETUP = 0
AA_SPI_PHASE_SETUP_SAMPLE = 1

# enum AardvarkSpiBitorder
AA_SPI_BITORDER_MSB = 0
AA_SPI_BITORDER_LSB = 1

# Configure the SPI master or slave interface
def aa_spi_configure (aardvark, polarity, phase, bitorder):
    "usage: int = aa_spi_configure(Aardvark, AardvarkSpiPolarity, AardvarkSpiPhase, AardvarkSpiBitorder)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_spi_configure(aardvark, polarity, phase, bitorder)


# Write a stream of bytes to the downstream SPI slave device.
def aa_spi_write (aardvark, data_out):
    "usage: (int, u08[]) = aa_spi_write(Aardvark, u08[])"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    if not isinstance(data_out, ArrayType) or data_out.typecode != 'B': raise TypeError("type for 'data_out' must be array('B')")
    num_bytes = len(data_out)
    data_in = array('B', [ 0 for i in range(num_bytes) ])
    (_ret_, data_in) = api.py_aa_spi_write(aardvark, num_bytes, data_out, data_in)
    del data_in[max(0, min(_ret_, len(data_in))):]
    return (_ret_, data_in)


# Enable/Disable the Aardvark as an SPI slave device
def aa_spi_slave_enable (aardvark):
    "usage: int = aa_spi_slave_enable(Aardvark)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_spi_slave_enable(aardvark)


def aa_spi_slave_disable (aardvark):
    "usage: int = aa_spi_slave_disable(Aardvark)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_spi_slave_disable(aardvark)


# Set the slave response in the event the Aardvark is put
# into slave mode and contacted by a Master.
def aa_spi_slave_set_response (aardvark, data_out):
    "usage: int = aa_spi_slave_set_response(Aardvark, u08[])"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    if not isinstance(data_out, ArrayType) or data_out.typecode != 'B': raise TypeError("type for 'data_out' must be array('B')")
    num_bytes = len(data_out)
    return api.py_aa_spi_slave_set_response(aardvark, num_bytes, data_out)


# Read the bytes from an SPI slave reception
def aa_spi_slave_read (aardvark, num_bytes):
    "usage: (int, u08[]) = aa_spi_slave_read(Aardvark, u16)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    data_in = array('B', [ 0 for i in range(num_bytes) ])
    (_ret_, data_in) = api.py_aa_spi_slave_read(aardvark, num_bytes, data_in)
    del data_in[max(0, min(_ret_, len(data_in))):]
    return (_ret_, data_in)


# Change the output polarity on the SS line.
# 
# Note: When configured as an SPI slave, the Aardvark will
# always be setup with SS as active low.  Hence this function
# only affects the SPI master functions on the Aardvark.
# enum AardvarkSpiSSPolarity
AA_SPI_SS_ACTIVE_LOW  = 0
AA_SPI_SS_ACTIVE_HIGH = 1

def aa_spi_master_ss_polarity (aardvark, polarity):
    "usage: int = aa_spi_master_ss_polarity(Aardvark, AardvarkSpiSSPolarity)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_spi_master_ss_polarity(aardvark, polarity)



#==========================================================================
# GPIO API
#==========================================================================
# The following enumerated type maps the named lines on the
# Aardvark I2C/SPI line to bit positions in the GPIO API.
# All GPIO API functions will index these lines through an
# 8-bit masked value.  Thus, each bit position in the mask
# can be referred back its corresponding line through the
# enumerated type.
# enum AardvarkGpioBits
AA_GPIO_SCL  = 0x01
AA_GPIO_SDA  = 0x02
AA_GPIO_MISO = 0x04
AA_GPIO_SCK  = 0x08
AA_GPIO_MOSI = 0x10
AA_GPIO_SS   = 0x20

# Configure the GPIO, specifying the direction of each bit.
# 
# A call to this function will not change the value of the pullup
# mask in the Aardvark.  This is illustrated by the following
# example:
#   (1) Direction mask is first set to 0x00
#   (2) Pullup is set to 0x01
#   (3) Direction mask is set to 0x01
#   (4) Direction mask is later set back to 0x00.
# 
# The pullup will be active after (4).
# 
# On Aardvark power-up, the default value of the direction
# mask is 0x00.
AA_GPIO_DIR_INPUT = 0
AA_GPIO_DIR_OUTPUT = 1
def aa_gpio_direction (aardvark, direction_mask):
    "usage: int = aa_gpio_direction(Aardvark, u08)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_gpio_direction(aardvark, direction_mask)


# Enable an internal pullup on any of the GPIO input lines.
# 
# Note: If a line is configured as an output, the pullup bit
# for that line will be ignored, though that pullup bit will
# be cached in case the line is later configured as an input.
# 
# By default the pullup mask is 0x00.
AA_GPIO_PULLUP_OFF = 0
AA_GPIO_PULLUP_ON = 1
def aa_gpio_pullup (aardvark, pullup_mask):
    "usage: int = aa_gpio_pullup(Aardvark, u08)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_gpio_pullup(aardvark, pullup_mask)


# Read the current digital values on the GPIO input lines.
# 
# The bits will be ordered as described by AA_GPIO_BITS.  If a
# line is configured as an output, its corresponding bit
# position in the mask will be undefined.
def aa_gpio_get (aardvark):
    "usage: int = aa_gpio_get(Aardvark)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_gpio_get(aardvark)


# Set the outputs on the GPIO lines.
# 
# Note: If a line is configured as an input, it will not be
# affected by this call, but the output value for that line
# will be cached in the event that the line is later
# configured as an output.
def aa_gpio_set (aardvark, value):
    "usage: int = aa_gpio_set(Aardvark, u08)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_gpio_set(aardvark, value)


# Block until there is a change on the GPIO input lines.
# Pins configured as outputs will be ignored.
# 
# The function will return either when a change has occurred or
# the timeout expires.  The timeout, specified in millisecods, has
# a precision of ~16 ms. The maximum allowable timeout is
# approximately 4 seconds. If the timeout expires, this function
# will return the current state of the GPIO lines.
#
# This function will return immediately with the current value
# of the GPIO lines for the first invocation after any of the
# following functions are called: aa_configure,
# aa_gpio_direction, or aa_gpio_pullup.
#
# If the function aa_gpio_get is called before calling
# aa_gpio_change, aa_gpio_change will only register any changes
# from the value last returned by aa_gpio_get.
def aa_gpio_change (aardvark, timeout):
    "usage: int = aa_gpio_change(Aardvark, u16)"
    if not AA_LIBRARY_LOADED: return AA_INCOMPATIBLE_LIBRARY
    return api.py_aa_gpio_change(aardvark, timeout)


