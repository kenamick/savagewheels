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

#ifndef __CSDL
#define __CSDL

////classes//////////////
class CGame;
class CSound;
class CKdf_Packeger;
/////////////////////////

#define RELEASE_SURF( surf ) if ( (surf) != NULL ) { SDL_FreeSurface( surf ); surf = NULL; }

//#define FONT_TTF						// use TTF_FONTS

#define MAX_SPRITES  60 				// maximum kartinki na ekarana
#define SDL_FAIL     0	      			// flagove za greshki
#define SDL_OK       -1
#define NO_COLORKEY  -1
#define NO_ALPHA	 255
#define MAX_SOUNDS	 30					// maximum slotove za sounda

#define MOUSE_BUTTON_UNPRESSED	0
#define MOUSE_BUTTON_UP			1
#define MOUSE_BUTTON_DOWN		2
#define JOY_AXIS_UP			    1
#define JOY_AXIS_DOWN			2
#define JOY_AXIS_LEFT			3
#define JOY_AXIS_RIGHT			4
#define JOY_AXIS_NONE		    5

#define BLACK		   0x0
#define WHITE		   0xFFFFFF
#define MAGENTA_888	   0xFF00FF
#define MAGENTA_565	   0xF81F		
#define MAGENTA_555	   0x7C1F
#define MAGENTA 	   MAGENTA_888
#define SHADOW_MASK565 0xF7DE
#define SHADOW_MASK555 0xFBDE
#define SHADOW_MASK888 0xFEFEFE

// #ifdef RGB_MODE555
//  #define RGB_16BIT(r,g,b) ( ((r) << 10) | ((g) << 5) | (b) ) 
// #else
//  #define RGB_16BIT(r,g,b) ( ((r) << 11) | ((g) << 5) | (b) ) 
// #endif

#define ANALOG_THRESHOLD 20000


typedef struct _BITMAPFILEHEADER {	// bmfh 
    Uint16  bfType; 
    Uint32  bfSize; 
    Uint16  bfReserved1; 
    Uint16  bfReserved2; 
    Uint32  bfOffBits; 
} __BITMAPFILEHEADER; 

// (SDL_Mixer) Sound Class
class CSound
{

public:
	CSound()
		: buffered(false), play_channel(-1), loaded(false)
#ifdef WITH_FMOD
		,sound(NULL)
#endif
	{
	};

	~CSound()
	{
	};

#ifdef WITH_FMOD	
	FMOD_SOUND   *sound;
#endif
	bool	buffered;				// buffered sound?
	int		play_channel;			// unbuffered sounds do not need separate channels
	bool	loaded;
	
	void Release();

};



class CSdl 
{

protected:

	// graphics
	struct STRUCT_BLIT 
	{
		Sint32 x,y,z;
		SDL_Surface *surf;
	};

	typedef std::vector<int>	udtButtonsBuffer;
   
	SDL_Surface *screen;			    // glavna powyrhnost(buffer) na Sdl-to
	STRUCT_BLIT surface[MAX_SPRITES];
	Uint32		num_surfaces;			// broi kartinki koito da bydat rendirani pri sledwashitq kadyr  
	int			mouse_x, mouse_y;
	int			mouse_lbutton, mouse_rbutton;

	Uint16		magenta16;
	Uint16		shadow_mask16;
	Uint16		bytes_per_color;
	CGame		*_game;

#ifdef FONT_TTF
   TTF_Font			*font_ttf;
#else
   SDL_Surface		*font_bmp;
   Uint16			font_size;
#endif

#ifdef WITH_FMOD
   FMOD_SYSTEM      *fmod_system;
   FMOD_CHANNEL 	*fmod_musicChannel;
#endif

   CSound		  sounds[MAX_SOUNDS];	
   bool			  bsound_initialized;	 // flag-rezultat inicializaciqta na zvuka
   int			  volume_sound;
   int			  volume_music;

   SDL_Joystick		*_joystick;
   bool			_bJoystickSupport;
   int 			_nJoystickDevices;
   int			_nJoystickIdxDeviceToUse;
   Sint16		_xJoystick;
   Sint16		_yJoystick;
   udtButtonsBuffer	_JoystickButtons;
 
public:
	Uint8			*keys;				// masiv s aktivnite klavishi (natisnati)
	int				JoystickAxisX;
	int				JoystickAxisY;
	Uint8			JoystickHatState;

protected:
	void	_Blitall();
	Uint32	_GetPixel(SDL_Surface *surface, int x, int y);
	int		_Slock( SDL_Surface *surface );
	void	_Sunlock( SDL_Surface *surface );
	int		_ClipRect( int *x , int *y, SDL_Rect *rSurf );
//	SDL_Surface* LoadBitmap( char *filename, long file_offset, Uint32 file_size, Uint32 color_key = NO_COLORKEY, Uint16 alpha_value = NO_ALPHA);

public:
	CSdl();
	~CSdl();

	// sdl_gfx
	bool Initialize( CGame *game, int nWidth, int nHeight, int nBpp, bool bFullscreen, bool bHardware = false );
	bool InitializeJoystick();
	bool AcquireJoystick();
	void UnAcquireJoystick();
	void ReleaseJoystick();
	void Close();
	void Flip();
	void FlipTo( SDL_Surface *dest_surf );
	void ToggleFullscreen();
	int  Addtoblit( Sint32 x, Sint32 y, SDL_Surface *surf );
	void BlitNow( Sint32 x, Sint32 y, SDL_Surface *surf );
	void BlitNow( Sint32 x, Sint32 y, SDL_Surface *surf, SDL_Rect *rsurf );
	void BlitShadow( Sint32 x, Sint32 y, Uint32 *mask, SDL_Rect *rsurf );
	void BlitShadow( Sint32 x, Sint32 y, SDL_Surface *surf);
	int  Collide( SDL_Rect *r_result, SDL_Rect *r1, SDL_Rect *r2 );
	int  Collide( SDL_Rect *r1, Uint32 *mask1, SDL_Rect *r2, Uint32 *mask2 );
	void MakeBoolMask( SDL_Surface *surf, Uint32 *& );
	//SDL_Surface* LoadBitmap( char *filename, Uint32 color_key = NO_COLORKEY, Uint8 alpha_value = NO_ALPHA );
	//SDL_Surface* LoadBitmap( char *filename, long file_offset, Uint32 file_size, Uint32 color_key = NO_COLORKEY, Uint8 alpha_value = NO_ALPHA );
	SDL_Surface* LoadBitmap( const char *filename, Uint32 color_key = NO_COLORKEY, Uint8 alpha_value = NO_ALPHA );
	SDL_Surface* LoadBitmap( const char *filename, int32_t file_offset, Uint32 file_size, Uint32 color_key = NO_COLORKEY, Uint16 alpha_value = NO_ALPHA);
	SDL_Surface* CreateEmptySurface( int width, int height );
	void	SetRect( SDL_Rect *rect, int x, int y, int width, int height );

	// sdl_font
#ifdef FONT_TTF
	void InitializeFont( int fontsize = 12 );
	void DrawText( int x, int y, char *text, SDL_Color forecolor );
	void DrawText( int x, int y, char *text, SDL_Color forecolor, SDL_Color backcolor );
#else
	void InitializeFont();
	void DrawNum( int x, int y, char *text );
#endif

	SDL_Color CreateColor( int r, int g, int b, int a );
	SDL_Color CreateColor( int r, int g, int b );

	// Input methods
	void GetInput();
	bool GetJoystickButtonPressed( int idx );
	int  GetMouseX() { return mouse_x; };
	int  GetMouseY() { return mouse_y; };
	int  GetMouseLButton() { return mouse_lbutton; };
	int  GetMouseRButton() { return mouse_rbutton; };

	// Sound functions
#ifdef WITH_FMOD
	static bool IsFModOK(FMOD_RESULT result);
#endif
	int  LoadSound( const char *filename, bool buffered_sound, bool IsStream = false );
	void PlaySound( int snd_index, int position = -1 );
	void PlayMusic( int snd_index, bool looped );
	void StopMusic();
	bool IsMusicPlaying();
	//Mix_Chunk* LoadWav( char *filename, long file_offset, Uint32 file_size );

	int  GetSoundVolume() { return volume_sound; };
	int  GetMusicVolume() { return volume_music; };
	void SetSoundVolume( int );
	void SetMusicVolume( int );
	void ChangeSoundVolume( int );
	void ChangeMusicVolume( int );
	
  private:

	void BlitShadow16( Sint32 x, Sint32 y, Uint32 *mask, SDL_Rect *rsurf );
	void BlitShadow32( Sint32 x, Sint32 y, Uint32 *mask, SDL_Rect *rsurf );
	void BlitShadow16( Sint32 x, Sint32 y, SDL_Surface *surf );
	void BlitShadow32( Sint32 x, Sint32 y, SDL_Surface *surf );
	void MakeBoolMask16( SDL_Surface *surf, Uint32 *& );
	void MakeBoolMask32( SDL_Surface *surf, Uint32 *& );
};


#endif
