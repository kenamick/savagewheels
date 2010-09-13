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
#	include <sys/timeb.h>
#	include <time.h>

#	define _ftime	ftime

struct timeb   time_struct;
#else
struct _timeb  time_struct;
#endif

static std::ofstream	   debug_file;



///////////////////////////////////////////////////////////////////////
// Ime: GetFormattedTime()
// Opisanie:
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
// Ime: FixAngle()
// Opisanie: angle E{ 0, PI*2 ) (RAD)
///////////////////////////////////////////////////////////////////////
void FixAngle ( float *angle )
{
    float myangle	= *angle;
    bool  bfixed	= false;

    while ( !bfixed )
    {
        if ( myangle > PI2 )
        {
            myangle -= ( float ) PI2;
        }
        else if ( myangle < 0 )
        {
            myangle += ( float ) PI2;
        }
        else
            bfixed = true;
    }

    *angle = myangle;
}


///////////////////////////////////////////////////////////////////////
// Ime: intGetRnd()
// Opisanie: vryshta prozivolno cqlo chislo
///////////////////////////////////////////////////////////////////////
int intGetRnd ( int min_val, int max_val )
{
    int range = max_val - min_val;
    int num = rand() % range;
    return ( num + min_val );
}


///////////////////////////////////////////////////////////////////////
// Ime: fGetRnd()
// Opisanie:
///////////////////////////////////////////////////////////////////////
float fGetRnd ( float min_val, float max_val )
{
    return ( ( max_val - min_val ) * ( float ) rand() / ( float ) RAND_MAX ) + min_val;
}


/////////////////////////////////////////////////////////////////////////
//// Ime: GetDistance()
//// Opisanie: Vryshta razstoqnieto m/u 2 tochki
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
//// Ime: fGetDistance()
//// Opisanie: Vryshta razstoqnieto m/u 2 tochki (FLOAT)
/////////////////////////////////////////////////////////////////////////
//float  fGetDistance( float x1, float y1, float x2, float y2 )
//{
//	float dx = x2 - x1;
//	float dy = y2 - y1;
//
//	return sqrt( (float)( dx*dx + dy*dy ) );
//}


///////////////////////////////////////////////////////////////////////
// Ime: GetDistanceNSR()
// Opisanie: (INT)
///////////////////////////////////////////////////////////////////////
Uint32  GetDistanceNSR ( int x1, int y1, int x2, int y2 )
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    return ( Uint32 ) ( dx*dx + dy*dy );
}


///////////////////////////////////////////////////////////////////////
// Ime: GetDistanceNSR()
// Opisanie: (FLOAT)
///////////////////////////////////////////////////////////////////////
float  fGetDistanceNSR ( float x1, float y1, float x2, float y2 )
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    return ( dx*dx + dy*dy );
}


///////////////////////////////////////////////////////////////////////
// Ime: InRange()
// Opisanie: proverqwa dali dadeno chislo e m/u 2 drugi
///////////////////////////////////////////////////////////////////////
bool InRange ( float val, float bound1, float bound2 )
{
    if ( bound1 > bound2 )
    {
        if ( val <= bound1 && val >= bound2 ) return true;
    }
    else if ( bound1 < bound2 )
    {
        if ( val <= bound2 && val >= bound1 ) return true;
    }

    return false;
}


///////////////////////////////////////////////////////////////////////
// Ime: OpenLog()
// Opisanie: otvarq Log file-a
///////////////////////////////////////////////////////////////////////
int OpenLog ( const char* filename )
{
    String time( GetFormattedTime() );

    // open debug file
    debug_file.open ( "debug.html", std::ios::out ); //ios::ate );
    if ( ! debug_file.good() )
      return 0;

    debug_file << "<html><head><title>Savage Wheels Log File</title></head><body><h1>Savage Wheels V" << VER_MAJ << "." << VER_MIN << " - Log File</h1>";
    debug_file << "<hr/><pre>";
    debug_file <<  time << "Build: " << APP_NAME << " <br/>";
    debug_file <<  time << "Copyright &copy; 2003-2010 KenamicK Entertainment <br />";
    debug_file <<  time << "Opened on: " << __DATE__ << "<br />";
    debug_file <<  time << "Opened at: " << __TIME__ << "<br />";
    debug_file <<  time << LOG_DASH << "<br />";

    return 1;
}


///////////////////////////////////////////////////////////////////////
// Ime: AppendToLog()
// Opisanie: dobavq red kym Log-a
///////////////////////////////////////////////////////////////////////
void AppendToLog ( const char *dbgstring )
{
    String time( GetFormattedTime() );
    debug_file << time << dbgstring << "\n";
    debug_file.flush();
}


/////////////////////////////////////////////////////////////////////////
//// Ime: AppendToMultilog()
//// Opisanie: dobavq nqkolko niz-a kym Log-a
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
// Ime: CloseLog()
// Opisanie: zatvarq Log file
///////////////////////////////////////////////////////////////////////
void CloseLog ( void )
{
    AppendToLog ( "Game closed." );
    debug_file << "\n</pre></body></html>";
    debug_file.close();
}


///////////////////////////////////////////////////////////////////////
// Ime: ExtractFilename()
// Opisanie:
///////////////////////////////////////////////////////////////////////
String ExtractFilename ( const String strPath )
{
    String strResult ( strPath );

#define ANY_BACKSLASH         "/\\"

    String::size_type idx = strResult.find_last_of ( ANY_BACKSLASH );
    if ( idx != String::npos )
    {
        strResult.erase ( 0, idx + 1 );
    }

    return strResult;
}


///////////////////////////////////////////////////////////////////////
// Ime: PathExists()
// Opisanie:
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
        if ( -1 != ::stat ( strPath.c_str(), _pStats ) )
            return true;
    }
    else
    {
        struct stat _stats;

        if ( -1 != stat ( strPath.c_str(), &_stats ) )
            return true;
    }

#endif

    return false;
}
