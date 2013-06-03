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

#ifndef __UTILS
#define __UTILS

#define PI			3.1415926f
#define PI2			6.2831853f
#define PI_2		1.5707963f
#define RAD1		57.2957795786f
#define DEG1		0.0174532925f
// XXX .00001f or FLT_EPSILON 
#define MIN_FLOAT	0.00000001f

#define LOG_DASH   "------------------------------------------------"
#define ANY_BACKSLASH         "/\\"
#define LOG( stream_msg ) { OutputSStream ostr; ostr << stream_msg; AppendToLog( ostr.str().c_str() ); }

#ifdef _DEBUG
#	define DBG(x)	LOG("DBG: " << x)
#else
#	define DBG(x)	
#endif

#define INRANGE(x,y,z) ( (x) >= (y) ? ( (x) <= (z) ? true : false ) : false ) 
//#define INRANGE(x,y,z) ( (z)-(y) > (z)-(x) ? true : false ) 

void FixAngle(float *angle);
float FixRad(float rad);
float Rad2Deg(float rad);
float Deg2Rad(float deg);
float fGetRnd(float min_val, float max_val);
int intGetRnd(int min_val, int max_val);
//Uint16  GetDistance( int x1, int y1, int x2, int y2 ); 
//float   fGetDistance( float, float, float, float ); 
Uint32 GetDistanceNSR(int x1, int y1, int x2, int y2);
float fGetDistanceNSR(float, float, float, float);
bool InRange(float val, float bound1, float bound2);
float fRangeGetXY(float in, float inMin, float inMax, float min, float max);
int RangeGetXY(int in, int inMin, int inMax, int min, int max);
bool fIsZero(float value);

bool OpenLog(const char *filename);
void AppendToLog(const char *dbgstring);
//void	AppendToMultilog( char *dbgstring, ... );
void CloseLog(void);
String ExtractFilename(const String strPath);
bool PathExists(const String strPath, struct stat* _pStats = NULL);

#endif
