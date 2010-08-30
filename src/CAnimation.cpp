// CAnimation.cpp  -  class container za animaciite

#include "Main.h"

///////////////////////////////////////////////////////////////////////
// Ime: CAnimations()
// Opisanie: destructor/free animations
///////////////////////////////////////////////////////////////////////
CAnimations::CAnimations()
: _game(NULL)
{

}

///////////////////////////////////////////////////////////////////////
// Ime: ~CAnimations()
// Opisanie: destructor/free animations
///////////////////////////////////////////////////////////////////////
CAnimations::~CAnimations()
{

}


///////////////////////////////////////////////////////////////////////
// Ime: Release()
// Opisanie: osvobodi animaciite
///////////////////////////////////////////////////////////////////////
void CAnimations::Release()
{
	AppendToLog("Closing CAnimations() class...");

	Uint32 i = 0;

	for ( i = 0; i < MAX_EXPLOSION_FRAMES; i++ )
		RELEASE_SURF( sprite_explosion[i] );
	for ( i = 0; i < MAX_BLOOD_FRAMES; i++ )
		RELEASE_SURF( sprite_blood[i] );
	for ( i = 0; i < MAX_SPLATS; i++ )
		RELEASE_SURF( sprite_splat[i] );
	for ( i = 0; i < MAX_BGETS; i++ )
		RELEASE_SURF( sprite_bmsg[i] );
}



///////////////////////////////////////////////////////////////////////
// Ime: Initialize()
// Opisanie: inicializira/zarejda animacii
///////////////////////////////////////////////////////////////////////
int CAnimations::Initialize( CGame *game )
{
	
	char   animname[8];
	char   dirname[32];
	Uint32 i = 0;

	this->_game = game;
	ASSERT( _game != NULL );

	// load bonus-sliders animation
	for ( i = 0; i < MAX_BGETS; i++ )
	{
		sprintf( dirname, "gfx/bget/bget%d.bmp", i );
		if ( (sprite_bmsg[i] = _game->Sdl.LoadBitmap( dirname, MAGENTA )) == NULL ) return 0; 
	}

	// load blood animation
	for ( i = 0; i < MAX_BLOOD_FRAMES; i++ )
	{
		sprintf( dirname, "gfx/anmbld/banim%d.bmp", i );
		if ( (sprite_blood[i] = _game->Sdl.LoadBitmap( dirname, BLACK )) == NULL ) return 0; 
	}

	// load splats animation
	for ( i = 0; i < MAX_SPLATS; i++ )
	{
		sprintf( dirname, "gfx/splats/splat%d.bmp", i );
		if ( (sprite_splat[i] = _game->Sdl.LoadBitmap( dirname, BLACK )) == NULL ) return 0; 
	}

	// load explosion animation
	sprintf( animname, "%s", "anmexp");
	for ( i = 0; i < MAX_EXPLOSION_FRAMES; i++ )
	{
		sprintf( dirname, "gfx/anmexp/exp%d.bmp", i );
		if ( (sprite_explosion[i] = _game->Sdl.LoadBitmap( dirname, BLACK )) == NULL ) return 0; 
	}

	return 1;
}


///////////////////////////////////////////////////////////////////////
// Ime: Create()
// Opisanie: syzdava dead_toya na ekrana
///////////////////////////////////////////////////////////////////////
void CAnimations::Create( Uint16 pos_x, Uint16 pos_y, CONST_ANIMATIONS anim_type )
{
	
	Uint32		i			= 0, 
				uc			= 0;
	CAnimation  *ptr_anm	= this->CAnim;

	if ( anim_type == ANIM_TIRESPLAT )
		uc = ANIM_MAX_CHILDS - 10;
	else
		uc = ANIM_MAX_CHILDS;

	while( i < uc )
	{
		if ( !ptr_anm->alive  )
		{
			ptr_anm->x = (float)pos_x;
			ptr_anm->y = (float)pos_y;
			ptr_anm->type = anim_type;
			ptr_anm->frame = 0;
			ptr_anm->alive = true;
			
			// special splat_paddle case
			switch ( anim_type )
			{
			
			case ANIM_SPLAT:
				ptr_anm->frame = (float)intGetRnd( 0, MAX_SPLATS - MAX_TIRESPLATS ); //(rand() % MAX_SPLATS);
				ptr_anm->life = _game->Timer.Time() + 5000;
			break;

			case ANIM_TIRESPLAT:
				ptr_anm->frame = (float)intGetRnd( MAX_SPLATS - MAX_TIRESPLATS, MAX_SPLATS );
				ptr_anm->life = _game->Timer.Time() + intGetRnd( MIN_SPLATSTAY_TIME, MAX_SPLATSTAY_TIME );
			break;

			/*else if ( anim_type == ANIM_TIRETRAILS )
			{
				ptr_anm->frame = 0; //intGetRnd(  );
				//ptr_anm->life = _game->Timer.Time() + intGetRnd( MIN_SPLATSTAY_TIME, MAX_SPLATSTAY_TIME );
			}*/
			}
			
			break;
		}
		
		i++;
		ptr_anm++;
	}

}



///////////////////////////////////////////////////////////////////////
// Ime: Create()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
void CAnimations::Create( enum CONST_ANIMATIONS anim_type, CONST_PLAYERSIDE player_side )
{

	Uint32 i = 0;
	CAnimation *ptr_anm = this->CAnim;

	while( i < ANIM_MAX_CHILDS )
	{

		if ( !ptr_anm->alive  )
		{
			if ( player_side == PS_LEFT )
			{
				ptr_anm->x = -160.0;
				ptr_anm->y = 5.0;//425.0;
				ptr_anm->type = anim_type;
				ptr_anm->frame = (float)((int)anim_type);
				ptr_anm->alive = true;
				ptr_anm->life = _game->Timer.Time() + 1600;
			}
			else
			{
				ptr_anm->x = 800.0;
				ptr_anm->y = 5.0;
				ptr_anm->type = anim_type;
				ptr_anm->frame = (float)((int)anim_type + 5);
				ptr_anm->alive = true;
				ptr_anm->life = _game->Timer.Time() + 1600;
			}

			break;
		}
		
		i++;
		ptr_anm++;
	}

}



///////////////////////////////////////////////////////////////////////
// Ime: Update()
// Opisanie: 
///////////////////////////////////////////////////////////////////////
void CAnimations::Update()
{
	Uint32		i			= 0;
	Uint32		cur_ticks	= _game->Timer.Time();
	CAnimation	*ptr_anm	= this->CAnim;
	//SDL_Rect	rdest;
	
	for( i = 0; i < ANIM_MAX_CHILDS; i++ )
	{
		if ( ptr_anm->alive )
		{
			switch( ptr_anm->type )
			{
				case ANIM_BLOOD:
				  if ( ptr_anm->frame >= MAX_BLOOD_FRAMES ) 
					ptr_anm->alive = false;
				  else
	   				_game->Sdl.Addtoblit( (int)ptr_anm->x-32, (int)ptr_anm->y-32, sprite_blood[(int)ptr_anm->frame] );
				  ptr_anm->frame += ANIM_RATE*_game->getMpf();			// increment anim frame
			    break;

				case ANIM_EXPLOSION:
				  if ( ptr_anm->frame >= MAX_EXPLOSION_FRAMES ) 
					ptr_anm->alive = false;
				  else
				  {
					//_game->Sdl.Addtoblit( (int)ptr_anm->x-40, (int)ptr_anm->y-30, sprite_explosion[(int)ptr_anm->frame] );
				    //_game->Sdl.Addtoblit( (int)ptr_anm->x-40, (int)ptr_anm->y-30, sprite_explosion[(int)ptr_anm->frame] );
					//_game->Sdl.BlitShadow( (int)ptr_anm->x-40+8, (int)ptr_anm->y-30+24, sprite_explosion[(int)ptr_anm->frame] );
					_game->Sdl.Addtoblit( (int)ptr_anm->x-50, (int)ptr_anm->y-50, sprite_explosion[(int)ptr_anm->frame] );
				  }
				  
				 ptr_anm->frame += ANIM_RATE*_game->getMpf();
				break;

				case ANIM_SPLAT:
				case ANIM_TIRESPLAT:
				  if ( ptr_anm->life < _game->Timer.Time() ) 
					ptr_anm->alive = false;
				  else
				    _game->Sdl.BlitNow( (int)ptr_anm->x-16, (int)ptr_anm->y-16, sprite_splat[(int)ptr_anm->frame] );
				break;

				/*case ANIM_TIRETRAILS:
					rdest.x = (int)ptr_anm->x-4;
					rdest.y = (int)ptr_anm->y-4;
					SDL_BlitSurface( sprite_tire[(int)ptr_anm->frame], NULL, back[cur_map], &rdest );
				*/
				
				case ANIM_ANGER:
				case ANIM_FRAGMASTER:
				case ANIM_LANDMINE:
				case ANIM_REPAIR:
				case ANIM_SPEED:
					
				  if ( ptr_anm->life < _game->Timer.Time() ) 
					ptr_anm->alive = false;
				  else
				  {
					  // do slide
					  if ( ptr_anm->frame > 4 )
					  {
						  ptr_anm->x -= _game->getMpf() * 600;
						  if ( ptr_anm->x < 490 ) ptr_anm->x = 490;
					  }
					  else
					  {
						  ptr_anm->x += _game->getMpf() * 600;
						  if ( ptr_anm->x > 0 ) ptr_anm->x = 0;
					  }

   					  _game->Sdl.BlitNow( (int)ptr_anm->x, (int)ptr_anm->y, sprite_bmsg[(int)ptr_anm->frame] );
				  }
				break;
			}
		
		}
	    
		// advance to the next animation class
		ptr_anm++;
	}

}



///////////////////////////////////////////////////////////////////////
// Ime: Reset()
// Opisanie: reset vars
///////////////////////////////////////////////////////////////////////
void CAnimations::Reset()
{
	memset( CAnim, 0, ANIM_MAX_CHILDS * sizeof(CAnimation) );
}


