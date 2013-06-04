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
 * "Savage Wheels" - we're goin' deadly...
 *
 * Copyright (c) 2003-2013 KenamicK Entertainment
 * http://www.kenamick.com/
 *
 * coded by Petar Petrov
 *
 * $Release history
 *
 * Release 1.5:   09.06.2013
 * Release 1.4:   12.09.2010 (Revision started on: 23.04.2005, ended on: 12.09.2010)
 * Patch 1.03:    ??.05.2004
 * Patch 1.02:    ??.03.2004
 * Final Release: 10.02.2004
 * Beta Release:  02.09.2003
 * Dev.Release:   04.08.2003
 * Alpha Release: 29.06.2003
 */

#include "Main.h"

int main( int argc, char *argv[] )
{
	bool hardware_support = true;
	/*
	 * Start the game in a window by default.
	 * The 640x480 resolution we are using is quite small and it really doesn't look good
	 * when the game is played on large screens.
	 * However, users may force fullscreen mode by specifying a command line parameter.
	 *
	 * Linux Note: Running the game in fullscreen would sometimes crash the gfx manager.
	 * This is probably due to some video drivers. I was so far able to reproduce it on
	 * ATI Mobility type of video cards.
	 */
	bool fullscreen = false;

#ifdef LINUX_BUILD
	setenv("SDL_VIDEO_CENTERED", "1", 1);
#else
	_putenv("SDL_VIDEO_CENTERED=1");
#endif

	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			if (!strcmp(argv[i], "-wnd")) {
				fullscreen = false;
				continue;
			} else if (!strcmp(argv[i], "-sw")) {
				hardware_support = false;
				continue;
			}

			if (!strcmp(argv[i], "-force-fullscreen")
					|| !strcmp(argv[i], "-fullscreen")) {
				fullscreen = true;
				continue;
			}

#ifdef LINUX_BUILD
			if (!strcmp(argv[i], "-snd_alsa")) {
				/*
				 * On the majority Linux distros this seems to be required !
				 */
				setenv("SW_SND_ALSA", "1", 0);
				continue;
			} else if (!strcmp(argv[i], "-snd_22khz")) {
				/*
				 * 44KHz somehow seems to be a problem for FMod on Linux
				 */
				setenv("SW_SND_22KHZ", "1", 0);
				continue;
			}
#endif
			perror("Unknown command line parameter passed!");
			exit(0);
		}
	}
	
	/*
	 * Load & Start Game
	 */
	  
	OpenLog("debug.html");
	
	CGame game;
	game.Execute(fullscreen, hardware_support);
	game.Close();

	CloseLog();

	return 0;
}
