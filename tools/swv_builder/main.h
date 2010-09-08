// main.h - 

#ifndef __MAIN_H__
#define __MAIN_H__

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>

#include "Cswv_module.h" 

using namespace std;

// prototypes
int create_swv_module(SWV_HEADER *swv);
void test_car(int idx);
long getfilesize(FILE *fp);

#endif