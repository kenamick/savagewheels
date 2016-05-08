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

#ifdef LINUX_BUILD
#include <sys/timeb.h>
#include <time.h>
#define _ftime	ftime
struct timeb   time_struct;
#else /*  LINUX_BUILD */
struct _timeb  time_struct;
#endif  /*  LINUX_BUILD */

static std::ofstream debug_file;


///////////////////////////////////////////////////////////////////////
// Name: FixAngle()
// Desc: angle E{ 0, PI*2 ) (RAD)
///////////////////////////////////////////////////////////////////////
void FixAngle(float *angle)
{
    float myangle	= *angle;
    bool  bfixed	= false;

    while ( !bfixed )
    {
        if ( myangle > PI2 )
        {
            myangle -= PI2;
        }
        else if ( myangle < 0.0f )
        {
            myangle += PI2;
        }
        else
        {
            bfixed = true;
        }
    }

    *angle = myangle;
}

///////////////////////////////////////////////////////////////////////
// Name: Rad2Deg()
// Desc: Keeps radians in the range of (-PI, PI)
///////////////////////////////////////////////////////////////////////
float FixRad(float rad)
{
	float fixed_rad = rad;

	fixed_rad = fixed_rad > PI ? fixed_rad - PI2 : fixed_rad;
	fixed_rad = fixed_rad < -PI ? PI2 + fixed_rad : fixed_rad;

	return fixed_rad;
}

///////////////////////////////////////////////////////////////////////
// Name: Rad2Deg()
// Desc: Convert radians (-PI, PI) to degress (0, 360)
///////////////////////////////////////////////////////////////////////
float Rad2Deg(float rad)
{
	float fixed_rad = rad > 0.0f ? rad : (PI2 + rad);

	return fixed_rad * RAD1;
}

///////////////////////////////////////////////////////////////////////
// Name: Deg2Rad()
// Desc: Convert degress (0, 360) to radians (-PI, PI)
///////////////////////////////////////////////////////////////////////
float Deg2Rad(float deg)
{
	deg = deg < 0.0 ? 360.0f - deg : deg;
	deg = deg > 360.0f ? deg - 360.0f : deg;

	float fixed_rad = deg * DEG1;
	fixed_rad = fixed_rad > PI ? fixed_rad - PI2 : fixed_rad;
	fixed_rad = fixed_rad < -PI ? PI2 + fixed_rad : fixed_rad;

	return fixed_rad;
}

///////////////////////////////////////////////////////////////////////
// Name: intGetRnd()
// Desc:
///////////////////////////////////////////////////////////////////////
int intGetRnd ( int min_val, int max_val )
{
    int range = max_val - min_val;
    int num = rand() % range;
    return ( num + min_val );
}


///////////////////////////////////////////////////////////////////////
// Name: fGetRnd()
// Desc:
///////////////////////////////////////////////////////////////////////
float fGetRnd ( float min_val, float max_val )
{
    return ( ( max_val - min_val ) * ( float ) rand() / ( float ) RAND_MAX ) + min_val;
}

/////////////////////////////////////////////////////////////////////////
//// Name: GetDistance()
//// Desc: (Uint)
/////////////////////////////////////////////////////////////////////////
//Uint16  GetDistance( int x1, int y1, int x2, int y2 )
//{
//	int dx = x2 - x1;
//	int dy = y2 - y1;
//	float product = (float)( dx*dx + dy*dy );
//
//	return (Uint16)sqrt( product );
//}

/////////////////////////////////////////////////////////////////////////
//// Name: fGetDistance()
//// Desc: (FLOAT)
/////////////////////////////////////////////////////////////////////////
//float  fGetDistance( float x1, float y1, float x2, float y2 )
//{
//	float dx = x2 - x1;
//	float dy = y2 - y1;
//
//	return sqrt( (float)( dx*dx + dy*dy ) );
//}

///////////////////////////////////////////////////////////////////////
// Name: GetDistanceNSR()
// Desc: Get unsquared dance (int)
///////////////////////////////////////////////////////////////////////
Uint32 GetDistanceNSR ( int x1, int y1, int x2, int y2 )
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    return (Uint32)(dx*dx + dy*dy);
}

///////////////////////////////////////////////////////////////////////
// Name: GetDistanceNSR()
// Desc: Get unsquared dance (float)
///////////////////////////////////////////////////////////////////////
float fGetDistanceNSR ( float x1, float y1, float x2, float y2 )
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    return (dx*dx + dy*dy);
}

///////////////////////////////////////////////////////////////////////
// Name: InRange()
// Desc: check if value is between given range
///////////////////////////////////////////////////////////////////////
bool InRange ( float val, float rangeMin, float rangeMax )
{
    if ( rangeMin > rangeMax )
    {
        if ( val <= rangeMin && val >= rangeMax ) return true;
    }
    else if ( rangeMin < rangeMax )
    {
        if ( val <= rangeMax && val >= rangeMin ) return true;
    }

    return false;
}


///////////////////////////////////////////////////////////////////////
// Name: fRangeGetXY()
// Desc: Convert one range to another
///////////////////////////////////////////////////////////////////////
float fRangeGetXY(float in, float inMin, float inMax, float min, float max)
{
    float inRange = (inMax - inMin);
    float newRange = (max - min);
    float result = (((in - inMin) * newRange) / inRange) + min;

    return result;
}

///////////////////////////////////////////////////////////////////////
// Name: fRangeGetXY()
// Desc: Convert one range to another (INT input)
///////////////////////////////////////////////////////////////////////
int	RangeGetXY(int in, int inMin, int inMax, int min, int max)
{
    return (int)fRangeGetXY((float)in, (float)inMin, (float)inMax, (float)min, (float)max);
}

///////////////////////////////////////////////////////////////////////
// Name: fRangeGet0255()
// Desc:
///////////////////////////////////////////////////////////////////////
bool	fIsZero(float value)
{
	return fabsf(value) < MIN_FLOAT;
}

///////////////////////////////////////////////////////////////////////
// Name: GetFormattedTime()
// Desc:
///////////////////////////////////////////////////////////////////////
inline String GetFormattedTime()
{
    char buf[255];

#ifdef LINUX_BUILD
    time_t cur_time;
    tm *ptm = NULL;

    time ( &cur_time );
    ptm = localtime( &cur_time );

    sprintf ( buf, "%d:%d:%d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec );
    //sprintf ( buf1, "%s", ctime( &cur_time ) );
#else
    _strtime ( buf );
#endif

    _ftime ( &time_struct );
    sprintf ( buf, "%s.%.3u ", buf, time_struct.millitm );

    return buf;
}

///////////////////////////////////////////////////////////////////////
// Name: OpenLog()
// Desc: open global log file
///////////////////////////////////////////////////////////////////////
bool OpenLog ( const char* filename )
{
    String time( GetFormattedTime() );

    // open debug file
    debug_file.open ( filename, std::ios::out ); //ios::ate );
    if ( ! debug_file.good() )
      return false;

    debug_file << "<html><head><title>Savage Wheels Log File</title></head><body><h1>Savage Wheels V" << VER_MAJ << "." << VER_MIN << " - Log File</h1>";
    debug_file << "<hr/><pre>";
    debug_file <<  time << "Build: " << APP_NAME << " <br/>";
    debug_file <<  time << "Copyright &copy; 2003-2013 KenamicK Entertainment <br />";
    debug_file <<  time << "Opened on: " << __DATE__ << "<br />";
    debug_file <<  time << "Opened at: " << __TIME__ << "<br />";
    debug_file <<  time << LOG_DASH << "<br />";

    return true;
}

///////////////////////////////////////////////////////////////////////
// Name: AppendToLog()
// Desc: add a line to global log file
///////////////////////////////////////////////////////////////////////
void AppendToLog ( const char *dbgstring )
{
    String time( GetFormattedTime() );
    debug_file << time << dbgstring << "\n";

    // XXX: Flushing every time is brutally slow but helps
    //      against losing log info if game suddenly crashes !
    debug_file.flush();
}

/////////////////////////////////////////////////////////////////////////
//// Name: AppendToMultilog()
//// Desc: dobavq nqkolko niz-a kym Log-a
/////////////////////////////////////////////////////////////////////////
//void AppendToMultilog( char *dbgstring, ... )
//{
//	char *strnext = dbgstring;
//	va_list arg_list;
//	char buf1[64];
//
//#ifdef LINUX_BUILD
//	time_t cur_time;
//	time( &cur_time );
//	sprintf( buf1, "%s", ctime( &cur_time ) );
//#else
//	_strtime( buf1 );
//#endif
//
//	_ftime( &time_struct );
//	sprintf( buf1, "%s.%.3u", buf1, time_struct.millitm );
//
//	debug_file << buf1 << " ";
//
//	va_start( arg_list, dbgstring );
//
//	while ( strnext != NULL )
//	{
//		debug_file << strnext;
//		strnext = va_arg( arg_list, char* );
//	}
//
//	va_end( arg_list );
//
//	debug_file << "\n";
//
//	debug_file.flush();
//
//}

///////////////////////////////////////////////////////////////////////
// Name: CloseLog()
// Desc: close global log file
///////////////////////////////////////////////////////////////////////
void CloseLog ( void )
{
    AppendToLog ( "Game closed." );
    debug_file << "\n</pre></body></html>";
    debug_file.close();
}

///////////////////////////////////////////////////////////////////////
// Name: ExtractFilename()
// Desc:
///////////////////////////////////////////////////////////////////////
String ExtractFilename ( const String strPath )
{
    String strResult ( strPath );
    String::size_type idx = strResult.find_last_of ( ANY_BACKSLASH );
    if ( idx != String::npos )
    {
        strResult.erase ( 0, idx + 1 );
    }

    return strResult;
}

///////////////////////////////////////////////////////////////////////
// Name: PathExists()
// Desc:
///////////////////////////////////////////////////////////////////////
bool PathExists ( const String strPath, struct stat* _pStats /*= NULL*/ )
{
//#ifndef LINUX_BUILD
#if 0
    if ( INVALID_FILE_ATTRIBUTES != ::GetFileAttributesA ( strPath.c_str() ) )
    {
        return true;
    }
    else
    {
        DBG ( "Failed to find file %s! lasterror=%d", strPath.c_str(), GetLastError(), NULL );
    }
#else
    if ( _pStats )
    {
        //pStats = _pStats;
        if ( -1 != stat( strPath.c_str(), _pStats ) )
            return true;
    }
    else
    {
        struct stat _stats;

        if ( -1 != stat( strPath.c_str(), &_stats ) )
            return true;
    }
#endif

    return false;
}
