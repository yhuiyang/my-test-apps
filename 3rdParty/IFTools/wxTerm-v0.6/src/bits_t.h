/////////////////////////////////////////////////////////////////////////////
// Name:        bits_t.h
// Purpose:     a bitarray template
// Author:      Joachim Buermann
// Id:          $Id: bits_t.h,v 1.1.1.1 2005/05/11 07:40:04 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#ifndef __BITS_T_H
#define __BITS_T_H

// __HCHK_END
#include <string.h>

//
// Bitarray Template for several dimesions
// To create a new bitarray object:
// bits_t(bitabzahl)
// for example a bitarray with 32 bits:
// bits_t(32);
// Make a instance of this with:
// bits_t_32 my_bitarray;
// After this you can set/get some bits with:
// my_bitarray.set(5);
// int bit = my_bitarray.get(5);

#define bits_t(_n_) class bits_t_ ## _n_                          \
{                                                                 \
private:                                                          \
    enum {MAX_COUNT = _n_};                                       \
    char data[MAX_COUNT>>3];                                      \
public:                                                           \
    bits_t_ ## _n_ () {                                           \
	   memset(data,0,sizeof(data));                              \
    };                                                            \
    inline void Clr(int no) {                                     \
        *(char*)(data+(no>>3))&=~((char)1<<(no%8));               \
    };                                                            \
    inline void Clrall() {                                        \
        memset(data,0,sizeof(data));                              \
    };                                                            \
    inline int Get(int no) {                                      \
        return ((*(char*)(data+(no>>3))&((char)1<<(no%8))) != 0); \
    };                                                            \
    inline void Set(int no) {                                     \
        *(char*)(data+(no>>3))|=((char)1<<(no%8));                \
    };                                                            \
    inline int FindSet() {                                        \
        unsigned char* cp = (unsigned char*)data;                 \
        for(int i=0;i<(MAX_COUNT>>3);i++) {                       \
            if(*cp) {                                             \
                unsigned char m=1; int n=0;                       \
                while(!(m&*cp)) {m <<= 1; n++;}                   \
	           return ((i<<3) + n);                              \
            }                                                     \
            cp++;                                                 \
        }                                                         \
        return -1;                                                \
    };                                                            \
    inline int FindClr() {                                        \
	   unsigned char* cp = (unsigned char*)data;                 \
	   for(int i=0;i<(MAX_COUNT>>3);i++) {                       \
	       if((*cp & 0xff) != 0xff) {                            \
		      unsigned char m=1; int n=0;                       \
		      while(m&*cp) {m <<= 1; n++;}                      \
		      return ((i<<3) + n);                              \
	       }                                                     \
	       cp++;                                                 \
	   }                                                         \
	   return -1;                                                \
    };                                                            \
}                                                                 \

#endif
