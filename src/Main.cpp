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
