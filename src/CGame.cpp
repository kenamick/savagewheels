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
// Name: CGame()
// Desc:
//////////////////////////////////////////////////////////////////////
CGame::CGame() 
:	mpf( 0.0f ),
	bRunning( true ),
    quit_dialog( false ),
	show_fps( false ),
	game_paused( false ),
	game_cheats( false ),
	game_num_cars( 0 ),
	game_bots( false ),
	game_frags( 1 ),
	p1_auto_index( 0 ),
	p2_auto_index( 0 ),
	cur_map ( 0 ),
	player_level( 1 ),
	Gameplayers( GP_1PLAYER ),
	Gametype( GT_FRAGS ),
	Gamemode( GM_1BOT_MELEE ),
	Gamestate( GS_MENU ),
	game_hitmode( false )
{ 

	for( int cn = 0; cn < NUM_BACKGROUNDS; cn++ )
		back[cn] = NULL;

	for( int i = 0; i < 2; i++ )
		scales[i] = NULL;

	for( int i = 0; i < 5; i++ )
		but_quit[i] = NULL;

	self_dest = but_pause = pause_surf = winneris = splash = getready = NULL;

	for( int i = 0; i < 30; i++ )
		splash_logo[i] = NULL;
}

//////////////////////////////////////////////////////////////////////
// Name: ~CGame()
// Desc:
//////////////////////////////////////////////////////////////////////
CGame::~CGame()
{
 
}


///////////////////////////////////////////////////////////////////////
// Name: Close()
// Desc: close game...
///////////////////////////////////////////////////////////////////////
void CGame::Close()
{
	int cn = 0;

	AppendToLog( LOG_DASH );
	AppendToLog("Closing Game() class...");

	for( cn = 0; cn < NUM_BACKGROUNDS; cn++ )
		RELEASE_SURF( back[cn] );

	RELEASE_SURF( scales[0] );
	RELEASE_SURF( scales[1] );

	for ( cn = 0; cn < 5; cn++ )
		RELEASE_SURF( but_quit[cn] );

	RELEASE_SURF( but_pause );
	RELEASE_SURF( winneris );
	RELEASE_SURF( self_dest );
	// RELEASE_SURF( sprite_tire[0] );
	RELEASE_SURF( getready );

	for( cn = 0; cn < game_num_cars; cn++ )
		Auto[cn].Release();

	Mines.Release();
	Anims.Release();
	Dtoys.Release();

	// release menu
	Menu.Release();

	Snd.Release();
	Swv.Release();
	Sdl.Close();
	Bindings.Release();
}


///////////////////////////////////////////////////////////////////////
// Name: LoadGame()
// Desc: Zarejdane na igrata (grafika, zwuk, clasove i t.n.)
///////////////////////////////////////////////////////////////////////
bool CGame::LoadGame()
{
	char buf[255];
	int  i = 0;

	AppendToLog( LOG_DASH );
	AppendToLog("Loading Game...");

	// global screen rect
	rScreen.x = 23;
	rScreen.y = 19;
	rScreen.w = 614;
	rScreen.h = 408;

	// open KDF
	if ( KDF_SUCCESS != Kdf.Open( ART_FILE ) )
	{
		LOG( "Data file missing: " << ART_FILE );
		return false;
	}

	// load bindings
	if ( ! Bindings.Load( BINDINGS_FILE ) )
	{
		LOG( "Bindings file missing: " << BINDINGS_FILE );
		return false;
	}

	// zaredi font-a
	Sdl.InitializeFont();

	// load splash screen
	splash = Sdl.LoadBitmap( "gfx/interf/splash.bmp" );

	for ( i = 1; i < 31; i++ )
	{
		sprintf( buf, "gfx/swlogo/sw%d.bmp", i );
		if ( ( splash_logo[i - 1] = Sdl.LoadBitmap( buf, BLACK ) ) == NULL ) 
			return false;
	}

	UpdateSplash();  // UPDATESPLASH...  1

	// randomize seed
	srand( (unsigned)SDL_GetTicks() );

	UpdateSplash();  // UPDATESPLASH... 

	// search for vehicles
	if ( Swv.SearchAndLoad( "autos" ) != SWV_SUCCESS ) return false;

	// check for vehicle number
	if ( Swv.GetVehicles() < 4 ) 
	{
		AppendToLog( "Not enough vehicles in the /auto dir!" );
		AppendToLog( "At leaset 4 vehicle-files needed." );
		return false;
	}

	UpdateSplash();  // UPDATESPLASH... 

	// load sounds & music
	if ( ! Snd.Initialize( &Sdl ) )
	{
		AppendToLog( "...failed to load sfx. Missing sound/music files!" );
		//return false;
	}
	else
	{
		AppendToLog( "Loading sfx...successful!" );
	}

	UpdateSplash();  // UPDATESPLASH... 

	/*// load menu
	if ( !Menu.Initialize() ) 
	{
	AppendToLog( "...failed to initialize CMainMenu() class!" );
	return false;
	}*/

	UpdateSplash();  // UPDATESPLASH... 

	this->Gamestate	= GS_MENU;
	this->game_shadows = true;

	// {! -> to add}
	if ( ( getready = Sdl.LoadBitmap( "gfx/interf/getready.bmp" )) == NULL ) return 0;  
	UpdateSplash();

	// if ( (sprite_tire[0] = Sdl.LoadBitmap( "gfx/tire/tire0.bmp", MAGENTA )) == NULL ) return 0; 
	// UpdateSplash();  // UPDATESPLASH... 

	// {!}

	if ( (back[0] = Sdl.LoadBitmap( "gfx/interf/back.bmp" )) == NULL ) return false;
	UpdateSplash();  // UPDATESPLASH... 
	if ( (back[1] = Sdl.LoadBitmap( "gfx/interf/map1.bmp" )) == NULL ) return false;
	UpdateSplash();  // UPDATESPLASH... 
	if ( (back[2] = Sdl.LoadBitmap( "gfx/interf/map2.bmp" )) == NULL ) return false;
	UpdateSplash();  // UPDATESPLASH... 
	if ( (back[3] = Sdl.LoadBitmap( "gfx/interf/map3.bmp" )) == NULL ) return false;
	UpdateSplash();  // UPDATESPLASH... 
	if ( (back[4] = Sdl.LoadBitmap( "gfx/interf/map5.bmp" )) == NULL ) return false;
	UpdateSplash();  // UPDATESPLASH... 
	if ( (scales[0] = Sdl.LoadBitmap( "gfx/interf/sk1.bmp" )) == NULL ) return false;
	UpdateSplash();  // UPDATESPLASH... 
	if ( (scales[1] = Sdl.LoadBitmap( "gfx/interf/sk2.bmp" )) == NULL ) return false;
	UpdateSplash();  // UPDATESPLASH... 
	//if ( (but_quit = Sdl.LoadBitmap( "gfx/interf/butquit.bmp" )) == NULL ) return false;
	for ( i = 0; i < 5; i++)
	{
		sprintf( buf, "gfx/interf/qg%d.bmp", i );
		if ( (but_quit[i] = Sdl.LoadBitmap( buf, MAGENTA )) == NULL ) return false;
	}
	UpdateSplash();  // UPDATESPLASH... 
	if ( (but_pause = Sdl.LoadBitmap( "gfx/interf/butpaz.bmp", MAGENTA )) == NULL ) return false; 
	UpdateSplash();  // UPDATESPLASH... 
	if ( (winneris = Sdl.LoadBitmap( "gfx/interf/winner.bmp", BLACK )) == NULL ) return false;
	UpdateSplash();  // UPDATESPLASH...  16
	if ( (self_dest = Sdl.LoadBitmap( "gfx/interf/selfd.bmp", BLACK )) == NULL ) return false;

	pause_surf = Sdl.CreateEmptySurface( 640, 480 );

	UpdateSplash();  // UPDATESPLASH... 

	// inicializirai Clasovete
	if ( !Dtoys.Initialize( this ) ) 
	{
		AppendToLog( "...failed to initialize CDeadToys() class!" );
		return false;
	}
	else
		AppendToLog( "Initializing CDeadToys() class...successful." );

	UpdateSplash();  // UPDATESPLASH... 
	UpdateSplash();  // UPDATESPLASH... 
	UpdateSplash();  // UPDATESPLASH... 
	UpdateSplash();  // UPDATESPLASH... 

	if ( !Anims.Initialize( this ) ) 
	{
		AppendToLog( "...failed to initialize CAnimations() class!" );
		return false;
	}
	else
		AppendToLog( "Initializing CAnimations() class...successful." );

	UpdateSplash();  // UPDATESPLASH... 
	UpdateSplash();  // UPDATESPLASH... 
	UpdateSplash();  // UPDATESPLASH... 
	UpdateSplash();  // UPDATESPLASH... 

	if ( !Mines.Initialize( this ) ) 
	{
		AppendToLog( "...failed to initialize CLandMines() class!" );
		return false;
	}
	else
		AppendToLog( "Initializing CLandMines() class...successful." );

	UpdateSplash();  // UPDATESPLASH... 21 
	UpdateSplash();  // UPDATESPLASH... 
	UpdateSplash();  // UPDATESPLASH... 
	UpdateSplash();  // UPDATESPLASH... 

	/*UpdateSplash();  // UPDATESPLASH... 
	UpdateSplash();  // UPDATESPLASH...  
	UpdateSplash();  // UPDATESPLASH... 
	UpdateSplash();  // UPDATESPLASH... 
	UpdateSplash();  // UPDATESPLASH... 
	UpdateSplash();  // UPDATESPLASH... 
	UpdateSplash();  // UPDATESPLASH... 
	UpdateSplash();  // UPDATESPLASH... 
	UpdateSplash();  // UPDATESPLASH... */

	/*    // Za GAMEPLAY_only  
	game_frags = 43;
	Gameplayers = GP_1PLAYER;
	game_bots = true;
	//p1_auto_index = 1;
	p1_auto_index = 0;
	Gamestate = GS_GAMEPLAY_SETUP;
	*/

	// release splash screen
	RELEASE_SURF( splash );
	for ( i = 0; i < 30; i++ )
		RELEASE_SURF( splash_logo[i] );

	// load menu
	if ( ! Menu.Initialize( this ) ) 
	{
		AppendToLog( "...failed to initialize CMainMenu() class!" );
		return false;
	}

	Menu.LoadSettings();

	Snd.Play( MUS_MENU, true );

	return true;
}



///////////////////////////////////////////////////////////////////////
// Name: UpdateSplash()
// Desc: update splash screen on loading
///////////////////////////////////////////////////////////////////////
void CGame::UpdateSplash()
{

	static int cur_frame = 0;
 
	Sdl.BlitNow( 0, 0, splash );
	Sdl.BlitNow( 250, 90, splash_logo[cur_frame] );
	Sdl.Flip();

	cur_frame++;
	if ( cur_frame >= 30 ) cur_frame = 29;
}



///////////////////////////////////////////////////////////////////////
// Name: Execute()
// Desc:
///////////////////////////////////////////////////////////////////////
void CGame::Execute( bool bFullScreen, bool bHardware )
{
  
  float    ftimer			= 0.0f;
  float    ftimediff			= 0.0f;
  float    ffpstime			= 0.0f;
  int	   fps				= 0, 
		   frames		= 0;
  char	   szfps[3]			= { '0', '0' };
  int      i				= 0;
  int	   dx				= 0, 
		   dy			= 0;
  Uint32   winner_index			= 0U, 
		   winner_index2	= 0U;
  Uint32   time_destroyem		= 0U;
  Uint32   time_showwinner		= 0U;
  SDL_Rect rtemp;
  bool	   change_map			= false;
  bool	   change_map_key		= false;
  bool	   change_shadows		= false;
  bool	   change_shadows_key		= false;
  bool	   game_paused_key		= false;
  bool	   menu_escape_key		= false;
  bool	   music_off			= false;
  bool	   music_off_key		= false;
  bool	   toggle_fullscreen		= false;
  bool	   game_end			= false;
  float    qg_frame			= 0.0f;
  bool	   wait_key			= false;
  Uint32   time_round			= 0U, 
		   tv			= 0U;
  bool	   at_exit			= false;
  CVehicle *ptr_veh			= NULL;

  p1_auto_index = 99;
  p2_auto_index = 99;
  
  // inicializirai prozoreca
  bRunning = Sdl.Initialize( this, 640, 480, 16, bFullScreen, bHardware );

  Sdl.InitializeJoystick();
  Sdl.AcquireJoystick();
 // bRunning = false;
  //exit(0);

  if ( bRunning )
  {
	  if ( bRunning = (bool)LoadGame() )
		  AppendToLog("Starting Game loop...");
	  
	  ftimer = (float)SDL_GetTicks();
  }

  while( bRunning )
  {

	// do hires timing
	ftimediff = SDL_GetTicks() - ftimer;
	mpf = ftimediff / 1000.0f;
	ftimer = (float)SDL_GetTicks();
	// calculate fps
	if ( ftimer - ffpstime >= 1000 )
	{
		fps = frames;
		frames = 0;
		ffpstime = (float)SDL_GetTicks();
	}
	frames++;
	//fps = 1.0 / mpf;

  
	// get input  
	Sdl.GetInput(); 
	//if ( Sdl.keys[SDLK_ESCAPE] ) 
	//bRunning = false;

	// map-change
	if ( Sdl.keys[SDLK_F5] && !change_map_key ) 
	{
		change_map_key = true;
		change_map = true;
	
		if ( change_map )
		{
			cur_map++;
			if ( cur_map >= NUM_BACKGROUNDS ) cur_map = 1;
			change_map = false;
		}
	}
	if ( !Sdl.keys[SDLK_F5] ) 
	  change_map_key = false;

	// shadows on/off
	if ( Sdl.keys[SDLK_F9] && !change_shadows_key ) 
	{
		change_shadows_key = true;
		change_shadows = true;
	
		if ( change_shadows )
		{
			if ( game_shadows ) 
				game_shadows = false;
			else
				game_shadows = true;
			
			change_shadows = false;
		}
	}
	if ( !Sdl.keys[SDLK_F9] ) 
		change_shadows_key = false;
	
	if ( Sdl.keys[SDLK_F11] && !toggle_fullscreen )
	{
	  toggle_fullscreen = true;
	  Sdl.ToggleFullscreen();
	}
	if ( !Sdl.keys[SDLK_F11] ) 
	  toggle_fullscreen = false;

	// music on/off
	if ( Sdl.keys[SDLK_F4] && !music_off_key ) 
	{
		music_off_key = true;
		music_off = true;
	
		if ( music_off )
		{
			if ( Snd.IsMusicPlaying() ) 
				Snd.StopMusic();
			else
				Snd.PlayMusic();
			
			music_off = false;
		}
	}
	if ( !Sdl.keys[SDLK_F4] ) 
		music_off_key = false;

	//if ( Sdl.keys[SDLK_F12] ) 
	//	bRunning = false; //{!}

	if ( Sdl.keys[SDLK_F6] ) 
		show_fps = !show_fps;

	// check music sequence
	//if ( Gamestate != GS_MENU )
		Snd.CheckMusic();


	// GAME_STATES
	switch( Gamestate )
	{
	case GS_EXIT:

		if ( !at_exit )
		{
			at_exit = true;

			if ( ( splash = Sdl.LoadBitmap( "gfx/interf/advert.bmp" )) == NULL ) 
				at_exit = false;  
	
			time_destroyem = Timer.Time() + 15000;
		}

		if ( !at_exit || time_destroyem > Timer.Time() )
		{
			Sdl.BlitNow( 0, 0, splash ); 

			for ( i = 0; i < 255; i++ )
			{
				if ( Sdl.keys[i] && !Sdl.keys[SDLK_ESCAPE] )
				{
					bRunning = false;
					RELEASE_SURF( splash );
				}
			}
		}
		else
			bRunning = false;


	break;

	case GS_MENU:

		if ( !Sdl.keys[SDLK_ESCAPE]  )
			menu_escape_key = false;

		Menu.Update();

		if ( Sdl.keys[SDLK_ESCAPE] && !menu_escape_key )
		{
			menu_escape_key = true;
			Menu.SetMenuState( MS_EXIT );
		}

	break;


	case GS_GAMEPLAY_SETUP:

		if ( wait_key )
		{
			Sdl.BlitNow( 0, 0, getready );			
			
			for ( i = 0; i < 255; i++ )
			{
				if ( Sdl.keys[i] )
				{
					Gamestate = GS_GAMEPLAY;	// now play...*/
					wait_key = false;

					// choose random start_map
					cur_map = intGetRnd( 2, NUM_BACKGROUNDS );

					// stop menu-music
					Snd.StopMusic();
					// start_random_ingame_music
					Snd.Play( (CONST_MUSIC)intGetRnd( 1, NUM_MUSIX ), false );

					// prepare time if time-round
					time_round = Timer.Time() + game_time * 1000;

					break;
				}
			}
		}
		else
		{
			SetupVehicles();

			// reset classes
			Anims.Reset();
			Dtoys.Reset();
			Mines.Reset();
			wait_key = true;
		}

	break;


	case GS_GAMEPLAY:

		if ( Sdl.keys[SDLK_ESCAPE] && !game_paused ) 
		{
			//bRunning = false;  // {!} immediate QUIT
			quit_dialog = true;
			Sdl.FlipTo( pause_surf );
		}
		else if ( Sdl.keys[SDLK_p] && !quit_dialog && !game_paused_key )
		{
			game_paused_key = true;

			if ( game_paused ) 
			{
				game_paused = false;
				Timer.Unpause(); // unpause timer
			}
			else
			{
				Sdl.FlipTo( pause_surf );
				game_paused = true;
				Timer.Pause(); // pause timer
			}

		}

		if ( !Sdl.keys[SDLK_p] ) 
		  game_paused_key = false;


		if ( !game_paused && !quit_dialog ) 
		{
			// blit background
			Sdl.BlitNow( 0, 0, back[0] );
			Sdl.BlitNow( 6, 2, back[cur_map] );
	
			// update animations
			Anims.Update();
			// update toys
			Dtoys.Update();
			Mines.Update();
	
			// update vehicles
			ptr_veh = Auto;
			for ( i = 0; i < game_num_cars; i++, ptr_veh++ )
			{
				ptr_veh->Update();

				switch( i )
				{
				case 0:
					Sdl.BlitNow( 10, 429, ptr_veh->GetDriverNameSurface() );
					break;

				case 1:
					Sdl.BlitNow( 172, 429, ptr_veh->GetDriverNameSurface() );
					break;

				case 2:					
					Sdl.BlitNow( 333, 429, ptr_veh->GetDriverNameSurface() );
					break;

				case 3:					
					Sdl.BlitNow( 493, 429, ptr_veh->GetDriverNameSurface() );
					break;
				}
			
				// check for frag-winner
				if ( Gametype == GT_FRAGS )
				{
					if ( ptr_veh->GetFrags() >= game_frags ) 
					{
						winner_index = i;
						game_end = true;
						break;
					}
				}
			} // end for

			// check for round-time
			if ( Gametype == GT_TIME )
			{
				tv = (time_round - Timer.Time()) / 1000;
				if ( tv > 0 )
				{
					dx = tv / 60;
					sprintf( szfps, "%d", dx );
					Sdl.DrawNum( 305, 3, szfps );
					dx = tv % 60;
					sprintf( szfps, "%.2d", dx );
					Sdl.DrawNum( 319, 3, szfps );
				}
				else
				{
					game_end = true;

					if ( Gamemode == GM_2V2 )
					{
						dx = Auto[0].GetFrags() + Auto[1].GetFrags();
						dy = Auto[2].GetFrags() + Auto[3].GetFrags();

						if ( dx > dy )
						{
							if ( Auto[0].GetFrags() > Auto[1].GetFrags() )
							{
								winner_index = 0;
								winner_index2 = 1;
							}
							else
							{
								winner_index = 1;
								winner_index2 = 0;
							}
						}
						else if ( dx < dy )
						{
							winner_index = 2;
							winner_index2 = 3;
						}
						else
							winner_index = 9999;
					}
					else
					{
						// time's up, see who wins
						ptr_veh = &Auto[1];
						winner_index = 0;
						for ( i = 1; i < game_num_cars; i++, ptr_veh++ )
						{
							if ( ptr_veh->GetFrags() > Auto[winner_index].GetFrags() )
								winner_index = i;
							else if ( ptr_veh->GetFrags() == Auto[winner_index].GetFrags() 
									  && ptr_veh->GetTeam() != Auto[winner_index].GetTeam() )
							{
								winner_index = 9999; // game is draw
								break;
							}
						}
					} // end if GM2v2
				}
			}

			// end-game
			if ( game_end )
			{
				game_end = false;

				// if it's a team-game get the other winner's index
				if ( Gamemode == GM_2V2 && Gametype == GT_FRAGS )
				{
					for ( ptr_veh = &Auto[0], i = 0; i < game_num_cars; i++, ptr_veh++ )
					{
						if ( i != winner_index )
							if ( ptr_veh->GetTeam() == Auto[winner_index].GetTeam() )
							{
								winner_index2 = i;
								break;
							}	
					}
				}

				Gamestate = GS_GAMEPLAY_WINNER;
				time_showwinner = Timer.Time() + TIME_SHOWWINNER;
				Snd.FadeMusic( TIME_SHOWWINNER );
			}
		}

		// do quit_dialog || game_pause
		if ( quit_dialog )
		{
			dx = 320 - (but_quit[0]->w) / 2;
			dy = 240 - (but_quit[0]->h) / 2;

			Sdl.BlitNow( 0, 0, pause_surf );
			//Sdl.BlitShadow( dx + 9, dy + 9, but_quit );
			// blit quit_game dialog
			Sdl.BlitNow( dx, dy, but_quit[(int)qg_frame] );
			qg_frame += mpf * 10;
			if ( qg_frame > 4 ) qg_frame = 0;

			if ( Sdl.keys[SDLK_y] ) 
			{
				// reset campaign level
				if ( ( Gameplayers == GP_JUDY_CAMPAIGN || Gameplayers == GP_KEETH_CAMPAIGN ) ) 
					player_level = 1;

				quit_dialog = false;
				Gamestate = GS_MENU;
				Snd.StopMusic();
				Snd.Play( MUS_MENU, true );
			}
			if ( Sdl.keys[SDLK_n] ) quit_dialog = false;
		}
		else if ( game_paused )
		{
			dx = 320 - (but_pause->w / 2);
			dy = 240 - (but_pause->h / 2);

			Sdl.BlitNow( 0, 0, pause_surf );
			Sdl.BlitNow( dx, dy, but_pause );
			/*if ( Sdl.keys[SDLK_p] ) 
			{
				game_paused = false;
				Timer.Unpause(); // unpause timer
			}*/
		}

	break;


	case GS_GAMEPLAY_WINNER:

		if ( Sdl.keys[SDLK_ESCAPE] || time_showwinner < Timer.Time() ) 
		{
			Gamestate = GS_MENU;

			if ( time_showwinner < Timer.Time() )
			{
				Snd.StopMusic();
				Snd.Play( MUS_MENU, true );
			}
			
			// advance campaign level if player won
			if ( ( Gameplayers == GP_JUDY_CAMPAIGN || Gameplayers == GP_KEETH_CAMPAIGN ) ) 
			{
				if ( winner_index == 0 )
					player_level++;
				else
				{   // go back 1 level
					player_level--;
					if ( player_level < 1 ) player_level = 1;
				}
			}

			// release car-classes
			for ( i = 0; i < game_num_cars; i++ )
				Auto[i].Release();
		}
		else
		{
			// blit background
			Sdl.BlitNow( 0, 0, back[0] );
			Sdl.BlitNow( 6, 2, back[cur_map] );

			// update animations
			Anims.Update();
			// update toys
			Dtoys.Update();
			Mines.Update();
	
			// update vehicles	
			ptr_veh = Auto;
			for ( i = 0; i < game_num_cars; i++, ptr_veh++ )
			{
				ptr_veh->Update();

				switch( i )
				{
				case 0:
					Sdl.BlitNow( 10, 429, ptr_veh->GetDriverNameSurface() );
					break;

				case 1:
					Sdl.BlitNow( 172, 429, ptr_veh->GetDriverNameSurface() );
					break;

				case 2:					
					Sdl.BlitNow( 333, 429, ptr_veh->GetDriverNameSurface() );
					break;

				case 3:					
					Sdl.BlitNow( 493, 429, ptr_veh->GetDriverNameSurface() );
					break;
			
				}

				// destroy looser-vehicles
				if ( Timer.Time() > time_destroyem )
				{
					time_destroyem = 0;
					if ( i != winner_index )
						ptr_veh->DoDamage( 1000, NO_ATTACKER );
				}
			}

			if ( time_destroyem == 0 ) 
			  time_destroyem = Timer.Time() + intGetRnd( 1800, 3000 );

			if ( Gamemode == GM_2V2 )
			{
				if ( winner_index == 9999 )
				{
					// a draw game
					dx = 320 - 30;
					dy = rScreen.h / 2 - 40;
					rtemp.x = 0;
					rtemp.y = 13;
					rtemp.w = 60;
					rtemp.h = 12;
					Sdl.BlitNow( dx, dy, winneris, &rtemp );
				}
				else
				{
					// show 2 winners
					dx = 320 - 32;
					dy = rScreen.h / 2 - 40;
					rtemp.x = 0;
					rtemp.y = 25;
					rtemp.w = 64;
					rtemp.h = 12;
					Sdl.BlitNow( dx, dy, winneris, &rtemp );
					dx += 64;
					Sdl.BlitNow( dx, dy, Auto[winner_index].GetDriverNameSurface() );
					Sdl.BlitNow( dx, dy + 14, Auto[winner_index2].GetDriverNameSurface() );
				}
			}
			else
			{
				if ( winner_index == 9999 )
				{
					// a draw game
					dx = 320 - 30;
					dy = rScreen.h / 2 - 40;
					rtemp.x = 0;
					rtemp.y = 13;
					rtemp.w = 60;
					rtemp.h = 12;
					Sdl.BlitNow( dx, dy, winneris, &rtemp );
				}
				else
				{
					// show who's winner
					dx = 320 - 65;
					dy = rScreen.h / 2 - 40;
					rtemp.x = rtemp.y = 0;
					rtemp.w = 55;
					rtemp.h = 12;
					Sdl.BlitNow( dx, dy, winneris, &rtemp );
					dx += 52;
					Sdl.BlitNow( dx, dy, Auto[winner_index].GetDriverNameSurface() );
				}
			}
		}

	break;


	//case GS_EXIT:
		//...
		//bRunning = false;
	//break;
	}

	if ( show_fps )
	{
		// blit framespersec
		//sprintf( szfps, "%d %f", fps, mpf );
		sprintf( szfps, "%d", fps );
		Sdl.DrawNum( SCREEN_WIDTH - 40, 430, szfps );
		//Sdl.DrawNum( 500, 400, szfps );
	}
	
	// flip buffers
	Sdl.Flip();   
    
  }


}


///////////////////////////////////////////////////////////////////////
// Name: GetFreeCarIndex()
// Desc:
///////////////////////////////////////////////////////////////////////
void CGame::SetupVehicles()
{
	bool    bFound		= false;
	Uint32  total_cars	= 0U;
    //Uint32  swm_ai_index;
	Uint32  ai_car[3]; // ai_car1, ai_car2, ai_car3;
	char    *v_name;
	int		i			= 0;
	Uint32  cn			= 0U;

	static bool index_build = false;
	static int  v_array[8];

	// get total cars
	total_cars = (Uint32)Swv.GetVehicles();

	// prebuild indeces for better performance
	if ( !index_build )
	{
		index_build = true;

		for ( cn = 0; cn < total_cars; cn++ )
		{
			v_name = Swv.GetVehicleName( cn );
			if ( strcmp( v_name, "BMW" ) == 0 )
				v_array[CV_BMW] = cn;
			else if ( strcmp( v_name, "MERC" ) == 0 )
				v_array[CV_MERCEDES] = cn;
			else if ( strcmp( v_name, "DIABLO") == 0 )
				v_array[CV_DIABLO] = cn;
			else if ( strcmp( v_name, "DODGE") == 0 )
				v_array[CV_DODGE] = cn;
			else if ( strcmp( v_name, "DJUGAN") == 0 )
				v_array[CV_DJUGAN] = cn;
			else if ( strcmp( v_name, "JEEP") == 0 )
				v_array[CV_JEEP] = cn;
			else if ( strcmp( v_name, "F1") == 0 )
				v_array[CV_F1] = cn;
			else if ( strcmp( v_name, "DTRUCK") == 0 )
				v_array[CV_DTRUCK] = cn;
		}
	}

	if ( Gameplayers == GP_2PLAYER )
	{

		// setup 2-PLAYER_GAME with bots
		// ------------------------------

		if ( !Auto[0].Initialize( this, &(Swv.GetVehiclesData()[p1_auto_index]), 0 ) ) 
		{
			AppendToLog( "Error initializing vehicle #0 !" );
			return;
		}
		if ( !Auto[1].Initialize( this, &(Swv.GetVehiclesData()[p2_auto_index]), 1 ) ) 
		{
			AppendToLog( "Error initializing vehicle #1 !" );
			return;
		}

		game_num_cars = 2;

		if ( Gamemode == GM_MELEE )				// MELEE
		{
			game_num_cars = 4;
			// get 2 dif vehicles indices
			while ( !bFound )
			{
				ai_car[0] = intGetRnd( 0, total_cars );
				ai_car[1] = intGetRnd( 0, total_cars );

				if ( ai_car[0] != ai_car[1] &&
					 ai_car[0] != p1_auto_index &&
					 ai_car[0] != p2_auto_index &&
					 ai_car[1] != p1_auto_index &&
					 ai_car[1] != p2_auto_index )
				{
					bFound = true;
				}
			}

			for ( i = 2; i < game_num_cars; i++ )
			{
				if ( !Auto[i].Initialize( this, &(Swv.GetVehiclesData()[ai_car[i-2]]), i ) ) 
				{
					AppendToLog( "Error initializing vehicle [2P]!" );
					return;
				}

				Auto[i].SetControl( VC_AI );
				Auto[i].SetAttirbs( Gamediff );
				Auto[i].Create();
			}

			// configure players
			Auto[0].Create();
			Auto[0].SetTeam( 1 );
			Auto[0].SetControl( VC_PLAYER1 );
			Auto[1].Create(); 
			Auto[1].SetControl( VC_PLAYER2 );
			Auto[1].SetTeam( 2 );

		}
		else if ( Gamemode == GM_2V2 ) // TEAM v.s. TEAM
		{
			game_num_cars = 4;
			while ( !bFound )
			{
				ai_car[0] = intGetRnd( 0, total_cars );
				ai_car[1] = intGetRnd( 0, total_cars );

				if ( ai_car[0] != ai_car[1] &&
					 ai_car[0] != p1_auto_index &&
					 ai_car[0] != p2_auto_index &&
					 ai_car[1] != p1_auto_index &&
					 ai_car[1] != p2_auto_index )
				{
					bFound = true;
				}
			}

			for ( i = 2; i < game_num_cars; i++ )
			{
				if ( !Auto[i].Initialize( this, &(Swv.GetVehiclesData()[ai_car[i-2]]), i ) ) 
				{
					AppendToLog( "Error initializing vehicle [2P]!" );
					return;
				}

				Auto[i].SetControl( VC_AI );
				Auto[i].SetAttirbs( Gamediff );
				Auto[i].Create();
				Auto[i].SetTeam( 1 );
			}

			// configure players
			Auto[0].Create();
			Auto[0].SetTeam( 0 );
			Auto[0].SetControl( VC_PLAYER1 );
			Auto[1].Create(); 
			Auto[1].SetControl( VC_PLAYER2 );
			Auto[1].SetTeam( 0 );
		}
		else if ( Gamemode == GM_1V1 )
		{
			// configure players
			Auto[0].Create();
			Auto[0].SetTeam( 1 );
			Auto[0].SetControl( VC_PLAYER1 );
			Auto[1].Create(); 
			Auto[1].SetControl( VC_PLAYER2 );
			Auto[1].SetTeam( 2 );
		}


		//Auto[0].SetAttirbs( Gamediff );
		//Auto[1].SetAttirbs( Gamediff );
	}
	else if ( Gameplayers == GP_1PLAYER )
	{
		// setup 1-PLAYER_GAME 
		// ------------------------------

		// get 3 dif vehicles indices
		while ( !bFound )
		{
			ai_car[0] = intGetRnd( 0, total_cars );
			ai_car[1] = intGetRnd( 0, total_cars );
			ai_car[2] = intGetRnd( 0, total_cars );

			if ( ai_car[0] != ai_car[1] &&
				 ai_car[0] != ai_car[2] &&
				 ai_car[1] != ai_car[2] &&
				 ai_car[0] != p1_auto_index &&
				 ai_car[1] != p1_auto_index &&
				 ai_car[2] != p1_auto_index )
			{
				bFound = true;
			}
		}


		if ( !Auto[0].Initialize( this, &(Swv.GetVehiclesData()[p1_auto_index]), 0 ) ) 
		{
			AppendToLog( "Error initializing vehicle #0 !" );
			return;
		}

		// prepare 1v1 game
		if ( Gamemode == GM_1BOT_MELEE )
			game_num_cars = 2;
		else if ( Gamemode == GM_2BOTS_MELEE )
			game_num_cars = 3;
		else if ( Gamemode == GM_3BOTS_MELEE )
			game_num_cars = 4;

		for ( int i = 1; i < game_num_cars; i++ )
		{
			if ( !Auto[i].Initialize( this, &(Swv.GetVehiclesData()[ai_car[i-1]]), i ) ) 
			{
				AppendToLog( "Error initializing vehicle!" );
				return;
			}

			Auto[i].SetControl( VC_AI );
			Auto[i].SetAttirbs( Gamediff );
			Auto[i].Create();
		}
		
		Auto[0].SetControl( VC_PLAYER1 );
		//Auto[0].SetControl( VC_AI  );
		Auto[0].Create();

	}
	else if ( Gameplayers == GP_JUDY_CAMPAIGN )	  /* CAMPAIGNS - JUDY */
	{
		if ( !Auto[0].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_DIABLO]]), 0 ) ) 
		{
			AppendToLog( "Error initializing vehicle JUDY vehicle !" );
			return;
		}
		
		Auto[0].SetControl( VC_PLAYER1 ); // player 1 controls
		
		switch( player_level )
		{

		case 1:  // LEVEL 1
			game_num_cars = 2;
			game_time = 180;	// game-round-time
			
			if ( !Auto[1].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_BMW]]), 1 ) ) 
			{
				AppendToLog( "Error initializing JUDY OPPONENT #1 !" );
				return;
			}
			Auto[1].SetControl( VC_AI );
			Auto[1].SetAttirbs( Gamediff );
			Auto[0].Create();
			Auto[1].Create();
			
			break;

		
		case 2:  // LEVEL 2
			game_num_cars = 2;
			game_time = 180;	// game-round-time
			
			if ( !Auto[1].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_F1]]), 1 ) ) 
			{
				AppendToLog( "Error initializing JUDY OPPONENT #2 !" );
				return;
			}
			Auto[1].SetControl( VC_AI );
			Auto[1].SetAttirbs( Gamediff );
			Auto[0].Create();
			Auto[1].Create();
			
			break;

		
		case 3:  // LEVEL 3
			game_num_cars = 2;
			game_time = 180;	// game-round-time
			
			if ( !Auto[1].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_JEEP]]), 1 ) ) 
			{
				AppendToLog( "Error initializing JUDY OPPONENT #3 !" );
				return;
			}
			Auto[1].SetControl( VC_AI );
			Auto[1].SetAttirbs( Gamediff );
			Auto[0].Create();
			Auto[1].Create();
			break;

		
		case 4:  // LEVEL 4
			game_num_cars = 3;
			game_time = 300;	// game-round-time
			
			if ( !Auto[1].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_MERCEDES]]), 1 ) ) 
			{
				AppendToLog( "Error initializing JUDY OPPONENT #4.1 !" );
				return;
			}
			if ( !Auto[2].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_BMW]]), 2 ) ) 
			{
				AppendToLog( "Error initializing JUDY OPPONENT #4.2 !" );
				return;
			}

			Auto[1].SetControl( VC_AI );
			Auto[1].SetAttirbs( Gamediff );
			Auto[2].SetControl( VC_AI );
			Auto[2].SetAttirbs( Gamediff );
			Auto[0].Create();
			Auto[1].Create();
			Auto[2].Create();
			break;


		case 5:  // LEVEL 5
			game_num_cars = 4;
			game_time = 300;	// game-round-time
			
			if ( !Auto[1].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_DODGE]]), 1 ) ) 
			{
				AppendToLog( "Error initializing JUDY OPPONENT #4.1 !" );
				return;
			}
			if ( !Auto[2].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_F1]]), 2 ) ) 
			{
				AppendToLog( "Error initializing JUDY OPPONENT #4.2 !" );
				return;
			}
			if ( !Auto[3].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_JEEP]]), 3 ) ) 
			{
				AppendToLog( "Error initializing JUDY OPPONENT #4.3 !" );
				return;
			}

			Auto[1].SetControl( VC_AI );
			Auto[1].SetAttirbs( Gamediff );
			Auto[2].SetControl( VC_AI );
			Auto[2].SetAttirbs( Gamediff );
			Auto[3].SetControl( VC_AI );
			Auto[3].SetAttirbs( Gamediff );
			Auto[0].Create(); Auto[1].Create();
			Auto[2].Create(); Auto[3].Create();
			
			break;


		case 6:  // LEVEL 6
			game_num_cars = 2;
			game_time = 300;	// game-round-time
			
			if ( !Auto[1].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_DTRUCK]]), 1 ) ) 
			{
				AppendToLog( "Error initializing JUDY OPPONENT #6 !" );
				return;
			}
			Auto[1].SetControl( VC_AI );
			Auto[1].SetAttirbs( Gamediff );
			Auto[0].Create();
			Auto[1].Create();
			
			break;

		case 7: // WINER - JUDY
			Gamestate = GS_CAMPAIGN_WIN;
			break;
		}

	}
	else if ( Gameplayers == GP_KEETH_CAMPAIGN )	  /* CAMPAIGNS - KEETH */
	{
		if ( !Auto[0].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_DODGE]]), 0 ) ) 
		{
			AppendToLog( "Error initializing vehicle KEETH vehicle !" );
			return;
		}
		
		Auto[0].SetControl( VC_PLAYER1 ); // player 1 controls
		
		switch( player_level )
		{

		case 1:  // LEVEL 1
			game_num_cars = 2;
			game_time = 180;	// game-round-time
			
			if ( !Auto[1].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_MERCEDES]]), 1 ) ) 
			{
				AppendToLog( "Error initializing KEETH OPPONENT #1 !" );
				return;
			}
			Auto[1].SetControl( VC_AI );
			Auto[1].SetAttirbs( Gamediff );
			Auto[0].Create();
			Auto[1].Create();
			
			break;

		
		case 2:  // LEVEL 2
			game_num_cars = 2;
			game_time = 180;	// game-round-time
			
			if ( !Auto[1].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_F1]]), 1 ) ) 
			{
				AppendToLog( "Error initializing KEETH OPPONENT #2 !" );
				return;
			}
			Auto[1].SetControl( VC_AI );
			Auto[1].SetAttirbs( Gamediff );
			Auto[0].Create();
			Auto[1].Create();
			
			break;

		
		case 3:  // LEVEL 4
			game_num_cars = 3;
			game_time = 300;	// game-round-time
			
			if ( !Auto[1].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_MERCEDES]]), 1 ) ) 
			{
				AppendToLog( "Error initializing KEETH OPPONENT #4.1 !" );
				return;
			}
			if ( !Auto[2].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_BMW]]), 2 ) ) 
			{
				AppendToLog( "Error initializing KEETH OPPONENT #4.2 !" );
				return;
			}

			Auto[1].SetControl( VC_AI );
			Auto[1].SetAttirbs( Gamediff );
			Auto[2].SetControl( VC_AI );
			Auto[2].SetAttirbs( Gamediff );
			Auto[0].Create();
			Auto[1].Create();
			Auto[2].Create();
			break;

		case 4:  // LEVEL 3
			game_num_cars = 2;
			game_time = 180;	// game-round-time
			
			if ( !Auto[1].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_DJUGAN]]), 1 ) ) 
			{
				AppendToLog( "Error initializing KEETH OPPONENT #3 !" );
				return;
			}
			Auto[1].SetControl( VC_AI );
			Auto[1].SetAttirbs( Gamediff );
			Auto[0].Create();
			Auto[1].Create();
			break;

		case 5:  // LEVEL 5
			game_num_cars = 4;
			game_time = 300;	// game-round-time
			
			if ( !Auto[1].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_DIABLO]]), 1 ) ) 
			{
				AppendToLog( "Error initializing KEETH OPPONENT #4.1 !" );
				return;
			}
			if ( !Auto[2].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_F1]]), 2 ) ) 
			{
				AppendToLog( "Error initializing KEETH OPPONENT #4.2 !" );
				return;
			}
			if ( !Auto[3].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_JEEP]]), 3 ) ) 
			{
				AppendToLog( "Error initializing KEETH OPPONENT #4.3 !" );
				return;
			}

			Auto[1].SetControl( VC_AI );
			Auto[1].SetAttirbs( Gamediff );
			Auto[2].SetControl( VC_AI );
			Auto[2].SetAttirbs( Gamediff );
			Auto[3].SetControl( VC_AI );
			Auto[3].SetAttirbs( Gamediff );
			Auto[0].Create(); Auto[1].Create();
			Auto[2].Create(); Auto[3].Create();
			
			break;


		case 6:  // LEVEL 6
			game_num_cars = 2;
			game_time = 300;	// game-round-time
			
			if ( !Auto[1].Initialize( this, &(Swv.GetVehiclesData()[v_array[CV_DTRUCK]]), 1 ) ) 
			{
				AppendToLog( "Error initializing KEETH OPPONENT #6 !" );
				return;
			}
			Auto[1].SetControl( VC_AI );
			Auto[1].SetAttirbs( Gamediff );
			Auto[0].Create();
			Auto[1].Create();
			
			break;

		case 7: // WINER - KEETH
			break;
		}

	}

}

String CGame::GetWindowTitle()
{
	char buf[512];
	sprintf( buf, "Savage Wheels - KenamicK Entertainment V%d.%d.%d %s",
			VER_MAJ, VER_MIN, VER_PATCH, VER_ARCH );
	return String(buf);
}




