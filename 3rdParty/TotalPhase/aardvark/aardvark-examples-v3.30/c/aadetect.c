/*=========================================================================
| (c) 2003  Total Phase, Inc.
|--------------------------------------------------------------------------
| Project : Aardvark Sample Code
| File    : aadetect.c
|--------------------------------------------------------------------------
| Auto-detection test routine
|--------------------------------------------------------------------------
| Redistribution and use of this file in source and binary forms, with
| or without modification, are permitted.
|
| THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
| "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
| LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
| FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
| COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
| INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
| BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
| LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
| CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
| LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
| ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
| POSSIBILITY OF SUCH DAMAGE.
 ========================================================================*/

//=========================================================================
// INCLUDES
//=========================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aardvark.h"


//=========================================================================
// MAIN PROGRAM ENTRY POINT
//=========================================================================
int main (int argc, char *argv[])
{
    aa_u16 ports[16];
    aa_u32 unique_ids[16];
    int    nmemb = 16;

    int count = aa_find_devices_ext(nmemb, ports, unique_ids);
    int i;

    printf("%d device(s) found:\n", count);

    if (count > nmemb)  count = nmemb;
    for (i = 0; i < count; ++i) {
        const char *status = "(avail) ";
        if (ports[i] & AA_PORT_NOT_FREE) {
            ports[i] &= ~AA_PORT_NOT_FREE;
            status = "(in-use)";
        }
        printf("    port=%-3d %s (%04d-%06d)\n",
               ports[i], status,
               unique_ids[i] / 1000000,
               unique_ids[i] % 1000000);
    }

    return 0;
}
