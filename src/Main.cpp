/*
  "Savage Wheels" - we're goin' deadly...

  Copyright (c) 2003-2010 KenamicK Entertainment
  http://www.kenamick.com/

  coded by Peter "necroleak" Petrov

  // release history
  Alpha Release: 29.06.2003
  Dev.Release:   04.08.2003
  Beta Release:  02.09.2003
  Final Release: 10.02.2004
  Patch 1.02:    ??.03.2004
  Patch 1.03:    ??.05.2004

  //TODO:
  Started Revision: 23.04.2005
  Ended Revision: 
  Patch 1.04:    ??.??.2007

*/

#include "Main.h"

int main( int argc, char *argv[] )
{
	OpenLog( "debug.html" );
	
	CGame game;
	
	//game.Bindings.Load( "bindings.xml" );
	//CloseLog();
	//return 1;

#if 0
	if ( argc > 1 )
	{
		if ( !strcmp( argv[1], "-wnd" ) )
			game.Execute();
		else if ( !strcmp( argv[1], "-hw" ) )
			game.Execute( true, true );
	}
	else
		game.Execute(true); //{!}
#endif

	game.Execute( false, true );
		
	game.Close();

	CloseLog();

	return 0;
}
