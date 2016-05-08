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

const char *sys_datadir;
const char *usr_cfgdir;
const char *usr_datadir;

char *ART_FILE;
char *BINDINGS_FILE;

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
	String tmp;

#ifdef LINUX_BUILD
	sys_datadir = getenv("SAVAGEWHEELS_SYS_DATADIR");
	if (sys_datadir == NULL)
	{
	    fprintf(stderr, "SAVAGEWHEELS_SYS_DATADIR not set.\n");
	    return 1;
	}
	sys_datadir = strdup(sys_datadir);

	usr_cfgdir = getenv("SAVAGEWHEELS_USR_CONFDIR");
	if (usr_cfgdir == NULL)
	{
	    fprintf(stderr, "SAVAGEWHEELS_USR_CONFDIR not set.\n");
	    return 1;
	}
	usr_cfgdir = strdup(usr_cfgdir);

	usr_datadir = getenv("SAVAGEWHEELS_USR_DATADIR");
	if (usr_datadir == NULL)
	{
	    fprintf(stderr, "SAVAGEWHEELS_USR_DATADIR not set.\n");
	    return 1;
	}
	usr_datadir = strdup(usr_datadir);

	if (sys_datadir == NULL || usr_cfgdir == NULL || usr_datadir == NULL)
	{
	    fprintf(stderr, "Insufficient memory.  Execution aborted.\n");
	    return 1;
	}

	setenv("SDL_VIDEO_CENTERED", "1", 1);
#else
	sys_datadir = usr_cfgdir = usr_datadir = "./";
	_putenv("SDL_VIDEO_CENTERED=1");
#endif
	int len;
	len = snprintf(NULL, 0, "%s/graphics/gfxdata.kdf", sys_datadir);
	if (len < 0)
	{
		fprintf(stderr,
			"Unable to store '%s/graphics/gfxdata.kdf': %s\n",
			sys_datadir, strerror(errno));
		return 1;
	}
	if (len == INT_MAX)
	{
		fprintf(stderr, "Unable to store '%s/graphics/gfxdata.kdf': "
			"Path too log\n", sys_datadir);
		return 1;
	}
	ART_FILE = new (std::nothrow) char[len + 1];
	if (ART_FILE == NULL)
	{
		fprintf(stderr, "Insufficent memory.  Execution aborted.\n");
		return 1;
	}
	sprintf(ART_FILE, "%s/graphics/gfxdata.kdf", sys_datadir);

	len = snprintf(NULL, 0, "%s/bindings.xml", usr_cfgdir);
	if (len < 0)
	{
		fprintf(stderr,
			"Unable to store '%s/bindings.xml': %s\n", usr_cfgdir,
			strerror(errno));
		return 1;
	}
	if (len == INT_MAX)
	{
		fprintf(stderr, "Unable to store '%s/bindings.xml': "
			"Path too log\n", usr_cfgdir);
		return 1;
	}
	BINDINGS_FILE = new (std::nothrow) char[len + 1];
	if (BINDINGS_FILE == NULL)
	{
		fprintf(stderr, "Insufficent memory.  Execution aborted.\n");
		return 1;
	}
	sprintf(BINDINGS_FILE, "%s/bindings.xml", usr_cfgdir);

	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			if (!strncmp(argv[i], "-wnd", 4)) {
				fullscreen = false;
				continue;
			} else if (!strncmp(argv[i], "-sw", 3)) {
				hardware_support = false;
				continue;
			}

			if (!strncmp(argv[i], "-force-fullscreen", 17)
					|| !strncmp(argv[i], "-fullscreen", 11)) {
				fullscreen = true;
				continue;
			}

#ifdef LINUX_BUILD
			if (!strncmp(argv[i], "-snd_alsa", 9)) {
				/*
				 * On the majority Linux distros this seems to be required !
				 */
				setenv("SW_SND_ALSA", "1", 0);
				continue;
			} else if (!strncmp(argv[i], "-snd_22khz", 10)) {
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
	  
	tmp = String(usr_datadir).append("/debug.html");
	OpenLog(tmp.c_str());
	
	CGame game;
	game.Execute(fullscreen, hardware_support);
	game.Close();

	CloseLog();

	return 0;
}
