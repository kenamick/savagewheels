/*
    Copyright (c) 2003-2013 KenamicK Entertainment

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

  Copyright (c) 2003-2013 KenamicK Entertainment
  http://www.kenamick.com/

  coded by Peter Petrov

  // release history
  Alpha Release: 29.06.2003
  Dev.Release:   04.08.2003
  Beta Release:  02.09.2003
  Final Release: 10.02.2004
  Patch 1.02:    ??.03.2004
  Patch 1.03:    ??.05.2004
  Release 1.4:   12.09.2010 / Started Revision: 23.04.2005 / Ended Revision: 12.09.2010
  Release 1.5:   ??.??.2013
  

*/

#include "Main.h"

int main( int argc, char *argv[] )
{
	bool fullscreen = true, hardware_support = true;

#ifdef LINUX_BUILD
	// SDL environment vars
	setenv("SDL_VIDEO_CENTERED", "1", 1);
	fullscreen = false;	// on some video drivers fullscreen seems to crash the gfx manager
#else
	_putenv("SDL_VIDEO_CENTERED=1");
#endif

	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			if (!strcmp(argv[i], "-wnd"))
				fullscreen = false;
			else if (!strcmp(argv[i], "-sw"))
				hardware_support = false;
			else if (!strcmp(argv[i], "-force-fullscreen"))
				fullscreen = true;
			else if (!strcmp(argv[i], "-snd_alsa"))
				setenv("SW_SND_ALSA", "1", 0); // on the majority Linux distros this seems to be required !
			else if (!strcmp(argv[i], "-snd_22khz"))
				setenv("SW_SND_22KHZ", "1", 0); // 44KHz somehow seems to be a problem for FMod on Linux
			else {
				perror("Unknown command line parameter passed!");
				exit(0);
			}
		}
	}
	
	// Start Game
	  
	OpenLog( "debug.html" );
	
	CGame game;
	game.Execute(fullscreen, hardware_support);
	game.Close();

	CloseLog();

	return 0;
}
