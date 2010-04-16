// CDeadToy.h - 



#ifndef __CDEADTOY
#define __CDEADTOY

////classes//////////////
class CGame;
class CDeadtoy;
class CDeadtoys;
class CLandMine;
class CLandMines;
/////////////////////////

#define DT_MAX_DEADTOYS       5
#define DT_MAX_CHILDS	      15		
#define DT_TOYSPAWNRANGE      400 //20pixels
#define LANDMINE_FRAMES       2
#define LANDMINE_CHILDS       24
#define LANDMINE_TIMETOSTAY   12000


typedef enum CONST_DEADTOYS {
	DT_BEARGOAL = 0,
	DT_BEARANGER,
	DT_BEARHP,
	DT_BEARSPEED,
	DT_BEARLANDMINE
};


typedef enum CONST_DEADTOYGOAL {
	DTG_ONTHEROAD = 0,
	DTG_TAKEN
};


// child class
class CDeadtoy
{

public:

	CDeadtoy()
		: x(0), y(0), value(0U), life(0U), alive(false), frame(0.0f), max_frames(0U)
	{
	};

	~CDeadtoy()
	{
	};

	float			x,y;
	Uint16			value;
	Uint32			life;
	bool			alive;
	CONST_DEADTOYS  type;

	float 			frame;			 // anim vars
	Uint16			max_frames;

};


// mother class
class CDeadtoys 
{

private:

	CGame				*_game;
	SDL_Surface			*sprite[DT_MAX_DEADTOYS][2]; //***sprite;  
	CDeadtoy			CDt[DT_MAX_CHILDS];
	CONST_DEADTOYGOAL	goal_state;
	Uint32				goal_index;

public:
	CDeadtoys();
	~CDeadtoys();

	int	 Initialize( CGame *pGame );
	void Release();
	void Reset();
	void Create( Uint16 x, Uint16 y, CONST_DEADTOYS toy_type );
	void Update();
	void GetSafePosition( Uint16 *x, Uint16 *y, Uint16 range );

	void   KillToy( Uint32 toy_index ) { CDt[toy_index].alive = false; };
	void   GetToyRect( Uint32 toy_index, SDL_Rect *rect );
	int    GetToyVisible( Uint32 toy_index ) { return CDt[toy_index].alive; };
	Uint16 GetToyValue( Uint32 toy_index ) { return CDt[toy_index].value; };
	Uint32 GetToyGoal() { return goal_index; };

	CONST_DEADTOYGOAL GetToyGoalState() { return goal_state; };
	CONST_DEADTOYS    GetToyKind( Uint32 toy_index ) { return CDt[toy_index].type; };

	void SetToyGoalState( CONST_DEADTOYGOAL goalstate ) { goal_state = goalstate; };
	

};



// landmine_child_class

class CLandMine {

public:

	CLandMine()
		: x(0), y(0), Index(-1), lifetime(0), frame(0.0f), alive(false)
	{
	};

	~CLandMine()
	{
	};

	Uint16 x, y;
	Uint32 Index;    // index of the auto that left the mine
	Uint32 lifetime;
	float  frame;
	bool   alive;

};


// landmine_mother_class
class CLandMines {

private:

	CGame		 *_game;
	SDL_Surface  *sprite[LANDMINE_FRAMES];
	CLandMine    child[LANDMINE_CHILDS];

public:	
	CLandMines();
	~CLandMines();

	int     Initialize( CGame *pGame );
	void	Release();
	void	Reset();
	void    Create( Uint16 x, Uint16 y, Uint32 carIndex );
	void    Update();

	void    KillMine( Uint32 lm_index );
	void    GetMineRect( Uint32 lm_index, SDL_Rect *rect );
	bool    GetMineVisible( Uint32 lm_index ) { return child[lm_index].alive; };
	Uint32  GetMineIndex( Uint32 lm_index ) { return child[lm_index].Index; };
	SDL_Surface*  GetMineCurrentFrame( Uint32 lm_index ) { return sprite[(int)child[lm_index].frame]; };

};



#endif