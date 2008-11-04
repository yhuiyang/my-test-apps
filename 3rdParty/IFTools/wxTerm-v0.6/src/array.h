/////////////////////////////////////////////////////////////////////////////
// Name:        array.h
// Purpose:     dynamic char array to puffering the input data
// Author:      Joachim Buermann
// Id:          $Id: array.h,v 1.1.1.1 2005/05/11 07:40:07 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#ifndef __ARRAY_H
#define __ARRAY_H

class Array
{
protected:
    size_t m_count;
    size_t m_delta;
    size_t m_firstSize;
    size_t m_size;
    char* m_mem;
public:
    Array(size_t size,size_t delta = 0) {
	   Init(size,delta);
    };
    ~Array() {
	   delete m_mem;
    };
    void Clear() {
	   delete m_mem;
	   Init(m_firstSize,m_delta);
    };
    void Add(char* buf,size_t bufsize) {
	   if((m_count + bufsize) > m_size) {
		  // no more room for bufsize, so request more memory
		  char* tmp;
		  if(bufsize > m_delta) {
			 m_size += bufsize;
		  }
		  else {
			 m_size += m_delta;
		  }
		  tmp = new char[m_size];
		  memcpy(tmp,m_mem,m_count);
		  memcpy(&tmp[m_count],buf,bufsize);
		  m_count += bufsize;
		  delete m_mem;
		  m_mem = tmp;
	   }
	   else {
		  memcpy(&m_mem[m_count],buf,bufsize);
		  m_count += bufsize;
	   }
    };
    int Get(char& ch,size_t index) {
	   if(index < m_count) {
		  ch = m_mem[index];
		  return 1;
	   }
	   return 0;
    };
    size_t GetCount() {return m_count;};
    size_t GetSize() {return m_size;};
    void Init(size_t size,size_t delta) {
	   if(size < 512) size = 512;
	   m_size = m_firstSize = size;
	   m_mem = new char[size];
	   memset(m_mem,0,size);
	   if(delta == 0) m_delta = m_size;
	   else m_delta = delta;
	   m_count = 0;
    };
    size_t Read(char* buf,size_t bufsize,size_t& pos) {
	   if(bufsize > (m_count - pos)) {
		  bufsize = m_count - pos;
	   }
	   memcpy(buf,&m_mem[pos],bufsize);
	   pos += bufsize;
	   return bufsize;
    };
};

#endif
