/*
    Copyright (c) 2003-2010 KenamicK Entertainment

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

*/

#include "Main.h"

#define READ_OK(res) //if (0 == (res)) throw std::exception("Failed to read from buffered stream !")

CBufferedReader::CBufferedReader(FILE* fp)
: _fpSource(fp)
{
//   if ( NULL == fp )
//     throw std::exception("Null reference to Stream passed!");
}

CBufferedReader::~CBufferedReader()
{

}

char CBufferedReader::readChar()
{
  char c;
  READ_OK( fread( &c, sizeof(char), 1, _fpSource ) );

  return c;
}

unsigned char CBufferedReader::readUChar()
{
  unsigned char c;
  READ_OK( fread( &c, sizeof(unsigned char), 1, _fpSource ) );

  return c;
}

void CBufferedReader::readCharArray(char* dest, int size)
{
  READ_OK( fread( dest, sizeof(char) * size, 1, _fpSource ) );
}

void CBufferedReader::readUCharArray(unsigned char* dest, int size)
{
  READ_OK( fread( dest, sizeof(unsigned char) * size, 1, _fpSource ) );
}

short int CBufferedReader::readInt16()
{
  short n;
  READ_OK( fread( &n, sizeof(char) * 2, 1, _fpSource ) );

  return n;
}

int CBufferedReader::readInt32()
{
  int n;
  READ_OK( fread( &n, sizeof(char) * 4, 1, _fpSource ) );

  return n;
}

