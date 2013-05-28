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

#include "Main.h"


//////////////////////////////////////////////////////////////////////
// Name: CSounds()
// Desc:
//////////////////////////////////////////////////////////////////////
CSounds::CSounds()
: _sdl( NULL ), current_track( 0 ), music_stopped( false )
{
#ifdef WITH_FMOD
	memset( sounds, 0, sizeof(int) * NUM_SOUNDS );
	memset( music, 0, sizeof(int) * NUM_MUSIX );
#endif
}

//////////////////////////////////////////////////////////////////////
// Name: ~CSounds()
// Desc:
//////////////////////////////////////////////////////////////////////
CSounds::~CSounds()
{
}

//////////////////////////////////////////////////////////////////////
// Name: Initialize()
// Desc: zaredi wsichki zvuci v igrata
//////////////////////////////////////////////////////////////////////
bool CSounds::Initialize( CSdl *pSdl )
{
#ifdef WITH_FMOD
	this->_sdl = pSdl;
	ASSERT( _sdl != NULL );

#define LOAD_SOUND( container, name, buffered ) if ( (sounds[container] = _sdl->LoadSound( name, buffered )) == -1 ) { \
	LOG( "Failed to load " << name << " ! "); \
	return false; }

#define LOAD_MUSIC( container, name ) if ( (music[container] = _sdl->LoadSound( name, false, true )) == -1 ) { \
	LOG( "Failed to load music " << name << " ! "); \
	return false; }

	LOAD_SOUND( SND_CRASHLIGHT1, "sound/crash3.wav", true );
	LOAD_SOUND( SND_CRASHLIGHT2, "sound/crash2.wav", true );
	LOAD_SOUND( SND_CRASHBRAKE, "sound/crash1.wav", true );
	LOAD_SOUND( SND_EXPLOSION1, "sound/exp.wav", true );
	LOAD_SOUND( SND_SPLAT1, "sound/splat.wav", true );
	LOAD_SOUND( SND_TIRES1, "sound/tires1.wav", true );
	LOAD_SOUND( SND_TIRES2, "sound/tires2.wav", true );
	LOAD_SOUND( SND_WARNING, "sound/warn.wav", true );
	LOAD_SOUND( SND_MENU_NEXTCAR, "sound/snapshot.wav", true );
	LOAD_SOUND( SND_MENU_HONK1, "sound/honk1.wav", true );
//	LOAD_SOUND( SND_MENU_OVERBUTTON, "sound/mmh3.wav", false );
	LOAD_SOUND( SND_MENU_CLICK, "sound/mmh.wav", true );

	LOAD_MUSIC( MUS_MENU, "sound/seek.it" );
	LOAD_MUSIC( MUS_INGAME1, "sound/inertia.it" );
	LOAD_MUSIC( MUS_INGAME2, "sound/desert.it" );
	LOAD_MUSIC( MUS_INGAME3, "sound/vixens.it" );

	music_stopped = false;

#undef LOAD_SOUND
#undef LOAD_MUSIC
#endif
	return true;
}


//////////////////////////////////////////////////////////////////////
// Name: Play()
// Desc: play sound
//////////////////////////////////////////////////////////////////////
void CSounds::Play( CONST_SOUNDS snd_to_play )
{
#ifdef WITH_FMOD
	if ( _sdl )
		_sdl->PlaySound( sounds[snd_to_play] );
#endif
}


//////////////////////////////////////////////////////////////////////
// Name: Play()
// Desc: play sound with panning
//////////////////////////////////////////////////////////////////////
void CSounds::Play( CONST_SOUNDS snd_to_play, int pos )
{
#ifdef WITH_FMOD
	if ( _sdl )
		_sdl->PlaySound( sounds[snd_to_play], pos );
#endif
}


//////////////////////////////////////////////////////////////////////
// Name: Play()
// Desc: play music
//////////////////////////////////////////////////////////////////////
void CSounds::Play( CONST_MUSIC music_to_play, bool looped )
{
#ifdef WITH_FMOD  

	if ( _sdl->GetMusicVolume() <= 0 )
		return;

	current_track = (int)music_to_play;
	_sdl->PlayMusic( music[current_track], looped );

#endif
}


//////////////////////////////////////////////////////////////////////
// Name: StopMusic()
// Desc: play music
//////////////////////////////////////////////////////////////////////
void CSounds::StopMusic()
{
#ifdef WITH_FMOD
	if (music_stopped)
		return;

	if ( music[current_track] )
	{
		_sdl->StopMusic();
	}

	music_stopped = true;
#endif
}


void CSounds::FadeMusic( int milliseconds )
{
#ifdef WITH_FMOD
	StopMusic();
#endif
}


//////////////////////////////////////////////////////////////////////
// Name: CheckMusic()
// Desc: proverka za svarshila muzika ( ne trqbwa da se pravi
//			 pri MainMenu-to! )
//////////////////////////////////////////////////////////////////////
void CSounds::CheckMusic()
{
#ifdef WITH_FMOD  
	if ( !music_stopped )
	{
		if (!_sdl->IsMusicPlaying())
		{
//			StopMusic();
//			DBG("CheckMusic playing ...");

			current_track++;
			if ( current_track >= NUM_MUSIX )
				current_track = MUS_INGAME1;

			Play( (CONST_MUSIC)current_track, false );
		}
	}
#endif
}


////////////////////////////////////////////////////////////////////////
//// Name: setMusicVolume()
//// Desc:
////////////////////////////////////////////////////////////////////////
//void CSounds::setMusicVolume( int volume )
//{
//#ifdef WITH_FMOD
//	for ( int i = 0; i < NUM_MUSIX; i++ )
//	{
//		if ( music[i] )
//			FMUSIC_SetMasterVolume( music[i], volume );
//	}
//#endif
//}


//////////////////////////////////////////////////////////////////////
// Name: Release()
// Desc: release music
//////////////////////////////////////////////////////////////////////
void CSounds::Release()
{
#ifdef WITH_FMOD  
	// TODO
#endif
}

