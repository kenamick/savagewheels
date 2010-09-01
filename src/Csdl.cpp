// CSdl.cpp  - 

#include "Main.h"


///////////////////////////////////////////////////////////////////////
// Ime: CSdl()
// Opisanie: Constructor
///////////////////////////////////////////////////////////////////////
CSdl::CSdl()
:	_game( NULL ),
	keys( NULL ),
	num_surfaces( 0 ),
	_joystick( NULL ),
	_bJoystickSupport( false ),
	_nJoystickDevices( 0 ),
	_nJoystickIdxDeviceToUse( -1 ),
	_xJoystick( ANALOG_THRESHOLD ),
	_yJoystick( ANALOG_THRESHOLD ),
	JoystickHatState( SDL_HAT_CENTERED ),
	mouse_lbutton( MOUSE_BUTTON_UNPRESSED ),
	mouse_rbutton( MOUSE_BUTTON_UNPRESSED )
{

	// reset na powyrhnostite za blitvane
	keys = SDL_GetKeyState(NULL);

	_JoystickButtons = udtButtonsBuffer( 30 );

}



///////////////////////////////////////////////////////////////////////
// Ime: ~CSdl()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
CSdl::~CSdl()
{

}



///////////////////////////////////////////////////////////////////////
// Ime: Close()
// Opisanie: close SDL maintenance
///////////////////////////////////////////////////////////////////////
void CSdl::Close()
{
	
	AppendToLog("Closing SDL class...");


	// free all sounds
	if ( bsound_initialized )
	{
		AppendToLog("FMod Status: Releasing Game Sounds... ");
		for ( Uint32 i = 0; i < MAX_SOUNDS; i++ )
		{
			//if ( sounds[i].loaded )
				//sounds[i].Release();
		}


		AppendToLog("FMod: Closing...");
		FSOUND_Close();
	}


#ifdef FONT_TTF
	TTF_CloseFont( font_ttf );
#else
	RELEASE_SURF( font_bmp );
#endif

	ReleaseJoystick();

	SDL_Quit();

	AppendToLog( "SDL Status: CSdl closed" );

}


///////////////////////////////////////////////////////////////////////
// Ime: BlitNow()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
void CSdl::BlitNow( int x, int y, SDL_Surface *surf)
{
	SDL_Rect      rdest;

	rdest.x = x;
	rdest.y = y;
	SDL_BlitSurface( surf, NULL, screen, &rdest );
}



///////////////////////////////////////////////////////////////////////
// Ime: BlitNow()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
void CSdl::BlitNow( int x, int y, SDL_Surface *surf, SDL_Rect *rsurf )
{
	SDL_Rect      rdest;

	rdest.x = x;
	rdest.y = y;
	SDL_BlitSurface( surf, rsurf, screen, &rdest );
}


///////////////////////////////////////////////////////////////////////
// Ime: _BlitAll()
// Opisanie: izprashta wsichki kyrtinki za rendirane
///////////////////////////////////////////////////////////////////////
void CSdl::_Blitall()
{

  
  SDL_Rect      rdest;
  bool          bFlag = true;
  STRUCT_BLIT   surf;
  Uint32		i;

  if ( num_surfaces == 0 ) return;

  // sortirai kartinkite predi rendirane
  while(bFlag)
  {
	bFlag = false;

	for ( i = 0; i < num_surfaces - 1; i++)
	{
	  if ( surface[i].z > surface[i+1].z ) 
	  {
		surf = surface[i];
		surface[i] = surface[i+1];
		surface[i+1] = surf;
		bFlag = true;
	  }
	}
  
  }

  for ( i = 0; i < num_surfaces; i++ )
  {
    rdest.x = surface[i].x;
	rdest.y = surface[i].y;
	SDL_BlitSurface( surface[i].surf, NULL, screen, &rdest );
  }

  // izchisti broqcha
  num_surfaces = 0;

}


///////////////////////////////////////////////////////////////////////
// Ime: Addtoblit()
// Opisanie: dobawq kartinka kym masiva za pokazwane na ekrana
///////////////////////////////////////////////////////////////////////
int CSdl::Addtoblit( int x, int y, SDL_Surface *surf )
{

  // proweri dali ima mqsto w masiva
  if ( num_surfaces + 1 > MAX_SPRITES ) return SDL_FAIL;
 
  // dobawi powyrhnostta kym masiva za rendirane
  
  surface[num_surfaces].x = x;
  surface[num_surfaces].y = y;
  surface[num_surfaces].z = y + surf->w / 2;
  surface[num_surfaces].surf = surf;
  num_surfaces++;
  

  return SDL_OK;
}


///////////////////////////////////////////////////////////////////////
// Ime: Flip() 
// Opisanie: flipva, razmenq backbuffer-a i primbufera...abe pokazwa
// wsichko na ekrana ;]
///////////////////////////////////////////////////////////////////////
void CSdl::Flip()
{

  _Blitall();

  SDL_Flip(screen);

}



///////////////////////////////////////////////////////////////////////
// Ime: FlipTo() 
// Opisanie: blit-va backbuffera v/u dadena powyrhnost
///////////////////////////////////////////////////////////////////////
void CSdl::FlipTo( SDL_Surface *dest_surf )
{

  SDL_Rect      rdest;

  rdest.x = 0;
  rdest.y = 0;
  SDL_BlitSurface( screen, NULL, dest_surf, &rdest );

}



///////////////////////////////////////////////////////////////////////
// Ime: GetInput()
// Opisanie: wzema klawishite
///////////////////////////////////////////////////////////////////////
void CSdl::GetInput()
{

	//SDL_Event     event;  
	Uint8  button_mask = 0U;

	//SDL_PollEvent( &event );
	// wzemi rejimite na klavishite i gi zapishi w masiva
	SDL_PumpEvents();

	if ( _nJoystickIdxDeviceToUse != -1 )
	{
		SDL_JoystickUpdate();

		//_xJoystick = SDL_JoystickGetAxis( _joystick , 0 );
		//_yJoystick = SDL_JoystickGetAxis( _joystick, 1 );

		//JoystickAxisX = JoystickAxisY = JOY_AXIS_NONE;

		//if ( _xJoystick > ANALOG_THRESHOLD )
		//{
		//	JoystickAxisX = JOY_AXIS_RIGHT;
		//}
		//else if ( _xJoystick < -ANALOG_THRESHOLD )
		//{
		//	JoystickAxisX = JOY_AXIS_LEFT; 
		//}

		//if ( _yJoystick > ANALOG_THRESHOLD )
		//{
		//	JoystickAxisY = JOY_AXIS_UP;
		//}
		//else if ( _yJoystick < -ANALOG_THRESHOLD )
		//{
		//	JoystickAxisY = JOY_AXIS_DOWN;
		//}

		int num_hats = SDL_JoystickNumHats( _joystick );

		if ( num_hats > 0 )
		{
			JoystickHatState = SDL_JoystickGetHat( _joystick, 0 );
		}

		int num_buttons = SDL_JoystickNumButtons( _joystick );

		for( int i = 0; i < num_buttons; i++ )
		{
			if ( SDL_JoystickGetButton( _joystick, i ) )
				_JoystickButtons.push_back( i );
		}


	}

	button_mask = SDL_GetMouseState( &mouse_x, &mouse_y );

	// check mouse buttons
	if ( button_mask & SDL_BUTTON(1) )		// left
	{
		if ( mouse_lbutton == MOUSE_BUTTON_UNPRESSED )
			mouse_lbutton = MOUSE_BUTTON_DOWN;
	}
	else if ( button_mask & SDL_BUTTON(2) ) // right
	{
		if ( mouse_rbutton == MOUSE_BUTTON_UNPRESSED )
			mouse_rbutton = MOUSE_BUTTON_DOWN;
	}
	else
	{
		if ( mouse_lbutton == MOUSE_BUTTON_DOWN )
			mouse_lbutton = MOUSE_BUTTON_UP;
		else if ( mouse_lbutton == MOUSE_BUTTON_UP )
			mouse_lbutton = MOUSE_BUTTON_UNPRESSED;


		if ( mouse_rbutton == MOUSE_BUTTON_DOWN )
			mouse_rbutton = MOUSE_BUTTON_UP;
		else if ( mouse_rbutton == MOUSE_BUTTON_UP )
			mouse_rbutton = MOUSE_BUTTON_UNPRESSED;
	}

	//keys = SDL_GetKeyState(NULL);

	// same as GetKeyState() only done manual
	/*while ( SDL_PollEvent( &event ) )
	{
	switch ( event.type )
	{
	case SDL_KEYDOWN:
	//if ( event.key.keysym.sym == SDLK_ESCAPE )
	//bRunning = false;
	keys[event.key.keysym.sym] = 1;
	break;

	case SDL_KEYUP:
	keys[event.key.keysym.sym] = 0;
	break;

	}
	}*/

}

///////////////////////////////////////////////////////////////////////
// Ime: GetJoystickButtonPressed()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
bool CSdl::GetJoystickButtonPressed( int idx )
{

	for( udtButtonsBuffer::iterator it = _JoystickButtons.begin();
		it != _JoystickButtons.end(); 
		 )
	{
		if ( *it == idx )
		{
			it = _JoystickButtons.erase( it );
			return true;
		}

		it++;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////
// Ime: Collide()
// Opisanie: proverqwa dali 2 kvadrata se zastpywat
///////////////////////////////////////////////////////////////////////
int CSdl::Collide( SDL_Rect *r_result, SDL_Rect *r1, SDL_Rect *r2 )
{

	if ( 
		 ((r1->x > r2->x && r1->x < r2->w) || (r1->w > r2->x && r1->w < r2->w)) &&
		 ((r1->y > r2->y && r1->y < r2->h) || (r1->h > r2->y && r1->h < r2->h)) || 
	     ((r2->x > r1->x && r2->x < r1->w) || (r2->w > r1->x && r2->w < r1->w)) &&
		 ((r2->y > r1->y && r2->y < r1->h) || (r2->h > r1->y && r2->h < r1->h))
		 )
	{
		
			if ( r_result != NULL )
			{
				if ( r1->x < r2->x )
					r_result->x = r2->x;
				else
					r_result->x = r1->x;
			
				if ( r1->y < r2->y )
					r_result->y = r2->y;
				else
					r_result->y = r1->y;

				if ( r1->w > r2->w )
					r_result->w = r2->w;
				else
					r_result->w = r1->w;

				if ( r1->h > r2->h )
					r_result->h = r2->h;
				else
					r_result->h = r1->h;
			}

			return 1;
	}

	return 0;
}


///////////////////////////////////////////////////////////////////////
// Ime: BlitShadow() 
// Opisanie: shadow from surface 50/50 trans using MASK 
///////////////////////////////////////////////////////////////////////
void CSdl::BlitShadow( int x, int y, int *mask, SDL_Rect *rsurf )
{
    if ( bytes_per_color == 2 )
      BlitShadow16(x, y, mask, rsurf);
    else
      BlitShadow32(x, y, mask, rsurf);
}


///////////////////////////////////////////////////////////////////////
// Ime: BlitShadow16() 
// Opisanie: 16bit shadow from surface 50/50 trans using MASK 
///////////////////////////////////////////////////////////////////////
void CSdl::BlitShadow16( int x, int y, int *mask, SDL_Rect *rsurf )
{

	if ( ! _game->game_shadows ) 
		return;

	register Uint16		i = 0U, j = 0U;
	register Uint16		dst_color = 0U;
	int					*mask_val = NULL;

	_Slock( screen );

	x *= bytes_per_color;   // 2 bytes per pixel =16bpp
	Uint16 *pixel1 = (Uint16 *)((Uint8 *)screen->pixels + y * screen->pitch + x );

	for ( j = rsurf->y; j < rsurf->h; j++ )
	{
		mask_val = &mask[j*rsurf->w];

		for ( i = rsurf->x; i < rsurf->w; i++, mask_val++ )
		{
 			dst_color = *pixel1;

			if ( *mask_val )
				*pixel1 =  ((dst_color & shadow_mask) >> 1);
				//*pixel1 =  ((src_color & 0xF7DE) >> 1) + ((dst_color & 0xF7DE) >> 1);
			pixel1++;
		}

		// premseti pad-a
		pixel1 = (Uint16 *)((Uint8 *)screen->pixels + x + (y + j) * screen->pitch );
	}

	_Sunlock( screen );
}

///////////////////////////////////////////////////////////////////////
// Ime: BlitShadow32() 
// Opisanie: 32 bit shadow from surface 50/50 trans using MASK 
///////////////////////////////////////////////////////////////////////
void CSdl::BlitShadow32( int x, int y, int *mask, SDL_Rect *rsurf )
{

  
  
	if ( ! _game->game_shadows ) 
		return;

	register Uint16		i = 0U, j = 0U;
	register Uint32		dst_color = 0U;
	int			*mask_val = NULL;

	_Slock( screen );

	//x *= bytes_per_color; 
	Uint32 *pixel1 = ((Uint32 *)screen->pixels + y * (screen->pitch/4) + x);

	for ( j = rsurf->y; j < rsurf->h; j++ )
	{
		mask_val = &mask[j*rsurf->w];

		for ( i = rsurf->x; i < rsurf->w; i++, mask_val++ )
		{
 			dst_color = *pixel1;

			if ( *mask_val )
				*pixel1 = ((dst_color & SHADOW_MASK888) >> 1);
				//*pixel1 = ((dst_color & SDL_MapRGB(screen->format, 0xFE,0xFE,0xFE)) >> 1);

			pixel1++;
		}

		// premseti pad-a
		pixel1 = ((Uint32 *)screen->pixels + (y + j) * (screen->pitch/4) + x);
	}

	_Sunlock( screen );
}


///////////////////////////////////////////////////////////////////////
// Ime: BlitShadow() 
// Opisanie: shadow from surface 50/50 trans
///////////////////////////////////////////////////////////////////////
void CSdl::BlitShadow( int x, int y, SDL_Surface *surf )
{

	if ( ! _game->game_shadows ) 
		return;

	register Uint16		i = 0U, 
				j = 0U;
	SDL_Rect			rSurf1;
	Uint16				*pixel1 = NULL, 
					*pixel2 = NULL;
	register Uint16		src_color = 0U, 
				dst_color = 0U;

	rSurf1.x = 0;
	rSurf1.y = 0;
	rSurf1.w = surf->w;
	rSurf1.h = surf->h;

	// lock surfaces
	_Slock( screen );
	_Slock( surf );

	x *= 2;   // 2 bytes per pixel =16bpp

	pixel1 = (Uint16 *)((Uint8 *)screen->pixels + y * screen->pitch + x );
	pixel2 = (Uint16 *)((Uint8 *)surf->pixels );


	for ( j = rSurf1.y; j < rSurf1.h; j++ )
	{
		for ( i = rSurf1.x; i < rSurf1.w; i++ )
		{
 
			dst_color = *pixel1;
			src_color = *pixel2;

			if ( src_color != magenta )
				*pixel1 =  ((dst_color & 0xF7DE) >> 1);
				//*pixel1 =  ((src_color & 0xF7DE) >> 1) + ((dst_color & 0xF7DE) >> 1);

			pixel1++;
			pixel2++;
		}

		// premseti pad-a
		pixel1 = (Uint16 *)((Uint8 *)screen->pixels + x + (y + j) * screen->pitch );
		pixel2 = (Uint16 *)((Uint8 *)surf->pixels + j * surf->pitch );
	}


	_Sunlock( screen );
	_Sunlock( surf );

}



///////////////////////////////////////////////////////////////////////////
//// Name: _ClipRect()
//// Purpose: fit rectangle into world screen
///////////////////////////////////////////////////////////////////////////
int CSdl::_ClipRect( int *x , int *y, SDL_Rect *rSurf )
{

	SDL_Rect	rWld, rResult, rDest;
	int			tx = *x, 
				ty = *y;

	//tx -= rWorld.x;
	//ty -= rWorld.y;

	SetRect( &rWld, 0, 0, 640, 480 );
	SetRect( &rDest, tx, ty, tx + rSurf->w, ty + rSurf->x );

	if ( Collide( &rResult, &rWld, &rDest ) )
	{
		rSurf->y = rResult.y - rDest.y;
		rSurf->x = rResult.x - rDest.x;
		rSurf->w = rResult.w - rDest.x;
		rSurf->h = rResult.h - rDest.y;

		*x = rResult.x;
		*y = rResult.y;

		return true;
	}

	return false;
}



///////////////////////////////////////////////////////////////////////
// Ime: SetRect() 
// Opisanie: set rect dims
///////////////////////////////////////////////////////////////////////
void CSdl::SetRect( SDL_Rect *rect, int x, int y, int width, int height )
{
	rect->x = x;
	rect->y = y;
	rect->w = width;
	rect->h = height;
}

///////////////////////////////////////////////////////////////////////
// Ime: MakeBoolMask() 
// Opisanie: make sprte boolean mask
///////////////////////////////////////////////////////////////////////
void CSdl::MakeBoolMask( SDL_Surface *surf, int *&mask )
{
    if ( bytes_per_color == 2 )
      MakeBoolMask16( surf, mask );
    else //32 bit
      MakeBoolMask32( surf, mask );
}

///////////////////////////////////////////////////////////////////////
// Ime: MakeBoolMask16() 
// Opisanie: make sprte boolean mask
///////////////////////////////////////////////////////////////////////
void CSdl::MakeBoolMask16( SDL_Surface *surf, int *&mask )
{

	register int  i,j,w,h,pos;
	Uint16		  *pixel;

	_Slock( surf );

	pixel = (Uint16 *)((Uint8 *)surf->pixels );
	w = surf->w;
	h = surf->h;

	mask = (int *) new int[w*h];
	
	for ( j = 0; j < h; j++ )
	{
		for ( i = 0; i < w; i++, pixel++ )
		{
			pos = w * j + i; 
			mask[pos] = ( *pixel != magenta ) ? 1 : 0;
		}

		// premseti pad-a
		pixel = (Uint16 *)((Uint8 *)surf->pixels + j * surf->pitch );
	}

	_Sunlock( surf );
}

///////////////////////////////////////////////////////////////////////
// Ime: MakeBoolMask32() 
// Opisanie: make sprte boolean mask
///////////////////////////////////////////////////////////////////////
void CSdl::MakeBoolMask32( SDL_Surface *surf, int *&mask )
{

	register int  i,j,w,h,pos;
	Uint32	*pixel;

	_Slock( surf );

	pixel = (Uint32 *)surf->pixels;
	w = surf->w;
	h = surf->h;

	mask = (int *) new int[w*h];
	
	for ( j = 0; j < h; j++ )
	{
		for ( i = 0; i < w; i++, pixel++ )
		{
			pos = w * j + i; 
			mask[pos] = ( *pixel != MAGENTA ) ? 1 : 0;
		}

		// premseti pad-a
		pixel = (Uint32 *)surf->pixels + j * surf->pitch/bytes_per_color;
	}

	_Sunlock( surf );
}


///////////////////////////////////////////////////////////////////////
// Ime: Collide() 
// Opisanie: proverqwa dali pixelite na 2 povyrhnosti se zastypwat 
// PIXELPERFECT - collision detection using int Masks
///////////////////////////////////////////////////////////////////////
//int CSdl::Collide( SDL_Rect *r1, SDL_Surface *surf1, SDL_Rect *r2, SDL_Surface *surf2 )
int CSdl::Collide( SDL_Rect *r1, int *mask1, SDL_Rect *r2, int *mask2 )
{

	SDL_Rect  rt1, rt2, rret;
	SDL_Rect  rSurf1, rSurf2;
	int       col_width, col_height;
	int		  x_off1, y_off1;
	int		  x_off2, y_off2;
	bool	  bcollision = false;
	int 	  *pm1, *pm2;
	int		  w1, w2; //, h1, h2;

	SetRect( &rt1, r1->x, r1->y, r1->w, r1->h );
	SetRect( &rt2, r2->x, r2->y, r2->w, r2->h );
    
	w1 = r1->w - r1->x;
	w2 = r2->w - r2->x;

	if ( w1 < 0 || w2 < 0 || 
		 r1->w > 1000 || r1->h > 1000 || 
		 r2->w > 1000 || r2->h > 1000 )
	{
		DBG( "RECTANGLE OVERFLOW!" );
	}

	//h1 = r1->h - r1->y;
	//h2 = r2->h - r2->y;

	if ( Collide( &rret, &rt1, &rt2 ) )
	{
		// get overlappings
		rSurf1.x = rret.x - rt1.x;
		rSurf1.y = rret.y - rt1.y;
		rSurf1.w = rret.w - rt1.x;
		rSurf1.h = rret.h - rt1.y;
		x_off1 = ( rSurf1.y * w1 ) + rSurf1.x;
		
		rSurf2.x = rret.x - rt2.x;
		rSurf2.y = rret.y - rt2.y;
		rSurf2.w = rret.w - rt2.x;
		rSurf2.h = rret.h - rt2.y;
		x_off2 = ( rSurf2.y * w2 ) + rSurf2.x;

		// dimensii na intersekciqta
		col_width = (rret.w - rret.x);
		col_height = (rret.h - rret.y);

		for ( int j = 0; j < col_height; j++ )
		{
			// y - posiciq
			y_off1 = j * w1 + x_off1;
			y_off2 = j * w2 + x_off2;
			pm1 = &mask1[ y_off1 ];
			pm2 = &mask2[ y_off2 ];

			for ( int i = 0; i < col_width; i++, pm1++, pm2++ )
			{
				if ( *pm1 && *pm2 )
					bcollision = true;
			}

			// zatvori cikyla
			if ( bcollision ) break;
		}

		/*Slock( surf1 );
		Slock( surf2 );

		for ( int j = 0; j < col_height; j++ )
		{
			// y - posiciq
			y_off1 = j + rSurf1.y;
			y_off2 = j + rSurf2.y;

			for ( int i = 0; i < col_width; i++ )
			{
				// x - posiciq
				x_off1 = i + rSurf1.x;
				x_off2 = i + rSurf2.x;

				pixel1 = (Uint16 *)((Uint8 *)surf1->pixels + y_off1 * surf1->pitch + (x_off1 << 1) );
				pixel2 = (Uint16 *)((Uint8 *)surf1->pixels + y_off2 * surf2->pitch + (x_off2 << 1) );

				if ( *pixel1 != magenta && *pixel2 != magenta )
				{
					bcollision = true;
				}
			}

			// zatvori cikyla
			if ( bcollision ) break;
		}

		_Sunlock( surf1 );
		_Sunlock( surf2 );*/

		if ( bcollision ) return 1;

	}

	// no collision
	return 0;
}




///////////////////////////////////////////////////////////////////////
// Ime: _GetPixel() -> SDL_DOCUMENTATION
// Opisanie: 
///////////////////////////////////////////////////////////////////////
Uint32 CSdl::_GetPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}



///////////////////////////////////////////////////////////////////////
// Ime: _Slock()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
inline int CSdl::_Slock( SDL_Surface *surface )
{
	if ( SDL_MUSTLOCK( surface ) )
	{
		if ( SDL_LockSurface( surface ) < 0 ) 
			return 0;
	}

	return 1;
}


///////////////////////////////////////////////////////////////////////
// Ime: _Sunlock()
// Opisanie:
///////////////////////////////////////////////////////////////////////
inline void CSdl::_Sunlock( SDL_Surface *surface )
{

	if ( SDL_MUSTLOCK( surface ) )
	{
		SDL_UnlockSurface( surface );
	}

}


///////////////////////////////////////////////////////////////////////
// Ime: Initialize()
// Opisanie: Inicializira Sdl i prewkliuchwa video rejima
///////////////////////////////////////////////////////////////////////
bool CSdl::Initialize( CGame *game, int nWidth, int nHeight, int nBpp, int bFullscreen, int bHardware )
{

	char temp[128]				= { 0 };
	SDL_VideoInfo  *vid_info	= NULL;
	Uint32			flags		= 0;

	this->_game = game;
	ASSERT( _game != NULL );

	AppendToLog("SDL Status: Opening Simple DirectMedia Layer� ... " );

	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		fprintf (stderr, "Couldn't initialize SDL: %s\n", SDL_GetError() );

		LOG( "SDL Error: ...failed to open SDL :  " << SDL_GetError() );

		return false;
	}

	// get video capabilities
	SDL_VideoDriverName( temp, 128 );
	LOG( "Video Driver: " << temp );
	
	vid_info =(SDL_VideoInfo *) SDL_GetVideoInfo();
	LOG( "Total Video Memory: " << vid_info->video_mem );
	LOG( "hw_available: " << vid_info->hw_available );
	LOG( "wm_available: " << vid_info->wm_available );
	LOG( "blit_hw: " << vid_info->blit_hw );
	LOG( "blit_hw_CC: " << vid_info->blit_hw_CC );
	LOG( "blit_hw_A: " << vid_info->blit_hw_A );
	LOG( "blit_sw: " << vid_info->blit_sw );
	LOG( "blit_sw_CC: " << vid_info->blit_sw_CC );
	LOG( "blit_sw_A: " << vid_info->blit_sw_A );

	// check video memory
	if ( vid_info->video_mem > 24000U )
		flags |= SDL_HWSURFACE;
	else
		flags |= SDL_SWSURFACE;

	// ne se nalaga wikane na SDL_Quit() pri izhod 
	//atexit( SDL_Quit );		*/


	// init na glawanata powyrhnost i video rejim
	if ( bFullscreen )
		flags |= SDL_FULLSCREEN;

	flags = flags | SDL_DOUBLEBUF | SDL_ANYFORMAT;

	LOG( "SDL Status: Switching video mode to " << nWidth << "x" << nHeight << "x" << nBpp );

// #ifndef LINUX_BUILD
	//if ( bHardware )
	//{
	//	CHK_VMODE( nWidth, nHeight, nBpp, flags );
	//	screen = SDL_SetVideoMode( nWidth, nHeight, nBpp, flags );
	//}
	if ( ! bHardware )
	{
		flags |= SDL_SWSURFACE;
		//CHK_VMODE( nWidth, nHeight, nBpp, flags | SDL_SWSURFACE );
		//screen = SDL_SetVideoMode( nWidth, nHeight, nBpp, flags | SDL_SWSURFACE );
		////screen = SDL_SetVideoMode( nWidth, nHeight, nBpp, SDL_FULLSCREEN | SDL_SWSURFACE | SDL_ANYFORMAT );
	}

// #else
// 	flags = SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT;
// #endif

	int vd = SDL_VideoModeOK( nWidth, nHeight, nBpp, flags ); 
	LOG( "SDL_VideoModeOK() recommends " << vd << " bit mode." );
	if ( vd == 24 )
	{
		LOG( "Savage Wheels does not support 24 bit video mode !" );
		return false;	  
	}
	
	if ( NULL == (screen = SDL_SetVideoMode( nWidth, nHeight, nBpp, flags )) )
	{
		LOG( "SDL Error: Setting video mode : " << SDL_GetError() );
		return false;
	}
	
	SDL_SetClipRect( screen, NULL );
	SDL_WM_SetCaption( "Savage Wheels - KenamicK Entertainment", "None" );
	SDL_ShowCursor( SDL_DISABLE );	

	// 16bit-mode-check
	bytes_per_color = screen->format->BytesPerPixel;
	magenta = MAGENTA; // 24&32bit
	shadow_mask = SHADOW_MASK888; //24&32bit

// check for 16/15 bit mode
	if ( bytes_per_color == 2 )
	{
		bytes_per_color = screen->format->BytesPerPixel;
		if ( screen->format->Rmask == 0xF800 )
		{
			magenta = MAGENTA_565;
			shadow_mask = SHADOW_MASK565;
		}
		else
		{
			magenta = MAGENTA_555;
			shadow_mask = SHADOW_MASK555;
		}
	}

	// inicializirai Sound-a
	AppendToLog("FMod Status: Opening... " );

	// check Fmod version and initialize
	if ( FSOUND_GetVersion() < FMOD_VERSION )
	{
		LOG( "FMod Error: You are using lesser FMOD DLL version! Required version is at least FMOD" << FMOD_VERSION );
		bsound_initialized = false;
	}
	else
	{
		bsound_initialized = true;


		if( !FSOUND_Init(44100, 32, FSOUND_INIT_USEDEFAULTMIDISYNTH) )
		{
			LOG( "FMod Error: ...failed to initialize :" << FMOD_ErrorString( FSOUND_GetError() ) );
			bsound_initialized = false;
		}


		LOG( "FMod Driver: " << FSOUND_GetDriverName( FSOUND_GetDriver() ) << " " << FSOUND_GetOutputRate() << " " << FSOUND_GetMaxChannels() );

		//sprintf( temp, "%s %d %d", FSOUND_GetDriverName( FSOUND_GetDriver() ), 
		//	FSOUND_GetOutputRate(), 
		//	FSOUND_GetMaxChannels() 
		//	);
		//memset( this->sounds, 0, sizeof(CSound) * MAX_SOUNDS );
	}

	AppendToLog( "FMod Status: Opened successfully" );

	// get volumes
	volume_sound = 255;
	volume_music = 255;

	return true;
}


///////////////////////////////////////////////////////////////////////
// Ime: InitializeJoystick()
// Opisanie: try to setup joystick support
///////////////////////////////////////////////////////////////////////
bool CSdl::InitializeJoystick()
{
	if ( SDL_InitSubSystem( SDL_INIT_JOYSTICK ) < 0 )
	{
		LOG( "SDL Error: ...failed to init Joystick :  " << SDL_GetError() );

		return false;
	}

	_nJoystickDevices = SDL_NumJoysticks();

	if ( _nJoystickDevices == 0 ) 
	{
		AppendToLog( "No joystick devices found on this system!" );
		return false;
	}

	for ( int i = 0; i < _nJoystickDevices; i++ )
	{
		LOG( "Joystick Device" << i << " name: " << SDL_JoystickName( i ) );
	}

	return _bJoystickSupport = true;
}


///////////////////////////////////////////////////////////////////////
// Ime: AcquireJoystick() 
// Opisanie: get access to joystick device
///////////////////////////////////////////////////////////////////////
bool CSdl::AcquireJoystick()
{
	_nJoystickIdxDeviceToUse = 0 ; // {!}

	if ( ! SDL_JoystickOpened( _nJoystickIdxDeviceToUse ) )
	{
		if ( NULL == ( _joystick = SDL_JoystickOpen( _nJoystickIdxDeviceToUse ) ) )
		{
			LOG( "Failed to open joystick device(id=" << _nJoystickIdxDeviceToUse << ") ! " << SDL_GetError() );

			return false;
		}

		LOG( "Joystick Device" << _nJoystickIdxDeviceToUse << " has: " << SDL_JoystickNumAxes( _joystick ) << " axes." );
		LOG( "Joystick Device" << _nJoystickIdxDeviceToUse << " has: " << SDL_JoystickNumBalls( _joystick ) << " balls." );
		LOG( "Joystick Device" << _nJoystickIdxDeviceToUse << " has: " << SDL_JoystickNumHats( _joystick ) << " pov hats." );
		LOG( "Joystick Device" << _nJoystickIdxDeviceToUse << " has: " << SDL_JoystickNumButtons( _joystick ) << " buttons." );
	}

	return true;
}


///////////////////////////////////////////////////////////////////////
// Ime: UnAcquireJoystick() 
// Opisanie: release access to joystick device
///////////////////////////////////////////////////////////////////////
void CSdl::UnAcquireJoystick()
{
	if ( _joystick )
	{
		SDL_JoystickClose( _joystick );
		_joystick = NULL;
	}
}


///////////////////////////////////////////////////////////////////////
// Ime: ReleaseJoystick() 
// Opisanie: 
///////////////////////////////////////////////////////////////////////
void CSdl::ReleaseJoystick()
{
	if ( _bJoystickSupport )
		UnAcquireJoystick();
}


///////////////////////////////////////////////////////////////////////
// Ime: LoadBitmap() {1}
// Opisanie: zarejda SDL surface ot bitmap
///////////////////////////////////////////////////////////////////////
SDL_Surface* CSdl::LoadBitmap( char *filename, Uint32 color_key, Uint8 alpha_value )
{

#ifndef LOCAL_RES

	String strFilename( ExtractFilename( filename ) );

	long offset = _game->Kdf.GetFilePosition( strFilename.c_str() );
	Uint32 size = _game->Kdf.GetFileSize( strFilename.c_str() );

	return LoadBitmap( ART_FILE, offset, size, color_key, alpha_value );

#else
	
	SDL_Surface *sdl_surf = NULL;

	char filename_buf[255];
	sprintf( filename_buf, "../%s", filename );

	if ( ( sdl_surf = SDL_LoadBMP( filename_buf )) == NULL )
	{
		LOG("...failed to load graphics from : " << filename_buf );
		return NULL;
	}

	// colorkey i alpha BEFORE dispformat vinagi !
	if ( color_key != NO_COLORKEY )
		SDL_SetColorKey( sdl_surf, SDL_SRCCOLORKEY, color_key );

	if ( alpha_value != NO_ALPHA )
		SDL_SetAlpha( sdl_surf, SDL_SRCALPHA, alpha_value );

	
	// display format savmestim s tozi na video mem
	SDL_Surface *new_surf = SDL_DisplayFormat( sdl_surf );
	SDL_FreeSurface( sdl_surf );
	sdl_surf = NULL;


	return new_surf;

#endif

	return NULL;
}



///////////////////////////////////////////////////////////////////////
// Ime: _LoadBitmap() {2}
// Opisanie: zarejda SDL surface ot bitmap v BINRreource file
///////////////////////////////////////////////////////////////////////
SDL_Surface* CSdl::LoadBitmap( char *filename, long file_offset, Uint32 file_size, Uint32 color_key, Uint16 alpha_value )
{

	SDL_Surface  *sdl_surf	= NULL;						// temp surface
	FILE		 *fp		= NULL;						// file pointer
	char		 *pimg		= NULL;						// image data
	SDL_RWops	 *sdl_rw	= NULL;						// sdl_read_write_operations
	//__BITMAPFILEHEADER bmf;
	// 19778 

	// otvori file-a i zaredi kartinkata v temp buffer
	if ( ( fp = fopen( filename, "rb")) == NULL ) 
	{
		LOG("...failed to load graphics from : " << filename );
		return NULL;
	}

	fseek( fp, file_offset, SEEK_CUR );
	// vzemi bitmap-info
	//fread( &bmf, sizeof(bmf), 1, fp );
	// info->mem
	pimg = new char[file_size];
	fread( pimg, file_size, 1, fp );
	fclose(fp);
	
	// syzdai read_write_operaciq
	if ( ( sdl_rw = SDL_RWFromMem( pimg, file_size )) == NULL ) 
	{
		LOG("...failed to create RWops with : " << filename );

		if ( pimg )
			delete[] pimg;

		return NULL;
	}
	// zaredi bitmap ot mem
	if ( ( sdl_surf = SDL_LoadBMP_RW( sdl_rw, 1 )) == NULL ) 
	{
		LOG("...failed to load surface into memory from : " << filename );

		if ( pimg )
			delete[] pimg;

		return NULL;
	}

	// colorkey i alpha BEFORE dispformat vinagi !
	if ( color_key != NO_COLORKEY )
		SDL_SetColorKey( sdl_surf, SDL_SRCCOLORKEY, color_key );

	if ( alpha_value != NO_ALPHA )
		SDL_SetAlpha( sdl_surf, SDL_SRCALPHA, (Uint8)alpha_value );

	SDL_Surface *new_surf = SDL_DisplayFormat( sdl_surf );
	SDL_FreeSurface( sdl_surf );
	sdl_surf = NULL;
	
	if ( pimg )
		delete[] pimg;

	return new_surf;
}



///////////////////////////////////////////////////////////////////////
// Ime: CreateEmptySurface()
// Opisanie: syzdava prazna powyrhnost
///////////////////////////////////////////////////////////////////////
SDL_Surface* CSdl::CreateEmptySurface( int width, int height )
{
	return SDL_CreateRGBSurface( SDL_HWSURFACE, width, height, bytes_per_color * 8,
								 screen->format->Rmask,
								 screen->format->Gmask,
								 screen->format->Bmask,
								 screen->format->Amask );
}




///////////////////////////////////////////////////////////////////////
// Ime: CreateColor()
// Opisanie: color+alpha
///////////////////////////////////////////////////////////////////////
SDL_Color CreateColor( int r, int g, int b, int a )
{
	SDL_Color clr = { r, g, b, a};
	return clr;
}


///////////////////////////////////////////////////////////////////////
// Ime: CreateColor()
// Opisanie: color
///////////////////////////////////////////////////////////////////////
SDL_Color CreateColor( int r, int g, int b )
{
	SDL_Color clr = { r, g, b, 0};
	return clr;
}




/* ---------- FONTS ----------

   define functions for the TTF_Font library
   #else will define functions for 
   Bitmap Font
*/
#ifdef FONT_TTF


///////////////////////////////////////////////////////////////////////
// Ime: InitializeFont()
// Opisanie: Inicializira TTF font
///////////////////////////////////////////////////////////////////////
void CSdl::InitializeFont( int fontsize )
{
	TTF_Init();
	atexit(TTF_Quit);

	// open the font file
	font_ttf = TTF_OpenFont( "fonts/comic.ttf", fontsize );

}



///////////////////////////////////////////////////////////////////////
// Ime: DrawText()
// Opisanie: will draw text on position with background color
///////////////////////////////////////////////////////////////////////
void CSdl::DrawText( int x, int y, char *text, SDL_Color forecolor, SDL_Color backcolor )
{

	SDL_Surface *txtsurf = TTF_RenderText_Shaded( font_ttf, text, forecolor, backcolor );
	SDL_Rect rDest = { x, y, 0, 0 };
	SDL_BlitSurface( txtsurf, NULL, screen, &rDest );
	RELEASE_SURF( txtsurf );
}



///////////////////////////////////////////////////////////////////////
// Ime: DrawText()
// Opisanie: will draw text on position
///////////////////////////////////////////////////////////////////////
void CSdl::DrawText( int x, int y, char *text, SDL_Color forecolor )
{
	SDL_Surface *txtsurf = TTF_RenderText_Solid( font_ttf, text, forecolor );
	SDL_Rect rDest = { x, y, 0, 0 };
	SDL_BlitSurface( txtsurf, NULL, screen, &rDest );
	RELEASE_SURF( txtsurf );
}


#else



///////////////////////////////////////////////////////////////////////
// Ime: InitializeFont()
// Opisanie: Inicializira Bitmap font
///////////////////////////////////////////////////////////////////////
void CSdl::InitializeFont()
{
	//...
	

#ifdef LOCAL_RES
	font_bmp = LoadBitmap( "gfx/interf/fnt.bmp", 0x0 );
#else // LOAD FROM KDF
	font_bmp = LoadBitmap( ART_FILE, _game->Kdf.GetFilePosition("fnt.bmp"), _game->Kdf.GetFileSize("fnt.bmp") , 0x0 );
#endif

	if ( font_bmp == NULL )
	{
		AppendToLog("Error loading font file!");
	}
	
	font_size = 8; //fontsize;  // 7.5

}



///////////////////////////////////////////////////////////////////////
// Ime: DrawText()
// Opisanie: will draw text on position
///////////////////////////////////////////////////////////////////////
void CSdl::DrawNum( int x, int y, char *text )
{
	SDL_Rect rdst;
	SDL_Rect rsrc;

	//16x16  50=ascii53 48
	font_size = 10;
//43= +,-./0
	while ( *text != NULL  )
	{
		rsrc.x = ((*text - '0') * font_size);  
		rsrc.y = 0;
		rsrc.w = font_size;
		rsrc.h = 16;

		rdst.x = x;
		rdst.y = y;
		rdst.w = font_size;
		rdst.h = 16;
		SDL_BlitSurface( font_bmp, &rsrc, screen, &rdst );
		
		x += font_size;
		text++;
	}

	/*for ( unsigned int i = 0; i < strlen(text); i++)
	{

		// 32 - 255
		rdst.x = x + (i * font_size);
		rdst.y = y;
		rdst.w = font_size*2;
		rdst.h = font_size*2;
		// 50  -> 48 53
		// get ascii code
		sprintf( bit, "%c", text[i] );
		// get position in bmp
		mx = bit[0] - font_size;
		mx = (mx / 16) + 1;
		my = bit[0] - font_size;
		my = (my % 8) + 1;
		mx = mx*2*font_size;
		my = my*2*font_size;
		//my -= 16;
		rsrc.x = mx;
		rsrc.y = my;
		rsrc.w = font_size*2;
		rsrc.h = font_size*2;

		SDL_BlitSurface( font_bmp, &rsrc, screen, &rdst );
	}*/

}

#endif




/* ---------- AUDIO ---------- */



///////////////////////////////////////////////////////////////////////
// Ime: LoadSound()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
int CSdl::LoadSound( const char *filename, bool buffered_sound )
{

	if ( !bsound_initialized ) return -1;

	Uint32		   i			= 0;
	static int	   cur_channel	= 0;
	CSound		   *ptr_snd		= sounds;
	bool		   bsnd_loaded	= false;


	while ( i < MAX_SOUNDS )
	{
		if ( !ptr_snd->loaded )
		{
			ptr_snd->sound = FSOUND_Sample_Load( FSOUND_UNMANAGED, filename, FSOUND_NORMAL, 0, 0 );  //Mix_LoadWAV( filename ); //LoadWav( filename, 0, 55000 ); 
			if ( ptr_snd == NULL )
			{
				LOG("...failed to load sound file : " << filename );
				return -1;
			}
			
			if ( !buffered_sound )
			{
				ptr_snd->buffered = false;
				ptr_snd->play_channel = cur_channel;
				cur_channel++;
			}
			else
				ptr_snd->buffered = true;
			
			ptr_snd->loaded = true;
			
			bsnd_loaded = true;
			break;
		}

		i++;
		ptr_snd++;
	}

	if ( !bsnd_loaded ) 
	{
		//... no slots available
		return -1;
	}

	return i;
}


/*
///////////////////////////////////////////////////////////////////////
// Ime: LoadWav()
// Opisanie:  zaredi wav file ot resfile
///////////////////////////////////////////////////////////////////////
Mix_Chunk* CSdl::LoadWav( char *filename, long file_offset, Uint32 file_size )
{

	Mix_Chunk    *chunk;
	FILE		 *fp;
	char		 *psnd;							// sound  data
	SDL_RWops	 *sdl_rw;						// sdl_read_write_operations


	if ( ( fp = fopen( filename, "rb")) == NULL ) 
	{
		AppendToMultilog("...failed to load wav from : ", filename, NULL );
		return NULL;
	}

	fseek( fp, file_offset, SEEK_CUR );
	// info->mem
	psnd = new char[file_size];
	fread( psnd, file_size, 1, fp );
	fclose( fp );

	// syzdai read_write_operaciq
	if ( ( sdl_rw = SDL_RWFromMem( psnd, file_size )) == NULL ) 
	{
		AppendToMultilog("...failed to create RWops with : ", filename, NULL );
		return NULL;
	}

	if ( ( chunk = Mix_LoadWAV_RW( sdl_rw, 1 )) == NULL ) 
	{
		AppendToMultilog("...failed to load wav into memory from : ", filename, NULL );
		return NULL;
	}

	delete[] psnd;

	return chunk;
}

*/

///////////////////////////////////////////////////////////////////////
// Ime: PlaySound()
// Opisanie: play sound at position ( for 640x480 res )
///////////////////////////////////////////////////////////////////////
void CSdl::PlaySound( int snd_index, int position )
{
	 //return;  // {!}
	int chn = -1;

	if ( !bsound_initialized || GetSoundVolume() <= 0 ) return;

	// izchisli posiciqta na zvuka
	if ( position != -1 )
	{

		if ( position > 320 )
		{
			position = (int)(128.0f + 0.1f * (float)position);
			
			if ( position > 205 )
				position = 205;
		}
		else
		{
			position = (int)(0.1f * (float)position);
			if ( position < 50 ) 
				position = 50;
		}
 	}
	else
		position = FSOUND_STEREOPAN;


	// play sound...
	if ( sounds[snd_index].buffered )
	{
		// nameri svoboden kanal i pusni zvuka
		for ( Uint8 i = 0; i < 255; i++ )
		{
			if ( !FSOUND_IsPlaying( i ) )
			{
				chn = FSOUND_PlaySound( i, sounds[snd_index].sound );
				FSOUND_SetPan( chn , position );
				break;
			}
		}
	}
	else
	{
		// pusni prez nebuferiraniq kanal
		//FSOUND_StopSound( sounds[snd_index].play_channel );
		chn = FSOUND_PlaySound( sounds[snd_index].play_channel, sounds[snd_index].sound );
		FSOUND_SetPan( chn, position );		

	}
}



///////////////////////////////////////////////////////////////////////
// Ime: SetMusicVolume()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
void CSdl::SetMusicVolume( int music_vol )
{
	volume_music = music_vol;
	
	if ( volume_music > 128 ) 
		volume_music = 128;
	else if ( volume_music < 0 ) 
		volume_music = 0;

	_game->Snd.setMusicVolume( volume_music );
	//CSnd.setMusicVolume( volume_music );
}



///////////////////////////////////////////////////////////////////////
// Ime: SetSoundVolume()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
void CSdl::SetSoundVolume( int snd_vol )
{
	volume_sound = snd_vol;
	
	if ( volume_sound > 256 ) 
		volume_sound = 256;
	else if ( volume_sound < 0 ) 
		volume_sound = 0;

	FSOUND_SetSFXMasterVolume( volume_sound );
}



/*///////////////////////////////////////////////////////////////////////
// Ime: PlaySound()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
void CSdl::PlaySound( int snd_index )
{

	if ( !bsound_initialized ) return;

	if ( sounds[snd_index].buffered )
		Mix_PlayChannel( -1, sounds[snd_index].sound, 0 );
	else
	{
		Mix_HaltChannel( sounds[snd_index].play_channel );
		Mix_PlayChannel( sounds[snd_index].play_channel, sounds[snd_index].sound, 0 );
	}
}

*/

///////////////////////////////////////////////////////////////////////
// Ime: Release()
// Opisanie: osvobojdava pametta
///////////////////////////////////////////////////////////////////////
void CSound::Release()
{
	if ( sound )
		FSOUND_Sample_Free( sound );
}
