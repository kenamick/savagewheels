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

#ifndef __MAIN_H__
#define __MAIN_H__

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>

#include "Cswv_module.h" 

using namespace std;

// Vehicles
enum CONST_VEHICLES
{
	CV_BMW = 0,
	CV_MERCEDES,
	CV_DIABLO,
	CV_DODGE,
	CV_DJUGAN,
	CV_JEEP,
	CV_F1,
	CV_DTRUCK
};

// prototypes
int create_swv_module(SWV_HEADER *swv);
int populate_swv_module(CONST_VEHICLES vehicle, SWV_HEADER *phdr);
int test_car(CONST_VEHICLES vehicle);
int32_t getfilesize(FILE *fp);

#endif