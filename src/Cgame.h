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

#ifndef __CGAME
#define __CGAME

//////////////////////
class CSdl;
class CSounds;
class CTimer;
class CDeadtoy;
class CLandMines;
class CVehicle;
class CKdf_Packeger;
class CSwv_module;
class CDeadtoys;
class CLandMines;
//////////////////////


#define APP_NAME		   "Savage Wheels 1.04 (BETA)"
#define NUM_MPS			   4
#define NUM_BACKGROUNDS    5
#define SCREEN_WIDTH       640
#define SCREEN_HEIGHT      480
#define SCREEN_PLAYWIDTH   640
#define SCREEN_PLAYHEIGHT  480  // 420
#define TIME_SHOWWINNER	   10000
#define MAX_TIRETRAILS	   1


// game states
enum CONST_GAMESTATE
{
	GS_MENU = 0,
	GS_GAMEPLAY_SETUP,
    GS_GAMEPLAY,
	GS_GAMEPLAY_WINNER,
	GS_EXIT,
	// campaign states
	GS_CAMPAIGN,
	GS_CAMPAIGN_WIN
};


// game-modes
enum CONST_GAMEMODE 
{
	GM_1BOT_MELEE = 1,
	GM_2BOTS_MELEE,
	GM_3BOTS_MELEE,
	GM_1V1,
	GM_MELEE,
	GM_2V2
};


// game-type
enum CONST_GAMETYPE
{
	GT_FRAGS = 0,
	GT_TIME
};


// game_mode
enum CONST_GAMEPLAYERS
{
	GP_1PLAYER = 0,
	GP_2PLAYER,
	GP_JUDY_CAMPAIGN,
	GP_KEETH_CAMPAIGN
};


// CAMPAIGN Vehicles
enum CONST_CAMPAIGN_VEHICLES //_CONST_CAMPAIGN_VEHICLES
{
	CV_BMW = 0,
	CV_MERCEDES,
	CV_DIABLO,
	CV_DODGE,
	CV_DJUGAN,
	CV_JEEP,
	CV_F1,
	CV_DTRUCK
};


class CGame 
{
 
private:

	bool			bRunning;
	bool			quit_dialog;
	float			mpf;			// motion per frame (hires-timing var)

	SDL_Surface		*but_quit[5];
	SDL_Surface		*but_pause;
	SDL_Surface		*pause_surf;
	SDL_Surface		*winneris;
	SDL_Surface		*splash;
	SDL_Surface		*splash_logo[30];
	SDL_Surface	    *getready;

	//SDL_Surface     *sprite_tire[MAX_TIRETRAILS]; 
	SDL_Surface		*back[NUM_BACKGROUNDS]; // ...

public:

	CMainMenu       Menu;
	CSdl			Sdl;
	CSounds			Snd;
	CTimer			Timer;
	CAnimations	    Anims;
	CVehicle		Auto[ NUM_MPS ];
	CKdf_Packeger   Kdf;					// KDF class
	CSwv_module		Swv;					// vehicles module class
	CDeadtoys		Dtoys;					// dead_toys class
	CLandMines		Mines;					// landmines class
	CBindings		Bindings;

	//--- game specific vars
	SDL_Rect		  rScreen;		// screen rectangle

	CONST_GAMESTATE	  Gamestate;
	CONST_GAMEPLAYERS Gameplayers;	
	CONST_DIFFICULTY  Gamediff;
	CONST_GAMEMODE	  Gamemode;
	CONST_GAMETYPE	  Gametype;

	bool			  game_hitmode;			// stop phsysics enabled !?
	bool			  game_shadows;
	bool			  game_bots;
	int				  game_frags;			// frags to win
	int				  game_time;			// round time
	bool			  show_fps;// = false;
	bool			  game_paused;
	int				  game_num_cars;	    // number of active vehicles
	bool			  game_cheats;// = false;
	Uint32			  p1_auto_index;
	Uint32			  p2_auto_index;
	Uint16			  player_level;// = 1;		// campaign_player_game_level

	Uint32			 cur_map;       			// current visible background map

	SDL_Surface      *scales[2];				// hit_points & anger scales
	SDL_Surface		 *self_dest;				// self_destruct pic

private:

	void UpdateSplash();
	void UpdateCredits();
	void SetupVehicles();
	bool LoadGame();

public:

	CGame();
	~CGame();

	void Execute( int bFullScreen = false, int bHardware = false);
	void Close();
	const float getMpf()	{ return mpf; };

};


#endif