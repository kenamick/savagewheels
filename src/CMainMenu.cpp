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

extern int attribACC[3];
extern int attribSPEED[5];
//extern int attribROT[3];
extern int attribDAMAGE[4];
extern int attribARMOUR[4];

//POINT	pos_menu[5] = { 420, 100, 420, 170, 420, 240, 420, 310, 420, 380 };
/*POINT	pos_opt_but[13] = { 230, 50, 230, 80, 230, 110, 230, 140, 230, 170, 230, 200,
							470, 50, 300, 80, 300, 110, 300, 140, 400, 170, 400, 200, 
							420, 400 };*/
static POINT pos_options[6]	= {37, 60, 37, 110, 37, 160, 37, 210, 37, 260, 37, 310, /*37, 360*/};
static POINT pos_opt_but[13] = { 230, 50, 230, 100, 230, 150, 230, 200, 230, 250, 230, 300, //230, 350,
		470, 50, 330, 100, 300, 150, 300, 200, 400, 250, 400, 300, //400, 350,
		520, 420 };

static POINT   pos_sel[8] = { 20, 100, 590, 100, 440, 420, 520, 420,
		160, 335, 360, 335,
		160, 385, 360, 385  };
static POINT pos_menu[5]		= { 20, 110, 10, 180, 0, 245, 0, 310, 0, 375 };
//static POINT pos_campmenu[2]	= { 440, 420, 520, 420 };
static POINT pos_campselect[2]	= { 59, 15, 59, 285 };

static int current_car 		= 1;
static bool faces_released	= true;
static bool p2_choice		= false;

static SDL_Surface *ssButtons[MENU_BUTTONS];
static SDL_Surface *ssStrings[STRINGS_SURFACES];

//////////////////////////////////////////////////////////////////////
// Ime: CMainMenu()
// Opisanie: 
//////////////////////////////////////////////////////////////////////
CMainMenu::CMainMenu()
: _game( NULL )
{
	Uint32 i = 0;
	for ( i = 0; i < MENU_BACKGROUNDS; i++ )
		 ssBack[i] = NULL;

	for ( i = 0; i < MENU_BUTTONS; i++ )
		ssButtons[i] = NULL;

	for ( i = 0; i < STRINGS_SURFACES; i++ )
		ssStrings[i] = NULL;

	ssVolume = NULL;
	
	for ( i = 0; i < 17; i++ )
		ssCursor[i] = NULL;
	
	ssCarAttribs = NULL;
	ssMenu[0] = NULL;
	ssMenu[1] = NULL;
    credits = NULL;

	for ( i = 0; i < 10; i++ )
		ssTitle[i] = NULL;

	// release upgrades
	for ( i = 0; i < 35; i++ )
	{
		ssUpg[ANIM_NOS][i] = NULL;
		ssUpg[ANIM_ENGINE][i] = NULL;
		ssUpg[ANIM_DAMAGE][i] = NULL;
		ssUpg[ANIM_ARMOUR][i] = NULL;
	}

	// release select menus
	savage_select[0] = NULL;
	savage_select[1] = NULL;

	camp_select = NULL;
	camp_selquad = NULL;

}

//////////////////////////////////////////////////////////////////////
// Ime: ~CMainMenu()
// Opisanie: 
//////////////////////////////////////////////////////////////////////
CMainMenu::~CMainMenu()
{
}

//////////////////////////////////////////////////////////////////////
// Ime: Release()
// Opisanie: release graphics & save settings
//////////////////////////////////////////////////////////////////////
void CMainMenu::Release()
{

	Uint32 i = 0;

	// save settings on close
	SaveSettings();

	for ( i = 0; i < MENU_BACKGROUNDS; i++ )
		RELEASE_SURF( ssBack[i] );

	for ( i = 0; i < MENU_BUTTONS; i++ )
		RELEASE_SURF( ssButtons[i] );

	for ( i = 0; i < STRINGS_SURFACES; i++ )
		RELEASE_SURF( ssStrings[i] );

	RELEASE_SURF( ssVolume );
	
	for ( i = 0; i < 17; i++ )
		RELEASE_SURF( ssCursor[i] );
	
	RELEASE_SURF( ssCarAttribs );
	RELEASE_SURF( ssMenu[0] );
	RELEASE_SURF( ssMenu[1] );
    RELEASE_SURF( credits );

	for ( i = 0; i < 10; i++ )
		RELEASE_SURF( ssTitle[i] );

	// release upgrades
	for ( i = 0; i < 35; i++ )
	{
		RELEASE_SURF( ssUpg[ANIM_NOS][i] );
		RELEASE_SURF( ssUpg[ANIM_ENGINE][i] );
		RELEASE_SURF( ssUpg[ANIM_DAMAGE][i] );
		RELEASE_SURF( ssUpg[ANIM_ARMOUR][i] );
	}

	// release select menus
	RELEASE_SURF( savage_select[0] );
	RELEASE_SURF( savage_select[1] );

	RELEASE_SURF( camp_select );
	RELEASE_SURF( camp_selquad );

	if ( !faces_released) 
		ReleaseCarsAttribs();

}



//////////////////////////////////////////////////////////////////////
// Ime: Initialize()
// Opisanie: load graphics & settings
//////////////////////////////////////////////////////////////////////
int CMainMenu::Initialize( CGame *game )
{
	char     buf[64];
	Uint32   i = 0, j = 0;

	this->_game = game;
	ASSERT( _game != NULL );

	// load credit screen
	sprintf( buf, "gfx/interf/credits.bmp");
	if ( (credits = _game->Sdl.LoadBitmap( buf, BLACK )) == NULL ) 
		return false;

	// load campign_backg & select quad
	sprintf( buf, "gfx/interf/cmpsel.bmp");
	if ( (camp_select = _game->Sdl.LoadBitmap( buf, 0U )) == NULL )
		return false;

	sprintf( buf, "gfx/interf/cmpoutln.bmp");
	if ( (camp_selquad = _game->Sdl.LoadBitmap( buf, 0U )) == NULL )
		return false;
	

	// load select titles
	sprintf( buf, "gfx/interf/sc1.bmp");
	if ( (savage_select[0] = _game->Sdl.LoadBitmap( buf, 0U )) == NULL )
		return false;

	sprintf( buf, "gfx/interf/sc2.bmp");
	if ( (savage_select[1] = _game->Sdl.LoadBitmap( buf, 0U )) == NULL )
		return false;
	
	// load upgareds
	for ( i = 0; i < 35; i++ )
	{
		sprintf( buf, "gfx/upg/eng%d.bmp", i );
		if ( (ssUpg[ANIM_ENGINE][i] = _game->Sdl.LoadBitmap( buf, MAGENTA )) == NULL )
			return false;
		sprintf( buf, "gfx/upg/arm%d.bmp", i );
		if ( (ssUpg[ANIM_ARMOUR][i] = _game->Sdl.LoadBitmap( buf, MAGENTA )) == NULL )
			return false;
		sprintf( buf, "gfx/upg/nos%d.bmp", i );
		if ( (ssUpg[ANIM_NOS][i] = _game->Sdl.LoadBitmap( buf, MAGENTA )) == NULL )
			return false;
		sprintf( buf, "gfx/upg/dmg%d.bmp", i );
		if ( (ssUpg[ANIM_DAMAGE][i] = _game->Sdl.LoadBitmap( buf, MAGENTA )) == NULL )
			return false;
	}


	// load menu_
	sprintf( buf, "gfx/interf/menu_1.bmp" );
	if ( (ssMenu[0] = _game->Sdl.LoadBitmap( buf, MAGENTA )) == NULL )
		return false;
	
	sprintf( buf, "gfx/interf/menu_2.bmp" );
	if ( (ssMenu[1] = _game->Sdl.LoadBitmap( buf, MAGENTA )) == NULL )
		return false;

	// load title
	for ( i = 0; i < 10; i++ )
	{
		sprintf( buf, "gfx/interf/tt/head%d.bmp", i );
		if ( (ssTitle[i] = _game->Sdl.LoadBitmap( buf, MAGENTA )) == NULL )
			return false;
	}

	// load background(s)
	for ( i = 0; i < MENU_BACKGROUNDS; i++ )
	{
		sprintf( buf, "gfx/interf/menub%d.bmp", i+1 );
		if ( (ssBack[i] = _game->Sdl.LoadBitmap( buf )) == NULL )
			return false;
	}

	// load buttons
	for ( i = 0; i < MENU_BUTTONS; i++ )
	{
		sprintf( buf, "gfx/interf/but%d.bmp", i+1 );
		if ( (ssButtons[i] = _game->Sdl.LoadBitmap( buf, MAGENTA )) == NULL )
			return false;
	}

	// load string_surfaces
	for ( i = 0; i < STRINGS_SURFACES; i++ )
	{
		sprintf( buf, "gfx/interf/butopt%d.bmp", i+1 );
		if ( (ssStrings[i] = _game->Sdl.LoadBitmap( buf, 0U )) == NULL )
			return false;
	}

	// load volume scale
	sprintf( buf, "gfx/interf/sk3.bmp" );
	if ( (ssVolume = _game->Sdl.LoadBitmap( buf )) == NULL )
		return false;


	// load cursor
	for ( i = 0; i < 17; i++ )
	{
		sprintf( buf, "gfx/cursor/cur%d.bmp", i );
		if ( (ssCursor[i] = _game->Sdl.LoadBitmap( buf, MAGENTA )) == NULL )
			return false;
	}


	sprintf( buf, "gfx/interf/atrb.bmp" );
	if ( (ssCarAttribs = _game->Sdl.LoadBitmap( buf, 0U )) == NULL )
		return false;


/*	// load background
	for ( i = 0; i < MENU_BACKGROUNDS; i++ )
	{
		sprintf( buf, "menub%d.bmp", i+1 );
		if ( (ssBack[i] = _game->Sdl.LoadBitmap( ART_FILE, _game->Kdf.GetFilePosition(buf), _game->Kdf.GetFileSize(buf) )) == NULL )
			return false;
	}

	// load buttons
	for ( i = 0; i < MENU_BUTTONS - 2; i++ )
	{
		sprintf( buf, "but%d.bmp", i+1 );
		if ( (ssButtons[i] = _game->Sdl.LoadBitmap( ART_FILE, _game->Kdf.GetFilePosition(buf), _game->Kdf.GetFileSize(buf) )) == NULL )
			return false;
	}

	// load buttons with alpha values
	sprintf( buf, "but5.bmp" );
	if ( (ssButtons[4] = _game->Sdl.LoadBitmap( ART_FILE, _game->Kdf.GetFilePosition(buf), _game->Kdf.GetFileSize(buf), NO_COLORKEY, (Uint8)128 )) == NULL )
		return false;
	sprintf( buf, "but6.bmp" );
	if ( (ssButtons[5] = _game->Sdl.LoadBitmap( ART_FILE, _game->Kdf.GetFilePosition(buf), _game->Kdf.GetFileSize(buf), NO_COLORKEY, (Uint8)128 )) == NULL )
		return false;
	

	// load string_surfaces
	for ( i = 0; i < STRINGS_SURFACES; i++ )
	{
		sprintf( buf, "butopt%d.bmp", i+1 );
		if ( (ssStrings[i] = _game->Sdl.LoadBitmap( ART_FILE, _game->Kdf.GetFilePosition(buf), _game->Kdf.GetFileSize(buf), 0U )) == NULL )
			return false;
	}

	// load volume scale
	sprintf( buf, "sk3.bmp" );
	if ( (ssVolume = _game->Sdl.LoadBitmap( ART_FILE, _game->Kdf.GetFilePosition(buf), _game->Kdf.GetFileSize(buf) )) == NULL )
		return false;

	// load logo image
	sprintf( buf, "logo.bmp" );
	if ( (sslogo = _game->Sdl.LoadBitmap( ART_FILE, _game->Kdf.GetFilePosition(buf), _game->Kdf.GetFileSize(buf), 0U )) == NULL )
		return false;

	// load cursor
	sprintf( buf, "cursor.bmp" );
	if ( (ssCursor = _game->Sdl.LoadBitmap( ART_FILE, _game->Kdf.GetFilePosition(buf), _game->Kdf.GetFileSize(buf), 0U )) == NULL )
		return false;

	sprintf( buf, "atrb.bmp" );
	if ( (ssCarAttribs = _game->Sdl.LoadBitmap(ART_FILE, _game->Kdf.GetFilePosition(buf), _game->Kdf.GetFileSize(buf), 0U )) == NULL )
		return false;
*/

	camp_preview = NULL;

	// create menu buttons
	/*buttons_menu[0].Initialize( &pos_menu[0], BUTTON_OPTIONS_UP, 0, BTXT_MENU );
	buttons_menu[1].Initialize( &pos_menu[1], BUTTON_OPTIONS_UP, 1, BTXT_MENU );
	buttons_menu[2].Initialize( &pos_menu[2], BUTTON_OPTIONS_UP, 2, BTXT_MENU );
	buttons_menu[3].Initialize( &pos_menu[3], BUTTON_OPTIONS_UP, 3, BTXT_MENU );
	buttons_menu[4].Initialize( &pos_menu[4], BUTTON_OPTIONS_UP, 4, BTXT_MENU );
	buttons_menu[5].Initialize( &pos_menu[5], BUTTON_OPTIONS_UP, 5, BTXT_MENU );
*/
	// create options buttons
	for ( j = 0, i = 0; i < NUM_BUTTONS_OPTIONS - 1; i += 2, j+=1 )
	{
		buttons_options[i].Initialize( &pos_opt_but[j], BUTTON_LEFTC_UP );
		buttons_options[i+1].Initialize( &pos_opt_but[j+6], BUTTON_RIGHTC_UP );
	}
	// back button
	buttons_options[NUM_BUTTONS_OPTIONS].Initialize( &pos_opt_but[12], BUTTON_OK_UP, 6, BTXT_MENU );

	
	// create select buttons
	buttons_select[0].Initialize( &pos_sel[0], BUTTON_LEFTC_UP );
	buttons_select[1].Initialize( &pos_sel[1], BUTTON_RIGHTC_UP );
	buttons_select[2].Initialize( &pos_sel[2], BUTTON_BACK_UP, 5, BTXT_MENU );
	buttons_select[3].Initialize( &pos_sel[3], BUTTON_OK_UP, 6, BTXT_MENU );

	buttons_select[4].Initialize( &pos_sel[4], BUTTON_LEFTC_UP );
	buttons_select[5].Initialize( &pos_sel[5], BUTTON_RIGHTC_UP );
	
	buttons_select[6].Initialize( &pos_sel[6], BUTTON_LEFTC_UP );
	buttons_select[7].Initialize( &pos_sel[7], BUTTON_RIGHTC_UP );

	// load settings
	//LoadSettings();

	ReloadCarsAttribs();
	Menustate = MS_STARTSCREEN;

	return true;
}



//////////////////////////////////////////////////////////////////////
// Ime: Update()
// Opisanie:
//////////////////////////////////////////////////////////////////////
void CMainMenu::Update()
{
	Uint32		   i		=0;
	SDL_Rect	   rsrc = { 0, 0, 0, 0 };
	int			   dx		= 0, 
				   //dy, 
				   dxx		= 0, 
				   dxxx		= 0;
	SDL_Rect	   rTemp = { 0, 0, 0, 0 }, rcursor = { 0, 0, 0, 0 };
	char	 	   buf[32];
	static float   cur_frame		= 0;
	static int	   select_menu		= 0;
	static Uint32  frag_time		= 0;
	static Uint32  gametime_press	= 0;
	static int	   gc				= 0;
	
	switch( Menustate )
	{

	case MS_STARTSCREEN:
		
		ScrollScreen();

		// blit left_menu
		_game->Sdl.BlitNow( 0, 0, ssMenu[0] );
		// blit title
		DoTitle();
		// blit credits
		UpdateCredits();


		rcursor.x = _game->Sdl.GetMouseX();
		rcursor.y = _game->Sdl.GetMouseY();
		rcursor.w = rcursor.x + 8;
		rcursor.h = rcursor.y + 8;

		// check 4 cheats
		rTemp.x = 340;
		rTemp.y = 40;
		rTemp.w = 557;
		rTemp.h = 76;

	    if ( _game->Sdl.GetMouseLButton() == MOUSE_BUTTON_UP )
		{
			if ( _game->Sdl.Collide( NULL, &rTemp, &rcursor ) )
			{
				if ( ++gc > 12 ) 
				{
					_game->game_cheats = true;
					_game->Sdl.PlaySound( SND_WARNING );
				}
			}
		}

		for ( i = 0; i < 5; i++ )
		{
			rTemp.x = pos_menu[i].x;
			rTemp.y = pos_menu[i].y;
			rTemp.w = rTemp.x + 100;
			rTemp.h = rTemp.y + 45;

			if ( _game->Sdl.Collide( NULL, &rTemp, &rcursor ) )
			{
				switch( i )
				{
				case 0:
					rTemp.x = 0;
					rTemp.y = 0;
					rTemp.w = 260;
					rTemp.h = 160;
					_game->Sdl.BlitNow( 0, 0, ssMenu[1], &rTemp );

					if ( _game->Sdl.GetMouseLButton() == MOUSE_BUTTON_UP )
					{

						//_game->Gameplayers = GP_1PLAYER;
						Menustate = MS_CAMPAIGN_SETUP;
						if ( faces_released ) ReloadCarsAttribs();
					}
					
					break;

				// 1 savage
				case 1:
					rTemp.x = 0;
					rTemp.y = 160;
					rTemp.w = 260;
					rTemp.h = 70;   // 230-160
					_game->Sdl.BlitNow( 0, 160, ssMenu[1], &rTemp );

					if ( _game->Sdl.GetMouseLButton() == MOUSE_BUTTON_UP )
					{
						_game->Gameplayers = GP_1PLAYER;
						Menustate = MS_PLAY_1PLAYER;
						if ( faces_released ) ReloadCarsAttribs();
					}

					_game->Gamemode = GM_1BOT_MELEE;

					break;

				// 2 savages
				case 2:
					rTemp.x = 0;
					rTemp.y = 230;
					rTemp.w = 260;
					rTemp.h = 70;
					_game->Sdl.BlitNow( 0, 230, ssMenu[1], &rTemp );

					if ( _game->Sdl.GetMouseLButton() == MOUSE_BUTTON_UP )
					{
						_game->Gameplayers = GP_2PLAYER;
						Menustate = MS_PLAY_2PLAYER;
						if ( faces_released ) ReloadCarsAttribs();
					}

					_game->Gamemode = GM_1V1;

					break;

				// settings
				case 3:
					rTemp.x = 0;
					rTemp.y = 300;
					rTemp.w = 260;
					rTemp.h = 70;
					_game->Sdl.BlitNow( 0, 300, ssMenu[1], &rTemp );

					if ( _game->Sdl.GetMouseLButton() == MOUSE_BUTTON_UP )
						Menustate = MS_SETTINGS;

					break;


				// resign
				case 4:
					rTemp.x = 0;
					rTemp.y = 370;
					rTemp.w = 260;
					rTemp.h = 70;
					_game->Sdl.BlitNow( 0, 370, ssMenu[1], &rTemp );

					if ( _game->Sdl.GetMouseLButton() == MOUSE_BUTTON_UP )
						Menustate = MS_EXIT;
					break;
				}
			}
		} // end for

	break;

	
	/*// Buy/Fix Screen {!}
	/*case MS_PLAY_SETUPCAR:

		ScrollScreen();
		PreviewCar( current_car );

		if ( buttons_select[0].GetState() == BS_UP )
		{
			// scroll left
			current_car--;
			if ( current_car < 0 ) current_car = _game->Swv.GetVehicles() - 1;
			// PLAY SOUND
			_game->Snd.Play( SND_MENU_NEXTCAR );
		}
		else if ( buttons_select[1].GetState() == BS_UP )
		{
			// scroll right
			current_car++;
			if ( current_car > _game->Swv.GetVehicles() - 1 ) current_car = 0;
			// PLAY SOUND
			_game->Snd.Play( SND_MENU_NEXTCAR );
		}
		else if ( buttons_select[3].GetState() == BS_UP )
		{
			// start game
			//...
			_game->p1_auto_index = current_car; // {!}
		
			_game->Gamestate = GS_GAMEPLAY_SETUP;
			Menustate = MS_STARTSCREEN;
			if ( !faces_released ) ReleaseCarsAttribs();
			
			if ( _game->Gameplayers == GP_1PLAYER )
			{
				_game->game_bots = true;
				_game->game_num_cars = 3;
			}
			else
			{
				_game->game_num_cars = ( _game->game_bots ? 4 : 2 );
			}

		}
		else if ( buttons_select[2].GetState() == BS_UP )
		{
			// back
			Menustate = MS_STARTSCREEN;
			//...
		}
		
		for ( i = 0; i < NUM_BUTTONS_SELECT; i++ )
			buttons_select[i].Update();
				
	break;*/

	// show CAMPAIGN screen
	case MS_CAMPAIGN_SETUP:

		ScrollScreen();
		_game->Sdl.BlitNow( 0, 0, camp_select );

		for ( i = 0; i < 2; i++ )
		{
			rTemp.x = pos_campselect[i].x;
			rTemp.y = pos_campselect[i].y;
			rTemp.w = rTemp.x + 375;
			rTemp.h = rTemp.y + 155;

			rcursor.x = _game->Sdl.GetMouseX();
			rcursor.y = _game->Sdl.GetMouseY();
			rcursor.w = rcursor.x + 8;
			rcursor.h = rcursor.y + 8;

			if ( _game->Sdl.Collide( NULL, &rTemp, &rcursor ) )
			{
				if ( i == 0 )
				{
					_game->Gameplayers = GP_JUDY_CAMPAIGN;
					break;
				} 
				else if ( i == 1 )
				{
					_game->Gameplayers = GP_KEETH_CAMPAIGN;
					break;
				}
			}
		}

		if ( _game->Gameplayers == GP_JUDY_CAMPAIGN ) 
			_game->Sdl.BlitNow( pos_campselect[0].x, pos_campselect[0].y, camp_selquad );	
		else if ( _game->Gameplayers == GP_KEETH_CAMPAIGN )
			_game->Sdl.BlitNow( pos_campselect[1].x, pos_campselect[1].y, camp_selquad );	

		// izberi
		if ( buttons_select[3].GetState() == BS_UP )
		{
			// SELECT
			Menustate = MS_CAMPAIGN_PREVIEW;
			// PLAY SOUND
			_game->Snd.Play( SND_MENU_NEXTCAR );
		}
		else if ( buttons_select[2].GetState() == BS_UP )
			Menustate = MS_STARTSCREEN;  // GO BACK


		if ( _game->Gameplayers == GP_JUDY_CAMPAIGN || _game->Gameplayers == GP_KEETH_CAMPAIGN )
			buttons_select[3].Update( _game );
		
		buttons_select[2].Update( _game );
	break;


	// Preview Campaign
	case MS_CAMPAIGN_PREVIEW:

		if ( _game->player_level >= 7 ) 
		{
			Menustate = MS_CAMPAIGN_WIN;
			return;
		}

		LoadPlayerLevel( _game->player_level );
		_game->Sdl.BlitNow( 0, 0, camp_preview );
		
		// izberi
		if ( buttons_select[3].GetState() == BS_UP )
		{
			// SELECT
			_game->game_bots = false;
			//Menustate = MS_CAMPAIGN_PREVIEW;
			_game->Gamestate = GS_GAMEPLAY_SETUP;
			//Menustate = MS_STARTSCREEN;
			_game->Gametype = GT_TIME;
		}
		else if ( buttons_select[2].GetState() == BS_UP )
		{
			_game->player_level = 1;
			Menustate = MS_CAMPAIGN_SETUP;
			ReleasePlayerLevel();
		}

		buttons_select[3].Update( _game );
		buttons_select[2].Update( _game );
    break;


	// Campaign WINNER
	case MS_CAMPAIGN_WIN:

		LoadPlayerLevel( _game->player_level );
		_game->Sdl.BlitNow( 0, 0, camp_preview );

		dx = (int)_game->Gamediff;
		dx -= 6;
		dx = dx*15+1;
		// blit comment
		rTemp.x = 0;
		rTemp.y = dx;
		rTemp.h = 15;
		rTemp.w = 235;
		_game->Sdl.BlitNow( 5, 460, ssStrings[3], &rTemp );
		
		// izberi
		if ( buttons_select[3].GetState() == BS_UP )
		{
			_game->Gamestate = GS_MENU;
			Menustate = MS_STARTSCREEN;
			ReleasePlayerLevel();
			_game->player_level = 1;
		}

		buttons_select[3].Update( _game );
    break;


	// Prepeare to play screen
	case MS_PLAY_1PLAYER:
	case MS_PLAY_2PLAYER:
		
		ScrollScreen();
		
		PreviewCar( current_car );

		// blit game-modes
		dx = (int)_game->Gamemode;
		rTemp.x = 0;
		rTemp.y = 0;
		rTemp.w = 190;
		rTemp.h = 20;
		_game->Sdl.BlitNow( 0, 340, ssStrings[0], &rTemp );
		
		rTemp.x = 0;
		rTemp.y = (dx) * 20;
		rTemp.w = 190;
		rTemp.h = 20;
		_game->Sdl.BlitNow( 180, 340, ssStrings[0], &rTemp );

		// blit game-type
		dxx = (int)_game->Gametype;
		rTemp.x = 0;
		rTemp.y = 140;
		rTemp.w = 190;
		rTemp.h = 20;
		_game->Sdl.BlitNow( 0, 390, ssStrings[0], &rTemp );
		
		rTemp.x = 0;
		rTemp.y = (dxx + 8) * 20;
		rTemp.w = 190;
		rTemp.h = 20;
		_game->Sdl.BlitNow( 180, 390, ssStrings[0], &rTemp );

		
		if ( !p2_choice )
		{
		  _game->Sdl.BlitNow( 170, 3, savage_select[select_menu] );  // blit text 1st player
		}
		else if ( p2_choice && current_car == _game->p1_auto_index )
		{
			_game->Sdl.SetRect( &rTemp, 0, 0, 400, 30 );
			_game->Sdl.BlitNow( 120, 3, savage_select[select_menu], &rTemp ); // blit text 2nd player
		}
		else
		{
			_game->Sdl.SetRect( &rTemp, 0, 0, 300, 30 );
    		_game->Sdl.BlitNow( 170, 3, savage_select[select_menu], &rTemp ); 
		}

		// button-ACTIONS
		if ( buttons_select[5].GetState() == BS_UP )
		{
			if ( _game->Gameplayers == GP_2PLAYER )
			{
				if ( ++dx > 6 ) dx = 4;
				_game->Gamemode = (CONST_GAMEMODE)dx;
			}
			else if ( _game->Gameplayers == GP_1PLAYER )
			{
				if ( ++dx > 3 ) dx = 1;
				_game->Gamemode = (CONST_GAMEMODE)dx;
			}
		}
		else if ( buttons_select[4].GetState() == BS_UP )
		{
			if ( _game->Gameplayers == GP_2PLAYER )
			{
				if ( --dx < 4 ) dx = 6;
				_game->Gamemode = (CONST_GAMEMODE)dx;
			}
			else if ( _game->Gameplayers == GP_1PLAYER )
			{
				if ( --dx < 1 ) dx = 3;
				_game->Gamemode = (CONST_GAMEMODE)dx;
			}
		}
		else if ( buttons_select[6].GetState() == BS_UP )
		{
			if ( --dxx < 0 ) dxx = 1;
			_game->Gametype = (CONST_GAMETYPE)dxx;
		}
		else if ( buttons_select[7].GetState() == BS_UP )
		{
			if ( ++dxx > 1 ) dxx = 0;
			_game->Gametype = (CONST_GAMETYPE)dxx;
		}
		else if ( buttons_select[0].GetState() == BS_UP )
		{
			// scroll left
			current_car--;

			// block Truck if cheat disabled
			if ( !strncmp(_game->Swv.GetVehiclesData()[current_car].vehiclename, "DTRUCK", 5)
					&& !_game->game_cheats )
			{
				current_car--;
			}

			if ( current_car < 0 )
				current_car = _game->Swv.GetVehicles() - 1;

			// PLAY SOUND
			_game->Snd.Play( SND_MENU_NEXTCAR );
		}
		else if ( buttons_select[1].GetState() == BS_UP )
		{
			// scroll right
			current_car++;

			// block Truck if cheat disabled
			if ( !strncmp(_game->Swv.GetVehiclesData()[current_car].vehiclename, "DTRUCK", 5)
					&& !_game->game_cheats )
			{
				current_car++;
			}

			if ( current_car > _game->Swv.GetVehicles() - 1 )
				current_car = 0;

			// PLAY SOUND
			_game->Snd.Play( SND_MENU_NEXTCAR );
		}
		else if ( buttons_select[3].GetState() == BS_UP )
		{
			// start game
			if ( _game->Gameplayers == GP_1PLAYER )
			{
				if ( !faces_released ) ReleaseCarsAttribs();				
				
				_game->p1_auto_index = current_car;
				_game->game_bots = true;
				_game->game_num_cars = 3;
				_game->Gamestate = GS_GAMEPLAY_SETUP;
				Menustate = MS_STARTSCREEN;
			}
			else
			{
				// did player2 made a choice
				if ( !p2_choice )
				{
					_game->p1_auto_index = current_car;
					p2_choice = !p2_choice;
					select_menu = 1;
				} 
				else if ( p2_choice && current_car != _game->p1_auto_index )
				{
					// start 2 Player game
					if ( !faces_released ) ReleaseCarsAttribs();				
					p2_choice = !p2_choice;
					
					_game->p2_auto_index = current_car;
					_game->game_num_cars = ( _game->game_bots ? 4 : 2 );
					_game->Gamestate = GS_GAMEPLAY_SETUP;
					Menustate = MS_STARTSCREEN;
					select_menu = 0;
				}
			}

		}
		else if ( buttons_select[2].GetState() == BS_UP )
		{
			Menustate = MS_STARTSCREEN;
			p2_choice = false;
			select_menu = 0;
		}

 		for ( i = 0; i < NUM_BUTTONS_SELECT; i++ )
			buttons_select[i].Update( _game );
				
	break;


	// game-settings
	case MS_SETTINGS:
				
		ScrollScreen();

		for ( i = 0; i < MENU_OPTIONS; i++ )
		{
			// blit menu text
			rsrc.x = 0;
			rsrc.w = MENU_TEXTWIDTH;
			rsrc.h = MENU_TEXTHEIGHT;

			if ( i == 6 ) 
			{
				rsrc.y = (i + 3) * MENU_TEXTHEIGHT;
			}
			else
			{
				rsrc.y = i * MENU_TEXTHEIGHT;
			}

			_game->Sdl.BlitNow( pos_options[i].x, pos_options[i].y, ssStrings[BTXT_OPTIONS], &rsrc );
		}

		// blit/update buttons
		for ( i = 0; i < NUM_BUTTONS_OPTIONS + 1; i++ )
			buttons_options[i].Update( _game );

		// check buttons
		for ( i = 0; i < NUM_BUTTONS_OPTIONS + 1; i++ )
		{
			if ( buttons_options[i].GetState() == BS_UP )
			{
				switch( i )
				{
				case 0: 
					dx = (int)_game->Gamediff;
					dx -= 1;
					_game->Gamediff = (CONST_DIFFICULTY)dx;
					if ( _game->Gamediff < DIFF_EASY ) _game->Gamediff = DIFF_HARD;
					break;

				case 1:
					dx = (int)_game->Gamediff;
					dx += 1;
					_game->Gamediff = (CONST_DIFFICULTY)dx;
					if ( _game->Gamediff > DIFF_HARD ) _game->Gamediff = DIFF_EASY;
					break;

				case 6:
				case 7:
					if ( _game->game_shadows ) 
						_game->game_shadows = false;
					else 
						_game->game_shadows = true;
					
					break;

				case 8:
					_game->Sdl.ChangeSoundVolume( -10 );
					break;

				case 9:
					_game->Sdl.ChangeSoundVolume( 10 );
					break;

				case 10:
					_game->Sdl.ChangeMusicVolume( -10 );
					break;

				case 11:
					_game->Sdl.ChangeMusicVolume( 10 );
					break;

				//case 12:
				//case 13:
				//	_game->game_hitmode = !_game->game_hitmode;
				//	break;

				case 12://14:
					Menustate = MS_STARTSCREEN;
					break;

				}
			}
			else if ( buttons_options[i].GetState() == BS_DOWN )
			{

				switch( i )
				{
				case 2:
					if ( gametime_press < _game->Timer.Time() )
					{
						_game->game_time -= 30;
						if ( _game->game_time <= 0 ) _game->game_time = 1800;
						gametime_press = _game->Timer.Time() + 110;
					}
					break;


				case 3:
					if ( gametime_press < _game->Timer.Time() )
					{
						_game->game_time += 30;
						if ( _game->game_time > 1800 ) _game->game_time = 30;
						gametime_press = _game->Timer.Time() + 110;
					}
					break;

				case 4:
					if ( frag_time < _game->Timer.Time() )
					{
						if ( --_game->game_frags < 1  ) _game->game_frags = 999;
						frag_time = _game->Timer.Time() + 85;
					}
					break;

				case 5:
					if ( frag_time < _game->Timer.Time() )
					{
						if ( ++_game->game_frags > 999  ) _game->game_frags = 1;
						frag_time = _game->Timer.Time() + 85;
					}
					break;
				}
			} // end if
		} // end for

		dx = pos_options[0].x + 235;
		
		// --- blit option_texts ---
		
		
		// game difficulties

		rsrc.x = 0;
		rsrc.y = (int)_game->Gamediff * MENU_TEXTHEIGHT;
		rsrc.w = MENU_TEXTWIDTH;
		rsrc.h = MENU_TEXTHEIGHT;

		_game->Sdl.BlitNow( dx, pos_options[0].y, ssStrings[BTXT_OPTIONS], &rsrc );

		
		// time limit
		dxx = _game->game_time / 60;
		dxxx = _game->game_time % 60;

		sprintf( buf, "%.2d", dxx );
		_game->Sdl.DrawNum( dx, pos_options[1].y, buf );
		sprintf( buf, "%.2d", dxxx );
		_game->Sdl.DrawNum( dx+25, pos_options[1].y, buf );
		
		//_game->Sdl.BlitNow( dx, pos_options[1].y, ssStrings[BTXT_ONOFF], &rsrc );

		// frag limit
		sprintf( buf, "%d", _game->game_frags );
		_game->Sdl.DrawNum( dx, pos_options[2].y, buf );
		
		// shadows
		rsrc.x = (_game->game_shadows == true ? 0 : 1) * 30;
		rsrc.y = 0;
		rsrc.w = rsrc.x + 30;
		rsrc.h = MENU_TEXTHEIGHT;
		
		_game->Sdl.BlitNow( dx, pos_options[3].y, ssStrings[BTXT_ONOFF], &rsrc );

		//// HitMode
		//rsrc.x = (_game->game_hitmode ? 1 : 0) * 30;
		//rsrc.w = rsrc.x + 30;
		//_game->Sdl.BlitNow( dx, pos_options[6].y, ssStrings[BTXT_ONOFF], &rsrc );

		// sound volume
		if ( _game->Sdl.GetSoundVolume() > 0 )
		{
			rsrc.x = 0;
			rsrc.y = 0;
			rsrc.w = _game->Sdl.GetSoundVolume();
			rsrc.h = MENU_TEXTHEIGHT;
			_game->Sdl.BlitNow( dx, pos_options[4].y + 5, ssVolume, &rsrc );
		}
		else
		{
			rsrc.x = 30;
			rsrc.y = 0;
			rsrc.w = rsrc.x + 30;
			rsrc.h = MENU_TEXTHEIGHT;
			_game->Sdl.BlitNow( dx, pos_options[4].y, ssStrings[BTXT_ONOFF], &rsrc );
		}

		// music volume
		if ( _game->Sdl.GetMusicVolume() > 0 )
		{
			rsrc.x = 0;
			rsrc.y = 0;
			rsrc.w = _game->Sdl.GetMusicVolume();
			rsrc.h = MENU_TEXTHEIGHT;
			_game->Sdl.BlitNow( dx, pos_options[5].y + 5, ssVolume, &rsrc );
		}
		else
		{
			rsrc.x = 30;
			rsrc.y = 0;
			rsrc.w = rsrc.x + 30;
			rsrc.h = MENU_TEXTHEIGHT;
			_game->Sdl.BlitNow( dx, pos_options[5].y, ssStrings[BTXT_ONOFF], &rsrc );
		}
	break;

	case MS_EXIT:
		_game->Gamestate = GS_EXIT;
		break;
	}

	// blit cursor
	_game->Sdl.BlitNow( _game->Sdl.GetMouseX(), _game->Sdl.GetMouseY(), ssCursor[(int)cur_frame] );
	cur_frame += _game->getMpf() * 25;
	if ( cur_frame > 16 ) 
		cur_frame = 0;

}



//---------------------------- Class Button ----------------



//////////////////////////////////////////////////////////////////////
// Ime: Initialize()
// Opisanie: init nav_button
//////////////////////////////////////////////////////////////////////
void CButton::Initialize( POINT *pos, Uint32 picture_index )
{
	x = pos->x;
	y = pos->y;
	pic_index = picture_index;
	type = BT_NAVIGATION;

	width = ssButtons[pic_index]->w;
	height = ssButtons[pic_index]->h;
}



//////////////////////////////////////////////////////////////////////
// Ime: Initialize()
// Opisanie: init button with text
//////////////////////////////////////////////////////////////////////
void CButton::Initialize( POINT *pos, Uint32 picture_index, Uint32 txt_index, Uint32 txtpicture_index )
{
	x = pos->x;
	y = pos->y;
	tx = x + 10;
	ty = y + 10;
	pic_index = picture_index;
	text_pic_index = txtpicture_index;
	text_index = txt_index;
	type = BT_TEXT;

	width = ssButtons[pic_index]->w;
	height = ssButtons[pic_index]->h;

}



//////////////////////////////////////////////////////////////////////
// Ime: Update()
// Opisanie: 
//////////////////////////////////////////////////////////////////////
void CButton::Update( CGame *game )
{

	Uint32		pIndex = pic_index;
	SDL_Rect	rcursor, rbutton, rsrc;

	rcursor.x = game->Sdl.GetMouseX();
	rcursor.y = game->Sdl.GetMouseY();
	rcursor.w = rcursor.x + 8;
	rcursor.h = rcursor.y + 8;

	rbutton.x = x;
	rbutton.y = y;
	rbutton.w = x + width;
	rbutton.h = y + height;

	// highlight if needed (mouseover)
	if ( game->Sdl.Collide( NULL, &rcursor, &rbutton ) )
	{
		pIndex += 1;
		state = BS_MOUSEOVER;

		// check mickey
		if ( game->Sdl.GetMouseLButton() == MOUSE_BUTTON_DOWN )
			state = BS_DOWN;
		
		if ( game->Sdl.GetMouseLButton() == MOUSE_BUTTON_UP )
		{
			state = BS_UP;
			// PLAY SOUND
			game->Snd.Play( SND_MENU_CLICK );
			//over_sound = false;
		}
		//else
		//{
		//	state = BS_MOUSEOVER;
		//	// PLAYSOUND
		//	/*if ( !over_sound )
		//	{		
		//		_game->Snd.Play( SND_MENU_OVERBUTTON );
		//		over_sound = true;
		//	}*/
		//}

	}
	else
	{
		// button lost focus
		if ( state == BS_DOWN )
			state = BS_NORMAL;
		
		over_sound = false;
	}

	// blit button
	game->Sdl.BlitNow( x, y, ssButtons[pIndex] );

	if ( type == BT_TEXT )
	{
		// blit text
		rsrc.x = 0;
		rsrc.y = text_index * MENU_TEXTHEIGHT;
		rsrc.w = MENU_TEXTWIDTH;
		rsrc.h = MENU_TEXTHEIGHT;

		//sdl->BlitNow( tx, ty, ssStrings[text_pic_index], &rsrc );
	}
}



//////////////////////////////////////////////////////////////////////
// Ime: SaveSettings()
// Opisanie: save game options
//////////////////////////////////////////////////////////////////////
void CMainMenu::SaveSettings()
{
	if ( _game == NULL )
	  return;

	FILE *fp;
	int   tmp_int;
	char  header[3] = { 'S', 'W', 'P' };

	if ( ( fp = fopen( "pref", "wb" ) ) == NULL )
	{
		AppendToLog( "Error writing to /pref file !" );
		return;
	}

	tmp_int = (int)_game->Gamediff;

	fwrite( header, sizeof(char) * 3, 1, fp );
	fwrite( &tmp_int, sizeof(tmp_int), 1, fp );
	fwrite( &_game->game_shadows, sizeof(_game->game_shadows), 1, fp );
	fwrite( &_game->game_bots, sizeof(_game->game_bots), 1, fp );
	fwrite( &_game->game_frags, sizeof(_game->game_frags), 1, fp );
	fwrite( &_game->game_time, sizeof(_game->game_time), 1, fp );

	tmp_int = _game->Sdl.GetSoundVolume();
	fwrite( &tmp_int, sizeof(tmp_int), 1, fp );
	tmp_int = _game->Sdl.GetMusicVolume();
	fwrite( &tmp_int, sizeof(tmp_int), 1, fp );

	fwrite( &_game->game_hitmode, sizeof(_game->game_hitmode), 1, fp );

	fclose( fp );
}



//////////////////////////////////////////////////////////////////////
// Ime: LoadSettings()
// Opisanie: loag game options
//////////////////////////////////////////////////////////////////////
void CMainMenu::LoadSettings()
{

	FILE  *fp		= NULL;
	int   tmp_int	= 0;
	char  header[3];
	bool  success	= true;

	if ( ( fp = fopen( "pref", "rb" ) ) == NULL )
	{
		AppendToLog( "Error opening /pref file !" );
		success = false;
	}

	if ( success )
	{
		fread( header, sizeof(char) * 3 , 1, fp );
		
		if ( header[0] != 'S' && header[1] != 'W' && header[2] != 'P' )
			success = false;
	}

	if ( ! success )
	{
		_game->Gamediff = DIFF_NORMAL;
		_game->game_shadows = true;
		_game->game_bots = true;
		_game->game_frags = 50;
		_game->game_time = 180;
		_game->Sdl.SetSoundVolume( 100 );
		_game->Sdl.SetMusicVolume( 80 );
		_game->game_hitmode = false;
	}
	else
	{
	
		fread( &tmp_int, sizeof(tmp_int), 1, fp );
		if ( tmp_int < 6 || tmp_int > 8 ) tmp_int = 6;	
		_game->Gamediff = (CONST_DIFFICULTY)tmp_int;
	
		fread( &_game->game_shadows, sizeof(_game->game_shadows), 1, fp );
		fread( &_game->game_bots, sizeof(_game->game_bots), 1, fp );
		fread( &_game->game_frags, sizeof(_game->game_frags), 1, fp );
		fread( &_game->game_time, sizeof(_game->game_time), 1, fp );
	
		fread( &tmp_int, sizeof(tmp_int), 1, fp );
		_game->Sdl.SetSoundVolume( tmp_int );
		fread( &tmp_int, sizeof(tmp_int), 1, fp );
		_game->Sdl.SetMusicVolume( tmp_int );

		fread( &_game->game_hitmode, sizeof(_game->game_hitmode), 1, fp );
	}

	if ( fp )
		fclose( fp );
}



//////////////////////////////////////////////////////////////////////
// Ime: ReloadCarsAttribs()
// Opisanie: 
//////////////////////////////////////////////////////////////////////
int CMainMenu::ReloadCarsAttribs()
{

	ssCarFace = new SDL_Surface*[_game->Swv.GetVehicles()];
	ssDriverFace = new SDL_Surface*[_game->Swv.GetVehicles()];
	ssDriverName = new SDL_Surface*[_game->Swv.GetVehicles()];

	bool loaded = true;

	for ( int i = 0; i < _game->Swv.GetVehicles(); i++ )
	{
		// load car faces
		loaded = ( ssCarFace[i] = _game->Sdl.LoadBitmap(
				_game->Swv.GetVehicleFilename( i ),
				_game->Swv.GetFacePos( i ),
				_game->Swv.GetFaceSize( i ))) == NULL && loaded;

		// load drivers
		loaded = ( ssDriverFace[i] = _game->Sdl.LoadBitmap(
				_game->Swv.GetVehicleFilename( i ),
				_game->Swv.GetDriverFacePos( i ),
				_game->Swv.GetDriverFaceSize( i ))) == NULL && loaded;

		// load driver names
		loaded = ( ssDriverName[i] = _game->Sdl.LoadBitmap(
				_game->Swv.GetVehicleFilename( i ),
				_game->Swv.GetNamePos( i ) ,
				_game->Swv.GetNameSize( i ),
				BLACK)) == NULL && loaded;
	}

	faces_released = false;
	
	return loaded;
}



//////////////////////////////////////////////////////////////////////
// Ime: ReleaseCarsAttribs()
// Opisanie: 
//////////////////////////////////////////////////////////////////////
void CMainMenu::ReleaseCarsAttribs()
{
	for ( int i = 0; i < _game->Swv.GetVehicles(); i++ )
	{
		RELEASE_SURF( ssCarFace[i] );
		RELEASE_SURF( ssDriverFace[i] );
		RELEASE_SURF( ssDriverName[i] );
	}

	if (ssCarFace)
		delete[] ssCarFace;

	if (ssDriverFace)
		delete[] ssDriverFace;

	if (ssDriverName)
		delete[] ssDriverName;

	faces_released = true;
}



//////////////////////////////////////////////////////////////////////
// Ime: PreviewCar()
// Opisanie: pokaji main_pic i harakteristiki na kolata
//////////////////////////////////////////////////////////////////////
void CMainMenu::PreviewCar( int car_index )
{

	int		      dy = 195, dx = 325;
	SDL_Rect      rsrc;
	static int    perc[5];
	static int    old_car_index = -1;

	// precalculate perc
	if ( old_car_index != car_index )
	{
		// 1.3f = 130.0f / 100.0f
		perc[0] = (Uint16)(1.3f * ( (float)attribSPEED[(int)_game->Swv.GetVehiclesData()[car_index].max_vel] / (float)attribSPEED[4] ) * 100.0f);
		perc[1] = (Uint16)(1.3f * ( (float)attribACC[(int)_game->Swv.GetVehiclesData()[car_index].acc] / (float)attribACC[2] ) * 100.0f);
		perc[2] = (Uint16)(1.3f * ( (float)_game->Swv.GetVehiclesData()[car_index].lbs / (float)4 ) * 100.0f);
		perc[3] = (Uint16)(1.3f * ( (float)attribARMOUR[(int)_game->Swv.GetVehiclesData()[car_index].hp] / (float)attribARMOUR[3] ) * 100.0f);
		perc[4] = (Uint16)(1.3f * ( (float)attribDAMAGE[(int)_game->Swv.GetVehiclesData()[car_index].damage] / (float)attribDAMAGE[3] ) * 100.0f);

		old_car_index = car_index;
	}
	
	_game->Sdl.BlitNow( 168, 30, ssDriverName[car_index] );
	_game->Sdl.BlitNow( 168, 45, ssDriverFace[car_index] );
	_game->Sdl.BlitNow( 270, 45, ssCarFace[car_index] );
	_game->Sdl.BlitNow( dx - 115, dy - 5, ssCarAttribs );
	// 195, 330(-20)

	// blit speed-scale
	_game->Sdl.SetRect( &rsrc, 0, 0, perc[0], 10 );
	_game->Sdl.BlitNow( dx, dy, ssVolume, &rsrc );
	
	// blit acc-scale
	rsrc.w = perc[1];
	_game->Sdl.BlitNow( dx, dy + 20, ssVolume, &rsrc );

	// blit lbs-scale
	//perc = ( (float)3 / (float)4 ) * 100.0f;
	rsrc.w = perc[2];
	_game->Sdl.BlitNow( dx, dy + 40, ssVolume, &rsrc );

	// blit armour-scale
	rsrc.w = perc[3];
	_game->Sdl.BlitNow( dx, dy + 60, ssVolume, &rsrc );

	// blit damage-scale
	rsrc.w = perc[4];
	_game->Sdl.BlitNow( dx, dy + 80, ssVolume, &rsrc );

	// upgrades
	static float frame = 0;

	dy = dy - 35; // 200;
	dx = dx - 160; //70;
	_game->Sdl.BlitNow( dx, dy, ssUpg[ANIM_ENGINE][(int)frame] );
	_game->Sdl.BlitNow( dx, dy + 40, ssUpg[ANIM_NOS][(int)frame] );
	_game->Sdl.BlitNow( dx, dy + 80, ssUpg[ANIM_ARMOUR][(int)frame] );
	_game->Sdl.BlitNow( dx, dy + 110, ssUpg[ANIM_DAMAGE][(int)frame] );

	frame += _game->getMpf() * 25;
	if ( frame > 34 ) frame = 0;
}



//////////////////////////////////////////////////////////////////////
// Ime: ScrollScreen()
// Opisanie: Scroll-ekrana
//////////////////////////////////////////////////////////////////////
void CMainMenu::ScrollScreen()
{

	static float  y_move = 0;
	int			  tmp_y;
	SDL_Rect	  rect1, rect2;

	y_move += 50 * _game->getMpf();
	tmp_y = SCREEN_HEIGHT - (int)y_move;

	// scroll up 
	// face 1
	_game->Sdl.SetRect( &rect1, 0, (int)y_move, SCREEN_WIDTH, SCREEN_HEIGHT );
	_game->Sdl.BlitNow( 0, 0, ssBack[0], &rect1 );
	// face 2
	_game->Sdl.SetRect( &rect2, 0, 0, SCREEN_WIDTH, (int)y_move );	
	_game->Sdl.BlitNow( 0, tmp_y, ssBack[0], &rect2 );

	/*// scroll down
	_game->Sdl.SetRect( &rect1, 0, tmp_y, SCREEN_WIDTH, SCREEN_HEIGHT );
	_game->Sdl.BlitNow( 0, 0, ssBack[0], &rect1 );
    _game->Sdl.SetRect( &rect2, 0, 0, SCREEN_WIDTH, tmp_y );
	_game->Sdl.BlitNow( 0, (int)y_move, ssBack[0], &rect2 );
	*/

	if ( y_move >= SCREEN_HEIGHT ) 
		y_move = 0;
}



//////////////////////////////////////////////////////////////////////
// Ime: DoTitle()
// Opisanie: 
//////////////////////////////////////////////////////////////////////
void CMainMenu::DoTitle()
{

	static float cur_frame = 0;
	static float reminder = 1;

	cur_frame = cur_frame + reminder * (_game->getMpf() * 15);
	
	if ( cur_frame >= 10 ) 
	{
		reminder = -1;
		cur_frame--;
	}
	else if ( cur_frame <= 0 )
	{
		reminder = 1;
		cur_frame++;
	}

	
	_game->Sdl.BlitNow( 340, 40, ssTitle[(int)cur_frame] );
}



//////////////////////////////////////////////////////////////////////
// Ime: LoadPlayerLevel()
// Opisanie: 
//////////////////////////////////////////////////////////////////////
bool CMainMenu::LoadPlayerLevel( int level )
{
	char		   buf[255];
	static Uint16  last_level = 255;

	if ( last_level == level && camp_preview != NULL ) 
		return true;
	else
		last_level = level;

	if ( _game->Gameplayers == GP_JUDY_CAMPAIGN )
		sprintf( buf, "gfx/interf/judy%d.bmp", level );
	else if ( _game->Gameplayers == GP_KEETH_CAMPAIGN )
		sprintf( buf, "gfx/interf/keeth%d.bmp", level );

	if ( (camp_preview = _game->Sdl.LoadBitmap( buf )) == NULL )
		return false;

	return true;
}



///////////////////////////////////////////////////////////////////////
// Ime: UpdateCredits()
// Opisanie: update credit list
///////////////////////////////////////////////////////////////////////
void CMainMenu::UpdateCredits()
{

	static int   phase = -1;
	static float x = 650, acc = 0;
	static int	 time = 0;
	SDL_Rect     rect;

	if ( time < _game->Timer.Time()  )
	{
		time = _game->Timer.Time() + 4500;
		if ( ++phase > 5 ) phase = 0;

		x = 650;
		acc = 0;
	}
	else
	{
		rect.x = 0;
		rect.y = ( phase == 5 ) ? 300  : phase * 50 + 1;
		rect.w = 220;
		rect.h = ( phase == 4 ) ? 60  : 50;

		acc += (6.0f * _game->getMpf());
		x -= acc;
		if ( x < 380 ) x = 380;

		_game->Sdl.BlitNow( (int)x, 350, credits, &rect );	
	}
}

