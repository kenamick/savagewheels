// CAnimation.h - 

#ifndef __CANIMATION
#define __CANIMATION

////classes//////////////
class CGame;
class CAnimation;
class CAnimations;
/////////////////////////


#define ANIM_MAX_CHILDS		  70
#define ANIM_RATE 			  20
#define MAX_BLOOD_FRAMES	  6
#define MAX_EXPLOSION_FRAMES  20
#define MAX_SPLATS			  5	
#define MAX_TIRESPLATS		  2
#define MIN_SPLATSTAY_TIME    2750
#define MAX_SPLATSTAY_TIME    3750

#define MAX_BGETS			  10 //DT_MAX_DEADTOYS >> 1)

enum CONST_ANIMATIONS {
	ANIM_ANGER,					/* bonus notifiers */
	ANIM_FRAGMASTER,
	ANIM_LANDMINE,
	ANIM_REPAIR,
	ANIM_SPEED,					/*  ...		   	   */
	ANIM_BLOOD,
	ANIM_EXPLOSION,
	ANIM_SPLAT,
	ANIM_TIRESPLAT
};


enum CONST_PLAYERSIDE {
	PS_LEFT,
	PS_RIGHT
};


// child class
class CAnimation 
{

public:

	CAnimation()
		: x(0.0f), y(0.0f), alive(false), frame(0.0f), life(0U)
	{
	};

	~CAnimation()
	{
	};

	float			  x, y;
	bool			  alive;
	float 			  frame;
	Uint32			  life;
	CONST_ANIMATIONS  type;
};


// mother class
class CAnimations 
{

private:
	CGame		 *_game;
	SDL_Surface  *sprite_blood[MAX_BLOOD_FRAMES];
	SDL_Surface  *sprite_explosion[MAX_EXPLOSION_FRAMES];
	SDL_Surface  *sprite_splat[MAX_SPLATS];
	SDL_Surface  *sprite_bmsg[MAX_BGETS];
	CAnimation   CAnim[ANIM_MAX_CHILDS];

public:
	CAnimations();
	~CAnimations();

	int Initialize( CGame *game );
	void Release();
	void Create( Uint16 x, Uint16 y, CONST_ANIMATIONS anim_type );
	void Create( CONST_ANIMATIONS anim_type, CONST_PLAYERSIDE player_side );
	void Update();
	void Reset();

};


#endif
