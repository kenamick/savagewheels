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

#ifndef __CSOUNDS_INCLUDED
#define __CSOUNDS_INCLUDED


#define NUM_SOUNDS 12
#define NUM_MUSIX  4 


enum CONST_MUSIC
{
	MUS_MENU = 0,
	MUS_INGAME1,
	MUS_INGAME2,
	MUS_INGAME3
};


enum CONST_SOUNDS
{
	SND_CRASHLIGHT1 = 0,
	SND_CRASHLIGHT2 = 1,
	SND_CRASHBRAKE = 2,
	SND_EXPLOSION1 = 3,
	SND_SPLAT1 = 4,
	SND_TIRES1 = 5,
	SND_TIRES2 = 6,
	SND_WARNING, 
	SND_MENU_NEXTCAR,
	SND_MENU_HONK1,
	SND_MENU_OVERBUTTON,
	SND_MENU_CLICK
};



class CSounds 
{

private:

	CSdl	*_sdl;
#ifdef WITH_FMOD
	int		sounds[NUM_SOUNDS];
	FMOD_SOUND	*music[NUM_MUSIX];
#endif

	int		current_track;
	bool	music_stopped;

public:

	CSounds();
	~CSounds();

	bool Initialize( CSdl *pSdl );
	void Release();

	void Play( CONST_SOUNDS snd_to_play );
	void Play( CONST_SOUNDS snd_to_play, int pos );
	void Play( CONST_MUSIC music_to_play, bool looped );
	void StopMusic();
	void PlayMusic() { music_stopped = false; };
	void FadeMusic( int milliseconds );
	void CheckMusic();
	bool IsMusicPlaying() { return !music_stopped; };

	void setMusicVolume( int volume );
};


#endif
