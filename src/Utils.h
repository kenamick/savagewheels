// Utils.h

#ifndef __UTILS
#define __UTILS

#define PI		   3.1415926f
#define PI2		   6.2831853f
#define PI_2	   1.5707963f
#define LOG_DASH   " ------------------------------------------------\n"

#ifdef _DEBUG
#	define DBG	LOG
#else
#	define DBG	
#endif

#define LOG( stream_msg ) { OutputSStream ostr; ostr << stream_msg; AppendToLog( ostr.str().c_str() ); }

#define INRANGE(x,y,z) ( (x) >= (y) ? ( (x) <= (z) ? true : false ) : false ) 
//#define INRANGE(x,y,z) ( (z)-(y) > (z)-(x) ? true : false ) 

// global functions prototypes
float	fGetRnd( float min_val, float max_val );
int		intGetRnd( int min_val, int max_val );
//Uint16  GetDistance( int x1, int y1, int x2, int y2 ); 
//float   fGetDistance( float, float, float, float ); 
Uint32  GetDistanceNSR( int x1, int y1, int x2, int y2 ); 
float   fGetDistanceNSR( float, float, float, float ); 
bool	InRange( float val, float bound1, float bound2 );
void	FixAngle( float *angle );

int		OpenLog( const char *filename );
void	AppendToLog( const char *dbgstring );
//void	AppendToMultilog( char *dbgstring, ... );
void	CloseLog(void);

String	ExtractFilename( const String strPath );
bool	PathExists( const String strPath, struct stat* _pStats = NULL );

#endif