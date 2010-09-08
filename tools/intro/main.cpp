// main.cpp


#define WIN32_LEAN_AND_MEAN
#define CLASS_NAME "Starting"

#include <windows.h>
#include <shellapi.h>
#include "mciAvi.h"


CAvi classavi;
bool bEsc = false;


LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{

	switch( msg )
	{
	case WM_SYSKEYUP:
		// if ( wparam == VK_TAB ) 
		break;

	case WM_KEYDOWN:

		if ( wparam == VK_ESCAPE ) 
		{
			/*PostMessage( hwnd, WM_CLOSE, 0U, 0U );*/
			bEsc = true;
			return 0L;
		}

		if ( wparam == VK_RETURN )
		{
			/*
			MessageBox( NULL, "play", NULL, MB_OK | MB_ICONEXCLAMATION );
			classavi.Initialize();
			classavi.Open( hwnd, "graphics/logo.avi", "mov1" );
			classavi.CenterWindow( "mov1", 640, 480 );
			classavi.Play( "mov1", PM_NOTIFY );*/

		}
		
		break;

	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0L;
		break;

	default:
		return DefWindowProc( hwnd, msg, wparam, lparam );
		break;

	}

	return 0L;
}


// Fulscreen mode
bool ChangeToFullScreen( int width, int height, int bits )
{

      DEVMODE dmScreenSettings;                                 // Device Mode
      memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));   // Makes Sure Memory's Cleared
      dmScreenSettings.dmSize       = sizeof(dmScreenSettings); // Size Of The Devmode Structure
      dmScreenSettings.dmPelsWidth  = width;                    // Selected Screen Width
      dmScreenSettings.dmPelsHeight = height;                   // Selected Screen Height
      dmScreenSettings.dmBitsPerPel = bits;                     // Selected Bits Per Pixel
      dmScreenSettings.dmFields     = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

      // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
      if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) 
	  {
                return false; // Return false
      }

	  return true;

}




long getfilesize( FILE *fp )
{

	long f_size;

	fseek( fp, 0, SEEK_END );
	f_size = ftell( fp );

	fseek( fp, 0, SEEK_SET );

	return f_size;

}
	


bool ModifyHeader( const char *file, bool bForPlay )
{
	char h_riff[] = { 'R', 'I', 'F', 'F' };
	char h_kk[] = { 'K', 'V', 'I', 'D' };
	char *buf;
	FILE *fp;
	int fsize;


	if ( bForPlay )  // set to RIFF
	{
		if ( fp = fopen( file, "rb+" ) )
		{

			fsize = getfilesize( fp );
			// try to allocate mem
			if ( ( buf = new char[fsize] ) == NULL )
				return false;

			fread( buf, fsize, 1, fp );
			strncpy( buf, h_riff, 4 );
			fseek( fp, 0, SEEK_SET );
			fwrite( buf, fsize, 1, fp );			
			fclose( fp );

			return true;
		}
		else
			return false;
	}
	else		  // set to KVID
	{
		if ( fp = fopen( file, "rb+" ) )
		{

			fsize = getfilesize( fp );
			// try to allocate mem
			if ( ( buf = new char[fsize] ) == NULL )
				return false;

			fread( buf, fsize, 1, fp );
			strncpy( buf, h_kk, 4 );
			fseek( fp, 0, SEEK_SET );
			fwrite( buf, fsize, 1, fp );
			fclose( fp );

			return true;
		}
		else
			return false;


	}

}




int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

	WNDCLASS wc;
	HWND	 hwnd;
	MSG		 msg;
	DWORD    nowt;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = (HCURSOR)LoadCursor( hInstance, IDC_ARROW );
	wc.hIcon = (HICON)LoadIcon( hInstance, IDI_WINLOGO );
	wc.hInstance = hInstance;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if ( !RegisterClass( &wc ) )
	{
		MessageBox( NULL, "Error initializing class", NULL, MB_OK );
		return -1;
	}

	hwnd = CreateWindowEx(					// create our window 
		WS_EX_APPWINDOW,
        CLASS_NAME,
		"SDK_WINDOW",
		WS_POPUP,
		0,
		0,
		640,
		480,
		NULL,
		NULL,
		hInstance,
		NULL );

	ShowWindow( hwnd, nCmdShow );
	UpdateWindow( hwnd );
	SetFocus( hwnd );

	char avi_name[] = "mov_sw";


	if ( !ChangeToFullScreen( 640, 480, 16 ) )
		goto r_end;

  
	if ( !ModifyHeader( "graphics/logo.avi", true ) )
		goto r_end;

	Sleep( 2000 );

	if ( classavi.Initialize() )
	{
	
		if ( !classavi.Open( hwnd, "graphics/logo.avi", avi_name ) )
			goto r_end;

		//classavi.CenterWindow( avi_name, 640, 480 );
		
		if ( !classavi.Play( avi_name, PM_NOTIFY ) )
			goto r_end;
	}
	else
		goto r_end;


	nowt = GetTickCount();
	ShowCursor( false );

	for(;;)
	{
		
		if (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE)) 
		{
			if (!GetMessage(&msg, NULL, 0, 0)) 
				return msg.wParam;

			TranslateMessage(&msg);	
		    DispatchMessage(&msg);
		}

		if ( GetTickCount() > nowt +  13250 || bEsc )
			break;
	}

	ShowCursor( true );
	ModifyHeader( "graphics/logo.avi", false );

r_end:
	classavi.Close( avi_name );
	classavi.Release();
	ShellExecute( NULL, "open", "sw.exe", NULL, NULL, SW_SHOWNORMAL);
 
	return 0L;

}
