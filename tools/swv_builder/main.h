// main.h - 

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
long getfilesize(FILE *fp);

#endif