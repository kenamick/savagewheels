// CSounds.h -

#ifndef __CSOUNDS_INCLUDED
#define __CSOUNDS_INCLUDED


#define NUM_SOUNDS 12
#define NUM_MUSIX  4 


typedef enum CONST_MUSIC
{
	MUS_MENU = 0,
	MUS_INGAME1,
	MUS_INGAME2,
	MUS_INGAME3
};


typedef enum CONST_SOUNDS
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

	CSdl			*_sdl;
	int				sounds[NUM_SOUNDS];
	FMUSIC_MODULE	*music[NUM_MUSIX];

	int				current_track;
	bool			music_stopped;

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