// CDeadToy.cpp - bonus toys class


#include "main.h"


Uint16 dt_values[DT_MAX_DEADTOYS]		= { 1, 66, 75, 1, 3 }; //bear,anger,hp,speed,landmine
Uint32 dt_timespawn[DT_MAX_DEADTOYS]	= { 45000, 4000, 15000, 8000, 10000 };
Uint32 dt_timestay[DT_MAX_DEADTOYS]		= { 6000, 4000, 8000, 3000, 5000 };


////////////////////////////////////////Class DeadToys///////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
// Ime: CDeadtoys()
// Opisanie: konstructor
///////////////////////////////////////////////////////////////////////
CDeadtoys::CDeadtoys()
: _game( NULL ), goal_index( 0 ), goal_state( DTG_ONTHEROAD )
{
	for ( Uint32 i = 0; i < DT_MAX_DEADTOYS; i++ )
		for ( Uint32 j = 0; j < 2; j++ )
			sprite[i][j] = NULL;
}


///////////////////////////////////////////////////////////////////////
// Ime: ~CDeadtoys()
// Opisanie: destructor
///////////////////////////////////////////////////////////////////////
CDeadtoys::~CDeadtoys()
{
	
}



///////////////////////////////////////////////////////////////////////
// Ime: Release()
// Opisanie:  
///////////////////////////////////////////////////////////////////////
void CDeadtoys::Release()
{
	AppendToLog("Closing CDeadToys() class...");

	for ( Uint32 i = 0; i < DT_MAX_DEADTOYS; i++ )
		for ( Uint32 j = 0; j < 2; j++ )
			RELEASE_SURF( sprite[i][j] );

	//delete[] sprite;
}


///////////////////////////////////////////////////////////////////////
// Ime: Reset()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
void CDeadtoys::Reset()
{
	memset( CDt, NULL, sizeof(CDeadtoy) * DT_MAX_DEADTOYS ) ;
}



///////////////////////////////////////////////////////////////////////
// Ime: Initialize()
// Opisanie: inicializira/zarejda dead_toya 
///////////////////////////////////////////////////////////////////////
int CDeadtoys::Initialize( CGame *game )
{
	char dirname[255];

	this->_game = game;
	ASSERT( _game != NULL );

	//sprite = (SDL_Surface ***) new SDL_Surface[DT_MAX_DEADTOYS][1];
	//sprite = (SDL_Surface ***)malloc( sizeof(SDL_Surface)*(DT_MAX_DEADTOYS)*2 );

	// load frag-bug
	//sprintf( dirname, "gfx/dtoys/dtoy%d.bmp", i );
	if ( ( sprite[0][0] = _game->Sdl.LoadBitmap( "gfx/dtoys/dtoy01.bmp", MAGENTA )) == NULL ) return 0; 
	if ( ( sprite[0][1] = _game->Sdl.LoadBitmap( "gfx/dtoys/dtoy02.bmp", MAGENTA )) == NULL ) return 0; 
	// load anger-bug
	if ( ( sprite[1][0] = _game->Sdl.LoadBitmap( "gfx/dtoys/dtoy11.bmp", MAGENTA )) == NULL ) return 0; 
	if ( ( sprite[1][1] = _game->Sdl.LoadBitmap( "gfx/dtoys/dtoy12.bmp", MAGENTA )) == NULL ) return 0; 

	// load other-bugs	
	for ( Uint32 i = 2; i < DT_MAX_DEADTOYS; i++ )
	{
		sprintf( dirname, "gfx/dtoys/dtoy%d.bmp", i );
		if ( ( sprite[i][0] = _game->Sdl.LoadBitmap( dirname, MAGENTA )) == NULL ) return 0; 
	}

	return 1;
}


///////////////////////////////////////////////////////////////////////
// Ime: Create()
// Opisanie: syzdava dead_toya na ekrana
///////////////////////////////////////////////////////////////////////
void CDeadtoys::Create( Uint16 pos_x, Uint16 pos_y, CONST_DEADTOYS toy_type )
{
	
	Uint32 i = 0;
	CDeadtoy  *ptr_cdt;	

	ptr_cdt = CDt;

	while( i < DT_MAX_CHILDS )
	{

		if ( !ptr_cdt->alive  )
		{
			ptr_cdt->x = (float)pos_x;
			ptr_cdt->y = (float)pos_y;
			ptr_cdt->value = dt_values[toy_type];
			ptr_cdt->alive = true;
			ptr_cdt->life = _game->Timer.Time() + dt_timestay[toy_type];
			ptr_cdt->type = toy_type;

			// set anims
			ptr_cdt->frame = 0;
			ptr_cdt->max_frames = 0;

			// prisvoi na glb vars v klasa inf za kyde e goal-a
			if ( toy_type == DT_BEARGOAL )
			{
				ptr_cdt->max_frames = 1;
				goal_index = i;
				goal_state = DTG_ONTHEROAD;
			}
			else if ( toy_type == DT_BEARANGER )
			{
				ptr_cdt->max_frames = 1;
			}
			
			// dtoy's created now exit
			break;
		}
		
		i++;
		ptr_cdt++;
	}
}


///////////////////////////////////////////////////////////////////////
// Ime: Update()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
void CDeadtoys::Update()
{
	Uint32	  i;
	Uint16    dx, dy;
	Uint32	  cur_ticks = _game->Timer.Time();
	CDeadtoy  *ptr_cdt;	
	
	static Uint32  tick_last_goaltoy = 0;
	static Uint32  tick_last_hptoy = 0;
	static Uint32  tick_last_angertoy = 0;
	static Uint32  tick_last_speedtoy = 0;
	static Uint32  tick_last_landminetoy = 0;

	// proveri dali ne e vreme da se syzdade nova igrachka
	if ( tick_last_goaltoy < cur_ticks && goal_state == DTG_ONTHEROAD )
	{
		tick_last_goaltoy = cur_ticks + dt_timespawn[DT_BEARGOAL];
		GetSafePosition( &dx, &dy, DT_TOYSPAWNRANGE );
		Create( dx, dy, DT_BEARGOAL );
	}
	if ( tick_last_hptoy < cur_ticks )
	{
		tick_last_hptoy = cur_ticks + dt_timespawn[DT_BEARHP];
		GetSafePosition( &dx, &dy, DT_TOYSPAWNRANGE );
		Create( dx, dy, DT_BEARHP );
	}
	if ( tick_last_angertoy < cur_ticks )
	{
		tick_last_angertoy = cur_ticks + dt_timespawn[DT_BEARANGER];
		GetSafePosition( &dx, &dy, DT_TOYSPAWNRANGE );
		Create( dx, dy, DT_BEARANGER );
	}
	if ( tick_last_speedtoy < cur_ticks )
	{
		tick_last_speedtoy = cur_ticks + dt_timespawn[DT_BEARSPEED];
		GetSafePosition( &dx, &dy, DT_TOYSPAWNRANGE );
		Create( dx, dy, DT_BEARSPEED );
	}
	if ( tick_last_landminetoy < cur_ticks )
	{
		tick_last_landminetoy = cur_ticks + dt_timespawn[DT_BEARLANDMINE];
		GetSafePosition( &dx, &dy, DT_TOYSPAWNRANGE );
		Create( dx, dy, DT_BEARLANDMINE );
	}

	
	// point v nachaloto na array-a
	ptr_cdt = CDt;
    // loop & updata all
	for( i = 0; i < DT_MAX_CHILDS; i++ )
	{
		if ( ptr_cdt->alive )
		{
			// inc animation frame
			ptr_cdt->frame += 7 * _game->getMpf(); ;
			if ( (int)ptr_cdt->frame > ptr_cdt->max_frames ) ptr_cdt->frame = 0;

			_game->Sdl.BlitShadow( (int)ptr_cdt->x + 2, (int)ptr_cdt->y + 3, sprite[ptr_cdt->type][(int)ptr_cdt->frame] );
			_game->Sdl.Addtoblit( (int)ptr_cdt->x, (int)ptr_cdt->y, sprite[ptr_cdt->type][(int)ptr_cdt->frame] );
			
			// see if its time to unwarp the toy
			if ( cur_ticks > ptr_cdt->life ) //&& ptr_cdt->type != DT_BEARGOAL )
			{
				ptr_cdt->alive = false;
			}
		}
	    
		// advance to the next toy class
		ptr_cdt++;
	}

}


///////////////////////////////////////////////////////////////////////
// Ime: GetToyRect()
// Opisanie: kvadrat za stylknovenie
///////////////////////////////////////////////////////////////////////
void CDeadtoys::GetToyRect( Uint32 toy_index, SDL_Rect *rect )
{

	int frame = (int)CDt[toy_index].frame;

	rect->x = (Uint16)CDt[toy_index].x;
	rect->y = (Uint16)CDt[toy_index].y; 
	rect->w = rect->x + sprite[CDt[toy_index].type][frame]->w;
	rect->h = rect->y + sprite[CDt[toy_index].type][frame]->h;
	
}


///////////////////////////////////////////////////////////////////////
// Ime: GetSafePosition()
// Opisanie: vzemi bezopasna poziciq
///////////////////////////////////////////////////////////////////////
void CDeadtoys::GetSafePosition( Uint16 *x, Uint16 *y, Uint16 range )
{
	Uint16  dx, dy, dist, loopcount = 0;
	bool    cantspawn = true;
	
	do
	{
		cantspawn = false;
		dx = intGetRnd( _game->rScreen.x, _game->rScreen.w - 20);
		dy = intGetRnd( _game->rScreen.y, _game->rScreen.h - 20);

		for ( Uint32 i = 0; i < _game->game_num_cars; i++ )
		{
			dist = GetDistanceNSR( dx, dy, _game->Auto[i].GetX(), _game->Auto[i].GetY() );
			if ( dist < range )
			{
				cantspawn = true;
			}
		}

		
	  if ( loopcount++ > 20 ) break;
	  
	} while( cantspawn );

	// return coords
	*x = dx;
	*y = dy;
}

/////////////////////////////////////Class LandMines///////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
// Ime: CLandMines()
// Opisanie: constructor
///////////////////////////////////////////////////////////////////////
CLandMines::CLandMines()
: _game(NULL)
{


}


///////////////////////////////////////////////////////////////////////
// Ime: ~CLandMines()
// Opisanie: destructor
///////////////////////////////////////////////////////////////////////
CLandMines::~CLandMines()
{

}


///////////////////////////////////////////////////////////////////////
// Ime: Release()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
void CLandMines::Release()
{

	AppendToLog("Closing CLandMines() class...");

	for ( Uint32 i = 0; i < LANDMINE_FRAMES; i++ )
	{
		RELEASE_SURF( sprite[i] );
	}

}



///////////////////////////////////////////////////////////////////////
// Ime: Reset()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
void CLandMines::Reset()
{
	memset( child, NULL, sizeof(CLandMine) * LANDMINE_CHILDS );
}



///////////////////////////////////////////////////////////////////////
// Ime: Initialize()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
int CLandMines::Initialize( CGame *game )
{
	char dirname[255];

	this->_game = game;
	ASSERT( _game != NULL );

	for ( Uint32 i = 0; i < LANDMINE_FRAMES; i++ )
	{
		sprintf( dirname, "gfx/landmine/mine%d.bmp", i );
		if ( ( sprite[i] = _game->Sdl.LoadBitmap( dirname, MAGENTA )) == NULL ) return 0; 
	}

	return 1;
}



///////////////////////////////////////////////////////////////////////
// Ime: Create()
// Opisanie: create a landmine at position
///////////////////////////////////////////////////////////////////////
void CLandMines::Create( Uint16 x, Uint16 y, Uint32 carIndex )
{

	Uint32	  i = 0;
	CLandMine *ptr_lm = child;

	
	while ( i < LANDMINE_CHILDS )
	{
		if ( !ptr_lm->alive )
		{
			ptr_lm->x = x;
			ptr_lm->y = y;
			ptr_lm->frame = 0;
			ptr_lm->Index = carIndex;
			ptr_lm->lifetime = _game->Timer.Time() + LANDMINE_TIMETOSTAY;
			ptr_lm->alive = true;
			break;
		}

		i++;
		ptr_lm++;
	}

}



///////////////////////////////////////////////////////////////////////
// Ime: Update()
// Opisanie: update animations and lifetime check
///////////////////////////////////////////////////////////////////////
void CLandMines::Update()
{
	
	CLandMine *ptr_lm = child;

	for( Uint32 i = 0; i < LANDMINE_CHILDS; i++ )
	{

		if ( ptr_lm->alive )
		{
			// check time
			if ( ptr_lm->lifetime < _game->Timer.Time() )
			{
				ptr_lm->alive = false;
				//KillMine( i );
			}
			else
			{
				//_game->Sdl.Addtoblit( (int)ptr_lm->x, (int)ptr_lm->y, sprite[(int)ptr_lm->frame] );
				_game->Sdl.BlitShadow( (int)ptr_lm->x + 1, (int)ptr_lm->y + 3, sprite[(int)ptr_lm->frame] );
				_game->Sdl.BlitNow( (int)ptr_lm->x, (int)ptr_lm->y, sprite[(int)ptr_lm->frame] );

				ptr_lm->frame += _game->getMpf() * 10; 
				if ( ptr_lm->frame >= LANDMINE_FRAMES )
					ptr_lm->frame = 0;

			}
		}

		ptr_lm++;
	}

}



///////////////////////////////////////////////////////////////////////
// Ime: GetMineRect()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
void CLandMines::GetMineRect( Uint32 lm_index, SDL_Rect *rect )
{
	rect->x = child[lm_index].x;
	rect->y = child[lm_index].y;
	rect->w = rect->x + sprite[(int)child[lm_index].frame]->w;
	rect->h = rect->y + sprite[(int)child[lm_index].frame]->h;
}



///////////////////////////////////////////////////////////////////////
// Ime: KillMine()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
void CLandMines::KillMine( Uint32 lm_index )
{
	child[lm_index].alive = false;
	// do_small_explosion...
	_game->Anims.Create( child[lm_index].x, child[lm_index].y, ANIM_EXPLOSION );
}


