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

///////////////////////////////////////////////////////////////////////
// Name: CSdl()
// Desc: Constructor
///////////////////////////////////////////////////////////////////////
CSdl::CSdl()
:	_game( NULL ),
#ifdef WITH_FMO
	fmod_system(NULL),
	fmod_musicChannel(NULL),
	fmod_groupSounds(NULL),
	fmod_groupMusic(NULL)
#endif
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

	keys = SDL_GetKeyState(NULL);
	_JoystickButtons = udtButtonsBuffer( 30 );

#ifndef FONT_TTF
	font_bmp = NULL;
#endif
}


///////////////////////////////////////////////////////////////////////
// Name: Close()
// Desc: close SDL maintenance
///////////////////////////////////////////////////////////////////////
void CSdl::Close()
{
	LOG("Closing CSdl ...");

	// free all sounds
#ifdef WITH_FMOD	
	if ( bsound_initialized )
	{
		FMOD_RESULT result;

		LOG("FModEx: Closing...");
		LOG("FModEx: Releasing game sounds... ");
		for ( Uint32 i = 0; i < MAX_SOUNDS; i++ )
		{
			if ( sounds[i].loaded )
				sounds[i].Release();
		}

		result = FMOD_SoundGroup_Release(fmod_groupSounds);
		IsFModOK(result);
		result = FMOD_SoundGroup_Release(fmod_groupMusic);
		IsFModOK(result);

	    result = FMOD_System_Close(fmod_system);
	    IsFModOK(result);
	    result = FMOD_System_Release(fmod_system);
	    IsFModOK(result);
	}
#endif

#ifdef FONT_TTF
	TTF_CloseFont( font_ttf );
#else
	RELEASE_SURF( font_bmp );
#endif

	ReleaseJoystick();

	SDL_Quit();

	LOG( "CSdl closed successfully." );
}


///////////////////////////////////////////////////////////////////////
// Name: BlitNow()
// Desc:
///////////////////////////////////////////////////////////////////////
void CSdl::BlitNow( Sint32 x, Sint32 y, SDL_Surface *surf)
{
	SDL_Rect      rdest = { 0, 0, 0, 0 };

	rdest.x = x;
	rdest.y = y;
	SDL_BlitSurface( surf, NULL, screen, &rdest );
}



///////////////////////////////////////////////////////////////////////
// Name: BlitNow()
// Desc:
///////////////////////////////////////////////////////////////////////
void CSdl::BlitNow( Sint32 x, Sint32 y, SDL_Surface *surf, SDL_Rect *rsurf )
{
	SDL_Rect      rdest = { 0, 0, 0, 0 };

	rdest.x = x;
	rdest.y = y;
	SDL_BlitSurface( surf, rsurf, screen, &rdest );
}


///////////////////////////////////////////////////////////////////////
// Name: _BlitAll()
// Desc: izprashta wsichki kyrtinki za rendirane
///////////////////////////////////////////////////////////////////////
void CSdl::Blitall()
{

	if (num_surfaces == 0)
		return;

	SDL_Rect 	rdest = { 0, 0, 0, 0 };
	bool 		bSorted = true;
	STRUCT_BLIT surf;
	Uint32 		i = 0;

	// sort by z-order (TODO: use qsort)
	while (bSorted) {
		bSorted = false;

		for (i = 0; i < num_surfaces - 1; i++) {
			if (surface[i].z > surface[i + 1].z) {
				surf = surface[i];
				surface[i] = surface[i + 1];
				surface[i + 1] = surf;
				bSorted = true;
			}
		}
	}

	for (i = 0; i < num_surfaces; i++) {
		rdest.x = surface[i].x;
		rdest.y = surface[i].y;
		SDL_BlitSurface(surface[i].surf, NULL, screen, &rdest);
	}

	num_surfaces = 0;
}

///////////////////////////////////////////////////////////////////////
// Name: ToggleFullscreen()
// Desc:
///////////////////////////////////////////////////////////////////////
void CSdl::ToggleFullscreen()
{
#ifdef LINUX_BUILD
  if ( ! SDL_WM_ToggleFullScreen(screen) )
  {
    LOG("Error: Switching to Fullscreen. ");
  }
#endif
}

///////////////////////////////////////////////////////////////////////
// Name: Addtoblit()
// Desc: dobawq kartinka kym masiva za pokazwane na ekrana
///////////////////////////////////////////////////////////////////////
bool CSdl::AddToBlit( Sint32 x, Sint32 y, SDL_Surface *surf )
{
	if (num_surfaces >= MAX_SPRITES) {
		return false;
	}

	surface[num_surfaces].x = x;
	surface[num_surfaces].y = y;
	surface[num_surfaces].z = y + (surf->w / 2);
	surface[num_surfaces].surf = surf;
	num_surfaces++;

	return true;
}


///////////////////////////////////////////////////////////////////////
// Name: Flip()
// Desc: flipva, razmenq backbuffer-a i primbufera...abe pokazwa
// wsichko na ekrana ;]
///////////////////////////////////////////////////////////////////////
void CSdl::Flip()
{

  Blitall();

  SDL_Flip(screen);

#ifdef WITH_FMOD
  if (bsound_initialized)
  {
	  FMOD_System_Update(fmod_system);
  }
#endif
}



///////////////////////////////////////////////////////////////////////
// Name: FlipTo()
// Desc: blit-va backbuffera v/u dadena powyrhnost
///////////////////////////////////////////////////////////////////////
void CSdl::FlipTo( SDL_Surface *dest_surf )
{

  SDL_Rect      rdest;

  rdest.x = 0;
  rdest.y = 0;
  SDL_BlitSurface( screen, NULL, dest_surf, &rdest );

}



///////////////////////////////////////////////////////////////////////
// Name: GetInput()
// Desc: wzema klawishite
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
// Name: GetJoystickButtonPressed()
// Desc:
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
// Name: Collide()
// Desc: Checks if two rectangles collide. Result is written back
//       into r_result (if valid object ptr is passed).
///////////////////////////////////////////////////////////////////////
bool CSdl::Collide( SDL_Rect *r_result, SDL_Rect *r1, SDL_Rect *r2 )
{
	if ( 
		 (((r1->x > r2->x && r1->x < r2->w) || (r1->w > r2->x && r1->w < r2->w))
				 && ((r1->y > r2->y && r1->y < r2->h) || (r1->h > r2->y && r1->h < r2->h)))
		 || (((r2->x > r1->x && r2->x < r1->w) || (r2->w > r1->x && r2->w < r1->w))
				 && ((r2->y > r1->y && r2->y < r1->h) || (r2->h > r1->y && r2->h < r1->h)))
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

			return true;
	}

	return false;
}


///////////////////////////////////////////////////////////////////////
// Name: BlitShadow()
// Desc: shadow from surface 50/50 trans using MASK
///////////////////////////////////////////////////////////////////////
void CSdl::BlitShadow( Sint32 x, Sint32 y, Uint32 *mask, SDL_Rect *rsurf )
{
    if ( bytes_per_color == 2 )
      BlitShadow16(x, y, mask, rsurf);
    else
      BlitShadow32(x, y, mask, rsurf);
}


///////////////////////////////////////////////////////////////////////
// Name: BlitShadow16()
// Desc: 16bit shadow from surface 50/50 trans using MASK
///////////////////////////////////////////////////////////////////////
void CSdl::BlitShadow16( Sint32 x, Sint32 y, Uint32 *mask, SDL_Rect *rsurf )
{

	if ( ! _game->game_shadows ) 
		return;

	Uint32			i = 0U, j = 0U;
	Uint32			*mask_val = NULL;

	Slock( screen );

	x *= bytes_per_color;   // 2 bytes per pixel =16bpp
	Uint16 *pixel1 = (Uint16 *)((Uint8 *)screen->pixels + y * screen->pitch + x );

	for ( j = rsurf->y; j < rsurf->h; j++ )
	{
		mask_val = &mask[j*rsurf->w];

		for ( i = rsurf->x; i < rsurf->w; i++, mask_val++ )
		{
			if ( *mask_val )
				*pixel1 =  ((*pixel1 & shadow_mask16) >> 1);
				//*pixel1 =  ((src_color & 0xF7DE) >> 1) + ((dst_color & 0xF7DE) >> 1);
			pixel1++;
		}

		// premseti pad-a
		pixel1 = (Uint16 *)((Uint8 *)screen->pixels + x + (y + j) * screen->pitch );
	}

	Sunlock( screen );
}

///////////////////////////////////////////////////////////////////////
// Name: BlitShadow32()
// Desc: 32 bit shadow from surface 50/50 trans using MASK
///////////////////////////////////////////////////////////////////////
void CSdl::BlitShadow32( Sint32 x, Sint32 y, Uint32 *mask, SDL_Rect *rsurf )
{
	if ( ! _game->game_shadows ) 
		return;

	register Uint32		i = 0U, j = 0U;
	Uint32				*mask_val = NULL;

	Slock( screen );

	//x *= bytes_per_color; 
	Uint32 *pixel1 = (Uint32 *)screen->pixels + y * (screen->pitch >> 2) + x;
	//Uint32 *pixel1 = (Uint32 *)screen->pixels + y * screen->pitch + x * sizeof(Uint32);

	for ( j = rsurf->y; j < rsurf->h; j++ )
	{
		mask_val = &mask[j * rsurf->w];

		for ( i = rsurf->x; i < rsurf->w; i++, mask_val++ )
		{
			if ( *mask_val )
				*pixel1 = (*pixel1 & SHADOW_MASK888) >> 1;
				//*pixel1 = ((dst_color & SDL_MapRGB(screen->format, 0xFE,0xFE,0xFE)) >> 1);

			pixel1++;
		}

		// premseti pad-a
		pixel1 = (Uint32 *)screen->pixels + (y + j) * (screen->pitch >> 2) + x;
		//pixel1 = (Uint32 *)screen->pixels + (y + j) * screen->pitch + (Uint32)x * sizeof(Uint32);
	}
	
	Sunlock( screen );
}


///////////////////////////////////////////////////////////////////////
// Name: BlitShadow()
// Desc: shadow from surface 50/50 trans
///////////////////////////////////////////////////////////////////////
void CSdl::BlitShadow( Sint32 x, Sint32 y, SDL_Surface *surf )
{
    if ( bytes_per_color == 2 )
      BlitShadow16(x, y, surf);
    else
      BlitShadow32(x, y, surf);  
}


///////////////////////////////////////////////////////////////////////////
//// Name: BlitShadow16()
//// Purpose: 
///////////////////////////////////////////////////////////////////////////
void CSdl::BlitShadow16( Sint32 x, Sint32 y, SDL_Surface *surf )
{
	if ( ! _game->game_shadows ) 
		return;

	register Uint16 i = 0U, j = 0U;
	Uint16 *pixel1 = NULL, *pixel2 = NULL;
	SDL_Rect rSurf1;

	rSurf1.x = 0;
	rSurf1.y = 0;
	rSurf1.w = surf->w;
	rSurf1.h = surf->h;

	// lock surfaces
	Slock( screen );
	Slock( surf );

	x *= bytes_per_color;   // 2 bytes per pixel =16bpp

	pixel1 = (Uint16 *)((Uint8 *)screen->pixels + y * screen->pitch + x );
	pixel2 = (Uint16 *)((Uint8 *)surf->pixels );

	for ( j = rSurf1.y; j < rSurf1.h; j++ )
	{
		for ( i = rSurf1.x; i < rSurf1.w; i++ )
		{
			if ( *pixel2 != magenta16 )
				*pixel1 =  (*pixel1 & shadow_mask16) >> 1;
				//*pixel1 =  ((src_color & 0xF7DE) >> 1) + ((dst_color & 0xF7DE) >> 1);

			pixel1++;
			pixel2++;
		}

		// premseti pad-a
		pixel1 = (Uint16 *)((Uint8 *)screen->pixels + x + (y + j) * screen->pitch );
		pixel2 = (Uint16 *)((Uint8 *)surf->pixels + j * surf->pitch );
	}

	Sunlock( screen );
	Sunlock( surf );
}


///////////////////////////////////////////////////////////////////////////
//// Name: BlitShadow32()
//// Purpose: 
///////////////////////////////////////////////////////////////////////////
void CSdl::BlitShadow32( Sint32 x, Sint32 y, SDL_Surface *surf )
{
	if ( ! _game->game_shadows ) 
		return;

	Uint32 		i = 0U, j = 0U;
	Uint32 		*pixel1 = NULL, *pixel2 = NULL;
	SDL_Rect 	rSurf1;

	rSurf1.x = 0;
	rSurf1.y = 0;
	rSurf1.w = surf->w;
	rSurf1.h = surf->h;

	// lock surfaces
	Slock( screen );
	Slock( surf );

	pixel1 = (Uint32 *)screen->pixels + y * (screen->pitch >> 2) + x;
	pixel2 = (Uint32 *)surf->pixels;

	for ( j = rSurf1.y; j < rSurf1.h; j++ )
	{
		for ( i = rSurf1.x; i < rSurf1.w; i++ )
		{
			if ( *pixel2 != MAGENTA_888 )
				*pixel1 = (*pixel1 & SHADOW_MASK888) >> 1;
				//*pixel1 =  ((src_color & 0xF7DE) >> 1) + ((dst_color & 0xF7DE) >> 1);

			pixel1++;
			pixel2++;
		}

		// premseti pad-a
		pixel1 = (Uint32 *)screen->pixels + x + (y + j) * (screen->pitch >> 2);
		pixel2 = (Uint32 *)surf->pixels + j * (surf->pitch >> 2);
	}

	Sunlock( screen );
	Sunlock( surf );
}

///////////////////////////////////////////////////////////////////////////
//// Name: _ClipRect()
//// Purpose: fit rectangle into world screen
///////////////////////////////////////////////////////////////////////////
int CSdl::ClipRect( int *x , int *y, SDL_Rect *rSurf )
{
	SDL_Rect 	rWld, rResult, rDest;
	int 		tx = *x,
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
// Name: SetRect()
// Desc: set rect dims
///////////////////////////////////////////////////////////////////////
void CSdl::SetRect( SDL_Rect *rect, int x, int y, int width, int height )
{
	rect->x = x;
	rect->y = y;
	rect->w = width;
	rect->h = height;
}

///////////////////////////////////////////////////////////////////////
// Name: MakeBoolMask()
// Desc: make sprte boolean mask
///////////////////////////////////////////////////////////////////////
void CSdl::MakeBoolMask( SDL_Surface *surf, Uint32 *&mask )
{
    if ( bytes_per_color == 2 )
      MakeBoolMask16( surf, mask );
    else //32 bit
      MakeBoolMask32( surf, mask );
}

///////////////////////////////////////////////////////////////////////
// Name: MakeBoolMask16()
// Desc: make sprte boolean mask
///////////////////////////////////////////////////////////////////////
void CSdl::MakeBoolMask16( SDL_Surface *surf, Uint32 *&mask )
{
	Uint32 	width = surf->w;
	Uint32 	height = surf->h;
	
	mask = new Uint32[width * height];

	Slock( surf );
	
	Uint16 *pixel = (Uint16 *)((Uint8 *)surf->pixels );

	for ( Uint32 j = 0; j < height; j++ )
	{
		for ( Uint32 i = 0; i < width; i++, pixel++ )
		{
			Uint32 pos = width * j + i;
			mask[pos] = ( *pixel != magenta16 ) ? 1 : 0;
		}

		// y-padding offset
		pixel = (Uint16 *)((Uint8 *)surf->pixels + j * surf->pitch );
	}

	Sunlock( surf );
}

///////////////////////////////////////////////////////////////////////
// Name: MakeBoolMask32()
// Desc: make sprte boolean mask
///////////////////////////////////////////////////////////////////////
void CSdl::MakeBoolMask32( SDL_Surface *surf, Uint32 *&mask )
{
	Uint32 	width = surf->w;
	Uint32 	height = surf->h;
	
	mask = (Uint32 *) new Uint32[width * height];
	
	Slock( surf );

	Uint32 *pixel = (Uint32 *)surf->pixels;

	for ( Uint32 j = 0; j < height; j++ )
	{
		for ( Uint32 i = 0; i < width; i++, pixel++ )
		{
			Uint32 pos = width * j + i;
			mask[pos] = ( *pixel != MAGENTA ) ? 1 : 0;
		}

		// y-padding offset
		pixel = (Uint32 *)surf->pixels + j * surf->pitch / 4;
	}

	Sunlock( surf );
}


///////////////////////////////////////////////////////////////////////
// Name: Collide()
// Desc: pixel-perfect collision detection using integer masks
///////////////////////////////////////////////////////////////////////
bool CSdl::Collide( SDL_Rect *r1, Uint32 *mask1,
		SDL_Rect *r2, Uint32 *mask2,
		SDL_Rect *rectResult /* = NULL */ )
{
	bool result = false;

	SDL_Rect rt1 = { 0, 0, 0, 0 };
	SDL_Rect rt2 = { 0, 0, 0, 0 };
	SDL_Rect rret = { 0, 0, 0, 0 };
	SDL_Rect rSurf1 = { 0, 0, 0, 0 };
	SDL_Rect rSurf2 = { 0, 0, 0, 0 };

	Sint32 col_width, col_height;
	Sint32 x_off1, y_off1;
	Sint32 x_off2, y_off2;
	Sint32 width1 = 0, width2 = 0;

	SetRect( &rt1, r1->x, r1->y, r1->w, r1->h );
	SetRect( &rt2, r2->x, r2->y, r2->w, r2->h );
    
	width1 = r1->w - r1->x;
	width2 = r2->w - r2->x;

#ifdef _DEBUG
	if ( width1 < 0 || width2 < 0 || 
		 r1->w > 1000 || r1->h > 1000 || 
		 r2->w > 1000 || r2->h > 1000 )
	{
		DBG( "RECTANGLE OVERFLOW! (surf may be corrupted)" );
	}
#endif

	//h1 = r1->h - r1->y;
	//h2 = r2->h - r2->y;

	if ( Collide( &rret, &rt1, &rt2 ) )
	{
		if (rectResult != NULL) {
			rectResult->x = rret.x;
			rectResult->y = rret.y;
			rectResult->w = rret.w;
			rectResult->h = rret.h;
		}

		// get overlappings
		rSurf1.x = rret.x - rt1.x;
		rSurf1.y = rret.y - rt1.y;
		rSurf1.w = rret.w - rt1.x;
		rSurf1.h = rret.h - rt1.y;
		x_off1 = rSurf1.y * width1 + rSurf1.x;
		
		rSurf2.x = rret.x - rt2.x;
		rSurf2.y = rret.y - rt2.y;
		rSurf2.w = rret.w - rt2.x;
		rSurf2.h = rret.h - rt2.y;
		x_off2 = rSurf2.y * width2 + rSurf2.x;

		// get intersection dimensions
		col_width = rret.w - rret.x;
		col_height = rret.h - rret.y;
		
//		DBG("Collision rect => Rect-X: " << rret.x << " Rect-Y:" << rret.y << " W: " << col_width << " H:" << col_height );

		for ( int j = 0; j < col_height; j++ )
		{
			// y - position
			y_off1 = j * width1 + x_off1;
			y_off2 = j * width2 + x_off2;
			Uint32 *pm1 = &mask1[ y_off1 ];
			Uint32 *pm2 = &mask2[ y_off2 ];

			for ( int i = 0; i < col_width; i++, pm1++, pm2++ )
			{
				if ( *pm1 && *pm2 ) {
					result = true;
					break;
				}
			}

			if ( result )
				break;
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
	}

	return result;
}


///////////////////////////////////////////////////////////////////////
// Name: _GetPixel() -> SDL_DOCUMENTATION
// Desc:
///////////////////////////////////////////////////////////////////////
Uint32 CSdl::GetPixel(SDL_Surface *surface, int x, int y)
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
// Name: _Slock()
// Desc: Locks a surface for direct access.
///////////////////////////////////////////////////////////////////////
inline bool CSdl::Slock( SDL_Surface *surface )
{
	if ( SDL_MUSTLOCK( surface ) )
	{
		// returns 0, or -1 if the surface couldn't be locked.
		if ( SDL_LockSurface( surface ) <= 0 )
			return false;
	}

	return true;
}


///////////////////////////////////////////////////////////////////////
// Name: _Sunlock()
// Desc: Unlocks locked surface.
///////////////////////////////////////////////////////////////////////
inline void CSdl::Sunlock( SDL_Surface *surface )
{
	if ( SDL_MUSTLOCK( surface ) )
	{
		SDL_UnlockSurface( surface );
	}
}


///////////////////////////////////////////////////////////////////////
// Name: Initialize()
// Desc: Inicializira Sdl i prewkliuchwa video rejima
///////////////////////////////////////////////////////////////////////
bool CSdl::Initialize( CGame *game, int nWidth, int nHeight, int nBpp, bool bFullscreen, bool bHardware )
{
	char temp[256] 	= { 0 };

	this->_game = game;
	ASSERT( _game != NULL );

	LOG("Initializing SDL ..." );

	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		fprintf (stderr, "Couldn't initialize SDL: %s\n", SDL_GetError() );
		LOG( "SDL Error: ...failed to open SDL :  " << SDL_GetError() );
		return false;
	}

	// get video capabilities
	SDL_VideoDriverName( temp, 256 );
	LOG( "Video Driver: " << temp );
	
	SDL_VideoInfo *vid_info =(SDL_VideoInfo *) SDL_GetVideoInfo();
	LOG( "Total Video Memory: " << vid_info->video_mem );
	LOG( "hw_available: " << vid_info->hw_available );
	LOG( "wm_available: " << vid_info->wm_available );
	LOG( "blit_hw: " << vid_info->blit_hw );
	LOG( "blit_hw_CC: " << vid_info->blit_hw_CC );
	LOG( "blit_hw_A: " << vid_info->blit_hw_A );
	LOG( "blit_sw: " << vid_info->blit_sw );
	LOG( "blit_sw_CC: " << vid_info->blit_sw_CC );
	LOG( "blit_sw_A: " << vid_info->blit_sw_A );

// 	// check video memory (Note: This seems to be stupi..uuhm... no longer valid !)
// 	if ( vid_info->video_mem > 24000U )
// 		flags |= SDL_HWSURFACE;
// 	else
// 		flags |= SDL_SWSURFACE;

	// init na glawanata powyrhnost i video rejim
	Uint32 flags = 0 ;
	
	if ( bFullscreen )
	  flags |= SDL_FULLSCREEN;

	flags |= bHardware ? SDL_HWSURFACE : SDL_SWSURFACE;

	// enumerate available video modes
	SDL_Rect **rModes = NULL;
	rModes = SDL_ListModes( NULL, flags );
	if ( (SDL_Rect **)NULL == rModes )
	{
		LOG( "SDL Error: Unable to enumerate video modes! " );
		return false;
	}
	else if ( (SDL_Rect **)-1 == rModes )
	{
		LOG( "All modes are available." );
	}
	else
	{
		LOG( "Listing available video modes:" );
		for( int i = 0; rModes[i]; i++ )
		{
			char buf[256];
			sprintf( buf, "Mode: %dx%d is OK", rModes[i]->w, rModes[i]->h );
			LOG( buf );
		}
	}

	// switch to desired video mode
	flags |= SDL_DOUBLEBUF | SDL_ANYFORMAT;

	int vd = SDL_VideoModeOK( nWidth, nHeight, nBpp, flags ); 
	LOG( "SDL_VideoModeOK() recommends " << vd << " bit mode." );
	
	if ( vd == 24 )
	{
		LOG( "Error: Sorry, Savage Wheels does not support 24 bit video mode !" );
		return false;	  
	}

	LOG( "Switching video mode to " << nWidth << "x" << nHeight << "x" << nBpp );
	if ( NULL == (screen = SDL_SetVideoMode( nWidth, nHeight, nBpp, flags )) )
	{
		LOG( "SDL Error: Setting video mode : " << SDL_GetError() );
		return false;
	}
	
	SDL_SetClipRect( screen, NULL );
	SDL_WM_SetCaption( _game->GetWindowTitle().c_str(), "None" );
	SDL_ShowCursor( SDL_DISABLE );	

	// check for 16-bit color mode
	bytes_per_color = screen->format->BytesPerPixel;
	magenta16 = MAGENTA_565; // 24&32bit
	shadow_mask16 = SHADOW_MASK565; //24&32bit

	// check for 16/15 bit mode
	if ( bytes_per_color == 2 )
	{
		bytes_per_color = screen->format->BytesPerPixel;
		if ( screen->format->Rmask == 0xF800 )
		{
			magenta16 = MAGENTA_565;
			shadow_mask16 = SHADOW_MASK565;
		}
		else
		{
			magenta16 = MAGENTA_555;
			shadow_mask16 = SHADOW_MASK555;
		}
	}

	LOG("SDL initialized successfully.");

	InitializeSound();

	return true;
}

///////////////////////////////////////////////////////////////////////
// Name: InitializeSound()
// Desc: Initialize sound support system
///////////////////////////////////////////////////////////////////////
bool CSdl::InitializeSound()
{
#ifdef WITH_FMOD
	LOG("Initializing FModEx ..." );

	FMOD_RESULT 	result;
	unsigned int	version;

    result = FMOD_System_Create(&fmod_system);
    if (IsFModOK(result)) {
    	result = FMOD_System_GetVersion(fmod_system, &version);
    	bsound_initialized = IsFModOK(result);
    }

    if (!bsound_initialized)
    {
    	LOG("FModEx failed to initialize! Game will start without sound.");
    }
    else if ( version < FMOD_VERSION )
	{
		LOG("FModEx Error: You are using an old FMOD version " << version << "! Required version is " << FMOD_VERSION);
		bsound_initialized = false;
	}
	else
	{
//		bsound_initialized = true;

#ifdef LINUX_BUILD
		// ALSA is preferred for FMod on Debian
		if ( getenv("SW_SND_ALSA") && !strcmp( getenv("SW_SND_ALSA"), "1" ) )
		{
			result = FMOD_System_SetOutput(fmod_system, FMOD_OUTPUTTYPE_ALSA);
			IsFModOK(result);
		}
#endif
		int mixrate = getenv("SW_SND_22KHZ") && !strcmp( getenv("SW_SND_22KHZ"), "1" ) ? 22050 : 44100;
		// mixrate ???

		result = FMOD_System_Init(fmod_system, 32, FMOD_INIT_NORMAL, NULL);
		if(!IsFModOK(result))
		{
			LOG("FModEx Error: Failed to initialize !");
			bsound_initialized = false;
		}
		else
		{
			// Get driver information

			int driver = -1;
			result = FMOD_System_GetDriver(fmod_system, &driver);
			if (IsFModOK(result)) {

				char driver_name[255];

				result = FMOD_System_GetDriverInfo(fmod_system, driver, driver_name, 255, NULL);
				if (IsFModOK(result)) {
					LOG( "FModEx Driver: " << driver_name);
				}

			}

			//sprintf( temp, "%s %d %d", FSOUND_GetDriverName( FSOUND_GetDriver() ),
			//	FSOUND_GetOutputRate(),
			//	FSOUND_GetMaxChannels()
			//	);
			//memset( this->sounds, 0, sizeof(CSound) * MAX_SOUNDS );

			result = FMOD_System_CreateSoundGroup(fmod_system, FMOD_SNDGROUP_SOUNDS,
					&fmod_groupSounds);
			IsFModOK(result);
			result = FMOD_System_CreateSoundGroup(fmod_system, FMOD_SNDGROUP_MUSIC,
					&fmod_groupMusic);
			IsFModOK(result);

			LOG("FModEx initialized successfully.");

			// set default volume
			volume_sound = 255;
			volume_music = 255;
		}
	}
#else

	// Nothing else is supported for sound right now.

	LOG("No sound support! Game was compiled without sound support.");

	bsound_initialized = false;

#endif

	return bsound_initialized;
}


///////////////////////////////////////////////////////////////////////
// Name: InitializeJoystick()
// Desc: try to setup joystick support
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
// Name: AcquireJoystick()
// Desc: get access to joystick device
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
// Name: UnAcquireJoystick()
// Desc: release access to joystick device
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
// Name: ReleaseJoystick()
// Desc:
///////////////////////////////////////////////////////////////////////
void CSdl::ReleaseJoystick()
{
	if ( _bJoystickSupport )
		UnAcquireJoystick();
}


///////////////////////////////////////////////////////////////////////
// Name: LoadBitmap() {1}
// Desc: zarejda SDL surface ot bitmap
///////////////////////////////////////////////////////////////////////
SDL_Surface* CSdl::LoadBitmap( const char *filename, Uint32 color_key, Uint8 alpha_value )
{

#ifndef LOCAL_RES

	String strFilename( ExtractFilename( filename ) );

	long offset = _game->Kdf.GetFilePosition( strFilename.c_str() );
	
	if ( KDF_ERROR_FILEDOESNOTEXIST != offset )
	{
	  Uint32 size = _game->Kdf.GetFileSize( strFilename.c_str() );
	  
	  return LoadBitmap( ART_FILE, offset, size, color_key, alpha_value );
	}
	
	LOG( "Error loading " << filename << " !" );

	return NULL;
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
// Name: _LoadBitmap() {2}
// Desc: zarejda SDL surface ot bitmap v BINRreource file
///////////////////////////////////////////////////////////////////////
SDL_Surface* CSdl::LoadBitmap( const char *filename, int32_t file_offset, Uint32 file_size, Uint32 color_key, Uint16 alpha_value )
{
	SDL_Surface  *sdl_surf	= NULL;						// temp surface
	FILE		 *fp		= NULL;						// file pointer
	SDL_RWops	 *sdl_rw	= NULL;						// sdl_read_write_operations

	if ( ( fp = fopen( filename, "rb")) == NULL ) 
	{
		LOG("...failed to open file : " << filename );
		return NULL;
	}

	fseek( fp, file_offset, SEEK_CUR );

//	if ( ( sdl_rw = SDL_RWFromMem( pimg, file_size )) == NULL )
	if ( ( sdl_rw = SDL_RWFromFP( fp, 1 )) == NULL )
	{
		LOG("...failed to create RWops with : " << filename << " Error: " << SDL_GetError());
		return NULL;
	}

	// load bitmap from memory handler
	if ( ( sdl_surf = SDL_LoadBMP_RW( sdl_rw, 1 )) == NULL ) 
	{
		LOG("...failed to load surface into memory from : " << filename
				<< " Error: " << SDL_GetError());
		return NULL;
	}

	// set color key & alpha BEFORE setting DisplayFormat!
	if ( color_key != NO_COLORKEY )
		SDL_SetColorKey( sdl_surf, SDL_SRCCOLORKEY, color_key );

	if ( alpha_value != NO_ALPHA )
		SDL_SetAlpha( sdl_surf, SDL_SRCALPHA, (Uint8)alpha_value );
	
	SDL_Surface *new_surf = SDL_DisplayFormat( sdl_surf );
	SDL_FreeSurface( sdl_surf );
	sdl_surf = NULL;
	
//	DBG( "Loaded surface from " << filename << " Pos: " << file_offset << " W: " << new_surf->w << " H: " << new_surf->h );
	
	return new_surf;
}



///////////////////////////////////////////////////////////////////////
// Name: CreateEmptySurface()
// Desc: syzdava prazna powyrhnost
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
// Name: CreateColor()
// Desc: color+alpha
///////////////////////////////////////////////////////////////////////
SDL_Color CreateColor( int r, int g, int b, int a )
{
	SDL_Color clr = { r, g, b, a};
	return clr;
}


///////////////////////////////////////////////////////////////////////
// Name: CreateColor()
// Desc: color
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
// Name: InitializeFont()
// Desc: Inicializira TTF font
///////////////////////////////////////////////////////////////////////
void CSdl::InitializeFont( int fontsize )
{
	TTF_Init();
	atexit(TTF_Quit);

	// open the font file
	font_ttf = TTF_OpenFont( "fonts/comic.ttf", fontsize );

}



///////////////////////////////////////////////////////////////////////
// Name: DrawText()
// Desc: will draw text on position with background color
///////////////////////////////////////////////////////////////////////
void CSdl::DrawText( int x, int y, char *text, SDL_Color forecolor, SDL_Color backcolor )
{

	SDL_Surface *txtsurf = TTF_RenderText_Shaded( font_ttf, text, forecolor, backcolor );
	SDL_Rect rDest = { x, y, 0, 0 };
	SDL_BlitSurface( txtsurf, NULL, screen, &rDest );
	RELEASE_SURF( txtsurf );
}



///////////////////////////////////////////////////////////////////////
// Name: DrawText()
// Desc: will draw text on position
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
// Name: InitializeFont()
// Desc: Inicializira Bitmap font
///////////////////////////////////////////////////////////////////////
void CSdl::InitializeFont()
{
	//...
	

#ifdef LOCAL_RES
	font_bmp = LoadBitmap( "gfx/interf/fnt.bmp", 0x0 );
#else // LOAD FROM KDF
	long position = _game->Kdf.GetFilePosition("fnt.bmp");
	if ( KDF_ERROR_FILEDOESNOTEXIST != position )
	  font_bmp = LoadBitmap( ART_FILE, _game->Kdf.GetFilePosition("fnt.bmp"), _game->Kdf.GetFileSize("fnt.bmp") , 0x0 );
#endif

	if ( font_bmp == NULL )
	{
		AppendToLog("Error loading font file!");
	}
	
	font_size = 8; //fontsize;  // 7.5
}



///////////////////////////////////////////////////////////////////////
// Name: DrawText()
// Desc: will draw text on position
///////////////////////////////////////////////////////////////////////
void CSdl::DrawNum( int x, int y, char *text )
{
	SDL_Rect rdst;
	SDL_Rect rsrc;

	//16x16  50=ascii53 48
	font_size = 10;
//43= +,-./0
	while ( *text != '\0'  )
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

#ifdef WITH_FMOD
inline bool CSdl::IsFModOK(FMOD_RESULT result) {
	if (result != FMOD_OK)
	{
		LOG( "FMOD error! (" << result << ") " << FMOD_ErrorString(result) );
//				<< " in " << __FILE__ << " on line " << __LINE__);
		return false;
	}
	return true;
}
#endif

///////////////////////////////////////////////////////////////////////
// Name: LoadSound()
// Desc:
///////////////////////////////////////////////////////////////////////
int CSdl::LoadSound( const char *filename, bool buffered_sound, bool IsStream )
{
#ifdef WITH_FMOD
	if ( !bsound_initialized )
		return -1;

	static int		cur_channel	= 0;		// TODO: remove the 'static'
	Uint32			index		= 0;
	CSound			*ptr_snd	= sounds;
	bool			bsnd_loaded	= false;
	FMOD_RESULT		result;

	while ( index < MAX_SOUNDS )
	{
		if ( !ptr_snd->loaded )
		{
			FMOD_SOUNDGROUP *bindingGroup;

			if (IsStream)
			{
				result = FMOD_System_CreateSound(fmod_system,
						filename,
						FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL,
						0,
						&ptr_snd->sound);

				bindingGroup = fmod_groupMusic;
			}
			else
			{
				result = FMOD_System_CreateSound(fmod_system,
						filename,
						FMOD_DEFAULT,
						0,
						&ptr_snd->sound);

				bindingGroup = fmod_groupSounds;
			}

			if (!IsFModOK(result))
			{
				LOG("...failed to load sound file : " << filename );
				return -1;
			}
			
			// bind to desired sound group
			result = FMOD_Sound_SetSoundGroup(ptr_snd->sound, bindingGroup);
			IsFModOK(result);

			// disable loop by default
			FMOD_Sound_SetLoopCount(ptr_snd->sound, 0);

			if ( !buffered_sound )
			{
				/*
				 * FIXME:
				 * This does not seem to work as intended since FMod Ex !
				 * Find another way to reserve a channel instead of using pure int indexes.
				 */
				ptr_snd->buffered = false;
				ptr_snd->play_channel = cur_channel;
				cur_channel++;
			}
			else
			{
				ptr_snd->buffered = true;
			}
			
			ptr_snd->isMusic = IsStream;
			ptr_snd->loaded = true;

			bsnd_loaded = true;
			break;
		}

		index++;
		ptr_snd++;
	}

	if ( !bsnd_loaded ) 
	{
		LOG("Sound load error: No slots available!");
		return -1;
	}

	return index;
#else
	return -1;
#endif
}


/*
///////////////////////////////////////////////////////////////////////
// Name: LoadWav()
// Desc: load wav from file
///////////////////////////////////////////////////////////////////////
Mix_Chunk* CSdl::LoadWav( char *filename, long file_offset, Uint32 file_size )
{

	Mix_Chunk    *chunk;
	FILE		 *fp;
	char		 *psnd;							// sound  data
	SDL_RWops	 *sdl_rw;						// sdl_read_write_operations


	if ( ( fp = fopen( filename, "rb")) == NULL ) 
	{
		LOG("...failed to load wav from : ", filename, NULL );
		return NULL;
	}

	fseek( fp, file_offset, SEEK_CUR );
	// info->mem
	psnd = new char[file_size];
	fread( psnd, file_size, 1, fp );
	fclose( fp );

	// create read/write op
	if ( ( sdl_rw = SDL_RWFromMem( psnd, file_size )) == NULL ) 
	{
		AppendToMultilog("...failed to create RWops with : ", filename, NULL );
		return NULL;
	}

	if ( ( chunk = Mix_LoadWAV_RW( sdl_rw, 1 )) == NULL ) 
	{
		LOG("...failed to load wav into memory from : ", filename, NULL );
		return NULL;
	}

	delete[] psnd;

	return chunk;
}

*/

///////////////////////////////////////////////////////////////////////
// Name: PlaySound()
// Desc: play sound at position ( for 640x480 res )
///////////////////////////////////////////////////////////////////////
void CSdl::PlaySound( int snd_index, int position )
{
#ifdef WITH_FMOD
	if ( !bsound_initialized || GetSoundVolume() <= 0 )
		return;

	// calculate sound position pan
	//	A left/right pan level, from -1.0 to 1.0 inclusive.
	// -1.0 = Full left, 0.0 = center, 1.0 = full right.
	// Default = 0.0.

	float pos = 0.0f;

	if ( position != -1 )
	{
		pos = fRangeGetXY(position, 0, 640, -1.0f, 1.0f);
 	}

	FMOD_CHANNEL 	*channel;

	if ( sounds[snd_index].buffered )
	{
		// find a free channel to play the sound
//		for ( Uint8 i = 0; i < 255; i++ )
//		{
			FMOD_BOOL is_playing = 0;
//			FMOD_System_GetChannel(fmod_system, i, &channel);
//			FMOD_Channel_IsPlaying(channel, &is_playing);
//			if ( !is_playing )
//			{
				FMOD_System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, sounds[snd_index].sound, 0, &channel);
				FMOD_Channel_SetPan(channel, pos);
//				chn = FSOUND_PlaySound( i, sounds[snd_index].sound );
//				FSOUND_SetPan( chn , position );
//				break;
//			}
//		}
	}
	else
	{
		// play through unbuffered channel
		//FSOUND_StopSound( sounds[snd_index].play_channel );
		FMOD_System_GetChannel(fmod_system, sounds[snd_index].play_channel, &channel);
		FMOD_System_PlaySound(fmod_system, FMOD_CHANNEL_REUSE, sounds[snd_index].sound, 0, &channel);
		FMOD_Channel_SetPan(channel, pos);
	}
#endif
}


///////////////////////////////////////////////////////////////////////
// Name: PlayMusic()
// Desc: Plays a specified sound in the music channel
///////////////////////////////////////////////////////////////////////
void CSdl::PlayMusic(int snd_index, bool looped)
{
#ifdef WITH_FMOD
	if ( !bsound_initialized || GetMusicVolume() <= 0 )
		return;

	FMOD_RESULT result;

	DBG("Playing music idx " << snd_index);
//	if (fmod_musicChannel != NULL )
//	{
//		result = FMOD_Channel_Stop(fmod_musicChannel);
//		FM_OK(result);
//	}

	result = FMOD_System_PlaySound(fmod_system,
			FMOD_CHANNEL_REUSE, //FMOD_CHANNEL_FREE
			sounds[snd_index].sound,
			0,
			&fmod_musicChannel);
	FM_OK(result);

	result = FMOD_Channel_SetLoopCount(fmod_musicChannel, looped ? -1 : 0);
	FM_OK(result);
	result = FMOD_Channel_SetPosition(fmod_musicChannel, 0, FMOD_TIMEUNIT_MS);
	FM_OK(result);
//	result = FMOD_Channel_SetPriority(fmod_musicChannel, 255);
//	FM_OK(result);
#endif
}


///////////////////////////////////////////////////////////////////////
// Name: StopMusic()
// Desc: Stop sound playing in the music channel.
///////////////////////////////////////////////////////////////////////
void CSdl::StopMusic()
{
#ifdef WITH_FMOD
	DBG("StopMusic");
	if ( !bsound_initialized )
		return;

	if (fmod_musicChannel != NULL )
	{
		FMOD_RESULT result = FMOD_Channel_Stop(fmod_musicChannel);
		FM_OK(result);
		fmod_musicChannel = NULL;
	}
#endif
}

///////////////////////////////////////////////////////////////////////
// Name: IsMusicPlaying()
// Desc: Checks if there is a sound currently being played in the
// music channel.
///////////////////////////////////////////////////////////////////////
bool CSdl::IsMusicPlaying()
{
#ifdef WITH_FMOD
	if ( !bsound_initialized )
		return false;

	FMOD_RESULT result;
	FMOD_BOOL 	is_playing = 0;

	if (fmod_musicChannel != NULL )
	{
		result = FMOD_Channel_IsPlaying(fmod_musicChannel, &is_playing);
		if (FM_OK(result)) {
//			if (!is_playing)
//				DBG("valid channel but not playing...");
			return (bool) is_playing;
		}
	}
#endif

	return false;
}


///////////////////////////////////////////////////////////////////////
// Name: SetMusicVolume()
// Desc:
///////////////////////////////////////////////////////////////////////
void CSdl::SetMusicVolume( int new_vol )
{
#ifdef WITH_FMOD  
	if ( new_vol > 256 )
		new_vol = 256;
	else if ( new_vol < 0 ) 
		new_vol = 0;

	volume_music = new_vol; // 0 - 256 range
	float fineVol = fRangeGet0255(volume_music, 0.0f, 1.0f);

	DBG("Setting Music volume to " << volume_music << " / fine volume " << fineVol);

	FMOD_RESULT result = FMOD_SoundGroup_SetVolume(fmod_groupMusic, fineVol);
	FM_OK(result);
//	if (fmod_musicChannel) {
//		FMOD_RESULT result = FMOD_Channel_SetVolume(fmod_musicChannel, fineVol);
//		FM_OK(result);
//	}
#endif
}


///////////////////////////////////////////////////////////////////////
// Name: SetSoundVolume()
// Desc:
///////////////////////////////////////////////////////////////////////
 void CSdl::SetSoundVolume( int new_vol )
{
#ifdef WITH_FMOD
	if ( new_vol > 256 ) 
		new_vol = 256;
	else if ( new_vol < 0 ) 
		new_vol = 0;

	volume_sound = new_vol;
	float fineVol = fRangeGet0255(volume_sound, 0.0f, 1.0f);

	DBG("Setting Sound volume to " << volume_sound << " / fine volume " << fineVol);

	FMOD_RESULT result = FMOD_SoundGroup_SetVolume(fmod_groupSounds, fineVol);
	FM_OK(result);

//	FMOD_CHANNELGROUP *channelGroup;
//	FMOD_RESULT result = FMOD_System_GetMasterChannelGroup(fmod_system, &channelGroup);
//	if (FM_OK(result)) {
//		result = FMOD_ChannelGroup_SetVolume(channelGroup, fineVol);
//		FM_OK(result);
//	}
#endif
}

///////////////////////////////////////////////////////////////////////
// Name: ChangeSoundVolume()
// Desc:
///////////////////////////////////////////////////////////////////////
void CSdl::ChangeSoundVolume( int s_vol ) 
{ 
#ifdef WITH_FMOD
	SetSoundVolume( volume_sound + s_vol ); 
#endif
};

 ///////////////////////////////////////////////////////////////////////
// Name: ChangeMusicVolume()
// Desc:
///////////////////////////////////////////////////////////////////////
void CSdl::ChangeMusicVolume( int m_vol ) 
{ 
#ifdef WITH_FMOD
	SetMusicVolume( volume_music + m_vol ); 
#endif
};


/*///////////////////////////////////////////////////////////////////////
// Name: PlaySound()
// Desc:
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
// Name: Release()
// Desc: Release memory
///////////////////////////////////////////////////////////////////////
void CSound::Release()
{
#ifdef WITH_FMOD
	if ( sound )
	{
		DBG( "Releasing FMod sample data ..." );
		FMOD_RESULT result = FMOD_Sound_Release(sound);
		FM_OK(result);

		loaded = false;
	}
#endif
}
