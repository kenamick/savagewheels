// mciAVI.cpp -


#include "mciAvi.h"



///////////////////////////////////////////////////////////////////////////////////////////////
//// Name: Initialize()
//// Desc: Init MCI player
///////////////////////////////////////////////////////////////////////////////////////////////
int CAvi::Initialize()
{

	MCIERROR rval = mciSendString( OPEN_AVI_VIDEO, NULL, NULL, NULL );
	if ( rval ) 
		return ShowErrorString( rval );

	bInitialized = true;

	return 1;
}


///////////////////////////////////////////////////////////////////////////////////////////////
//// Name: Open()
//// Desc: open a movie file with given alias
///////////////////////////////////////////////////////////////////////////////////////////////
int CAvi::Open( HWND hwnd, LPSTR avifile, LPSTR avialias )
{
    
	MCIERROR rval;
	char	 mcicommand[255];

	// open avi-file with given alias
	sprintf( mcicommand, MOV_OPEN_FORMAT, avifile, avialias, hwnd );
	rval = mciSendString( mcicommand, NULL, NULL, NULL );
	if ( rval ) 
		return ShowErrorString( rval );
	
	// realize the palette
	sprintf( mcicommand, "realize %s", avialias );
	rval = mciSendString( mcicommand, NULL, NULL, NULL );
   
  return 1;
}



///////////////////////////////////////////////////////////////////////////////////////////////
//// Name: Play()
//// Desc: play a movie file with the given alias
///////////////////////////////////////////////////////////////////////////////////////////////
int CAvi::Play( LPSTR avialias, enumPLAY_MODE playmode )
{
	MCIERROR rval;
	char	 mcicommand[255];

	switch ( playmode )
	{
	case PM_NOTIFY:  // play from pause
		sprintf( mcicommand, MOV_FORWARD, avialias );
		rval = mciSendString( mcicommand, NULL, NULL, NULL );
		if ( rval ) 
			return ShowErrorString( rval );
		break;

	case PM_FROMSTART:  // play from the start
		sprintf( mcicommand, MOV_SEEK_TO_START, avialias );
		rval = mciSendString( mcicommand, NULL, NULL, NULL );
		sprintf( mcicommand, MOV_FORWARD, avialias );
		rval = mciSendString( mcicommand, NULL, NULL, NULL );
		if ( rval ) 
			return ShowErrorString( rval );
		break;

	case PM_FROMEND:  // play backwards
		sprintf( mcicommand, MOV_REVERSE, avialias );
		rval = mciSendString( mcicommand, NULL, NULL, NULL );
		if ( rval ) 
			return ShowErrorString( rval );
		break;
	}

	return 1;
}


///////////////////////////////////////////////////////////////////////////////////////////////
//// Name: Pause()
//// Desc: 
///////////////////////////////////////////////////////////////////////////////////////////////
int CAvi::Pause( LPSTR avialias )
{
	MCIERROR rval;
	char	 mcicommand[255];

	sprintf( mcicommand, MOV_PAUSE, avialias );
    rval = mciSendString( mcicommand, NULL, NULL, NULL );
	
	return 1;
}


///////////////////////////////////////////////////////////////////////////////////////////////
//// Name: GetWindow()
//// Desc: Get window handle
///////////////////////////////////////////////////////////////////////////////////////////////
HWND CAvi::GetWindow( LPSTR avialias )
{
	MCIERROR rval;
	char	 mcicommand[255];
	char     lpstrwindow[255];
	long	 lwindow;


	sprintf( mcicommand, MOV_HANDLE, avialias );
	rval = mciSendString( mcicommand, lpstrwindow, 255, NULL );

	if ( !rval )
	{
		lwindow = atol( lpstrwindow );
		return (HWND)lwindow;
	}

	ShowErrorString( rval );
	
	return (HWND)-1L;
}


///////////////////////////////////////////////////////////////////////////////////////////////
//// Name: GetRect()
//// Desc: Get movie dimensions
///////////////////////////////////////////////////////////////////////////////////////////////
void CAvi::GetRect( LPSTR avialias, RECT *rect )
{
	MCIERROR rval;
	char	 mcicommand[255];
	char     lpstrrect[255];
	int		 x, y, w, h;

	sprintf( mcicommand, MOV_SOURCE, avialias );
	rval = mciSendString( mcicommand, lpstrrect, 255, NULL );
	if ( rval ) 
	{
		ShowErrorString( rval );
		return;
	}

	// get rect dimensions
	sscanf( lpstrrect, "%d %d %d %d", &x, &y, &w, &h );

	rect->left = x;
	rect->top = y;
	rect->right = w;
	rect->bottom = h;
}


///////////////////////////////////////////////////////////////////////////////////////////////
//// Name: CenterWindow()
//// Desc: Center movie window
///////////////////////////////////////////////////////////////////////////////////////////////
void CAvi::CenterWindow( LPSTR avialias, int screen_width, int screen_height, int width, int height )
{
	HWND	 hwnd;
	RECT	 rect;
	int	     x,y;

	GetRect( avialias, &rect );
	
	if ( hwnd = GetWindow( avialias ) )
	{
		if ( !width ) width = rect.right;
		if ( !height ) height = rect.bottom;

		x = (screen_width >> 1) - (width >> 1);
		y = (screen_height >> 1) - (height >> 1);

		MoveWindow( hwnd, x, y, width, height, true );
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
//// Name: Close()
//// Desc: close alias
///////////////////////////////////////////////////////////////////////////////////////////////
void CAvi::Close( LPSTR avialias )
{
	MCIERROR rval;
	char	 mcicommand[255];

	sprintf( mcicommand, MOV_CLOSE, avialias );
	rval = mciSendString( mcicommand, NULL, NULL, NULL );
	if ( rval ) 
		ShowErrorString( rval );
}

///////////////////////////////////////////////////////////////////////////////////////////////
//// Name: ShowErrorString()
//// Desc: Pop up error box
///////////////////////////////////////////////////////////////////////////////////////////////
int CAvi::ShowErrorString( MCIERROR errval )
{
	char lpstrError[255];
	mciGetErrorString( errval, lpstrError, 255 );

	MessageBox( NULL, lpstrError, "Unexpected error", MB_OK | MB_ICONEXCLAMATION );

	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////
//// Name: Release()
//// Desc: Release MCI
///////////////////////////////////////////////////////////////////////////////////////////////
void CAvi::Release()
{
	MCIERROR rval = mciSendString( CLOSE_AVI_VIDEO, NULL, NULL, NULL );
	if ( rval ) 
		ShowErrorString( rval );

	bInitialized = false;
}




