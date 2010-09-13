// mciAVI.h -

/*

  MCI Movie module

  coded by Peter "Pro-XeX" Petrov
  
  ( any changes should be described below )
  changes:
    - 

*/


#ifndef __AVIPLAYER_INCLUDED
#define __AVIPLAYER_INCLUDED


#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>


#define AVI_VIDEO "avivideo"
#define OPEN_AVI_VIDEO "open avivideo"
#define CLOSE_AVI_VIDEO "close avivideo"
#define MOV_REALIZE "realize %s"
#define MOV_SEEK_TO_START "seek %s to start"
#define MOV_SEEK_TO_END "seek mov to end"
#define MOV_CLOSE "close %s"
#define MOV_SOURCE "where %s source"
#define MOV_OPEN_FORMAT "open %s type %s alias %s style child parent %d"
//#define MOV_OPEN_FORMAT "open %s alias %s style child parent %d"
#define MOV_SHOW "window %s state show"
#define MOV_HANDLE "status %s window handle"
#define MOV_REVERSE "play %s reverse notify"
#define MOV_FORWARD "play %s notify"
//#define MOV_FORWARD "play %s fullscreen"
#define MOV_PAUSE "pause %s"
#define MOV_FORWARD_STEP "step %s by 1"
#define MOV_REVERSE_STEP "step %s reverse by 1"


enum enumPLAY_MODE
{
	PM_NOTIFY,
	PM_FROMSTART,
	PM_FROMEND
};


class CAvi
{

private:
	
	bool bInitialized;

	int ShowErrorString( MCIERROR errval );
	void GetRect( LPSTR avialias, RECT *rect );

public:
	CAvi() 
		: bInitialized(false)
	{};
	~CAvi() { Release(); };

	int Initialize();
	int Open( HWND hwnd, LPSTR avifile, LPSTR avialias );
	int Play( LPSTR avialias, enumPLAY_MODE playmode );
	int	Pause( LPSTR avialias );
	HWND GetWindow( LPSTR avialias );
	void CenterWindow( LPSTR avialias, int screen_width, int screen_height, int width = 0, int height = 0 );
	void Close( LPSTR avialias );
	void Release();


};


#endif
