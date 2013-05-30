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

#ifndef __CVEHICLE
#define __CVEHICLE

////classes//////////////
class CGame;
class CWaypoint;
class CVehicle;
/////////////////////////


#define MAX_ROTATION_FRAMES		36
#define HALF_ROTATION_FRAMES    18
#define TIRETRAILS_TIME			3500
#define ANGEREXPIRE_TIME		180
#define SPEEDEXPIRE_TIME		8000
#define GOALEXPIRE_TIME			5000
#define NO_ATTACKER				10

#define LANDMINE_DAMAGE			35U
#define MIN_DAMAGE_VELOCITY		45
#define SAFE_WARP_DISTANCE		6400 // 80 pixels
#define WAYPOINT_RADIUS			900  //30pixels
#define NO_WAYPOINT			    -1
#define WAYPOINT_VEHICLE		1
#define WAYPOINT_BONUS  		2
#define ACTIONS					5
#define ACTION_ATTACK			1
#define ACTION_TAKEBONUS		2
#define ACTION_DROPMINE			3

// game difficulty constants
enum CONST_DIFFICULTY
{
	DIFF_EASY = 6,
	DIFF_NORMAL = 7,
	DIFF_HARD = 8
};


// enum CONST_VEHICLE_TYPE 
// {
//   VT_KAMION = 0,
//   VT_FERRARI,
//   VT_MERCEDES,
//   VT_BUGGY
// };


enum CONST_VEHICLE_MOVEMENT 
{
  VM_NONE = 0,
  VM_FORWARD,
  VM_BACKWARD
};


enum CONST_VEHICLE_ROTATION 
{
  VR_NONE = 0,
  VR_LEFT,
  VR_RIGHT
};


enum CONST_VEHICLE_CONTROL 
{
  VC_PLAYER1 = 0,
  VC_PLAYER2,
  //VC_PLAYER3,
  VC_AI,
  VC_NONE
};


enum CONST_TIRETRAILS 
{
  VTT_NONE,
  VTT_TIRES,
  VTT_BLOOD
};


enum CONST_AI_VEHICLE_TYPE 
{
	AVT_WARRIOR,
	AVT_EXPLORER
};



class CWaypoint 
{
public:
	CWaypoint()
		: x(0), y(0),
		  index(0U), goal_type(0U),
		  static_pos(false),
		  reached(false),
		  do_precalculate(false),
		  do_reverse(false),
		  do_reverseTime(0)
	{};

	~CWaypoint() {};

	float	x,y;
	Uint32	index;
	Uint32	goal_type;
	bool	static_pos;		// is target moving?
	bool	reached;		// flag -> target reached?
	bool	do_precalculate;// if vehicle is off target calculate target coordinates again
	bool	do_reverse;		// reverse movement (for given time) if vehicle has hit another vehicle
	Sint32	do_reverseTime;
	//CONST_DEADTOYS  toy_kind;
};


class CVehicle
{
private:
	CGame		 *_game;

	Uint32		 myIndex;		// class index
	bool		 visible;		// is the car warped and ready
	bool		 released;		// is class released

	//int          vel,max_vel;	// skorost, maksimalna
	int          max_vel;	// skorost, maksimalna
	float		 vel;
	int          acc, dec_acc;  // uskorenie i dec_uskorenie na spirane
//	float		 hit_vel;

	int			 speed_bonus;   // bonus-kym skorostta
	Uint32		 speed_time;	// vreme na bonus-skorost

	int			 lbs;			// kolko teji 
	int			 rot_speed;		// rotation speed
	SDL_Rect     rCollide;		// kvadrat na zasichane
	Uint32		 center_x, center_y;
	//SDL_Rect	 rFrame;
	float	     x,y;			// poziciq
	float		 x_acc, y_acc;  // ugly na uskorenie pri udar
	float		 rep_frame;		// kadyr na udrqshtiqt ni avtomobil
	float 	     display_frame;	// kadyr za rendirane
	float		 motion_frame;  // kadyr za ygyla na dvijenie
	float		 tire_frame;	// kadyr na dvijenie na gumite
	Uint16		 tire_frames;	 // kadri za vyrtqshti se gumi

	int			 max_hitpoints;
	int 	     hit_points;
	int			 hit_points_crash;
	bool		 bcrashlook;
	int			 frags;			// kills
	Uint32		 goal_time;		// vreme za koeto zadryjame goal-a +3 frags 
	bool		 has_the_goal;  // v tazi mashina li e goal-a

	int			 landmines;		// kolko mini ima v nalichnost...
	Uint16		 damage;
	int   		 anger;
	Uint32		 anger_time;
	bool		 bputmine, bputminekey;

	bool		 i_self_destruct;
	bool		 self_destruct;
	bool		 self_destruction;
	Uint32		 destruct_time;
	Uint32		 warning_time;
	Uint32		 honk_status;		// 0 - none, 1 - down, 2 - up

	SDL_Surface  **sprite;			// current image-pointer
	SDL_Surface  **sprite_norm; 
	SDL_Surface  **sprite_crash;
	Uint32 	     **mask, **mask_norm, **mask_crash;
	SDL_Surface  *driver_name;
	//SDL_Surface	 ***sprite_norm; //*sprite_norm[36];  // car faces
	//SDL_Surface  ***sprite_crash; //*sprite_crash[36]; // car crashed-faces
	
	//CONST_VEHICLE_TYPE		me;
	CONST_VEHICLE_CONTROL   control;
	CONST_VEHICLE_MOVEMENT  vmove;
	CONST_VEHICLE_ROTATION	vrot;

	CONST_TIRETRAILS		tire_trails;
	Uint32					trails_time;

	// AI
	CONST_AI_VEHICLE_TYPE	avt;
	CWaypoint				waypoint;
	float					ai_cur_angle;
	float					ai_dest_angle;
	CONST_VEHICLE_ROTATION	ai_turning;				// poskoa na zavivane na avtomobila
	int						ai_maxvel;
	float					ai_final_frame;
	bool					ai_putmine;
	Uint32					ai_stucktime;			// car-stuck timer
	bool					ai_stuck;

	Uint16					team;					// vehicle Team

private:
	void DoMotion();
	void Move( CONST_VEHICLE_MOVEMENT mvt );
    void Rotate( CONST_VEHICLE_ROTATION rot );
	
	void   AI_Update();
	void   AI_GenerateWaypoint();
	void   AI_ProcessWaypoint();
	Uint16 AI_doFSM( Uint16 *proActions, Uint16 max_actions );
	//void AI_AddWaypoint();

public:
	CVehicle();  
    ~CVehicle();
	
//	void Repulse( int, float );
	//int	 Initialize( CONST_VEHICLE_TYPE vtype, Uint16 carIndex );
	bool Initialize( CGame *game, const SWV_HEADER *swv, Uint16 carIndex );
	void Release();
	void Create();
	void DoDamage( int damageAmount, Uint32 attackerIndex );
	void Update();
	void AddFrags( int fragnum ) { frags += fragnum; }; 

	SDL_Surface*	GetCurrentFrame();
	Uint32*			GetCurrentFrameMask();
	void 			GetFrameRect( SDL_Rect *rect );
	SDL_Surface*	GetDriverNameSurface() { return driver_name; };
	float			GetDirectionAngle();
	void			SetDirectionAngle(float rad);

	Uint32  GetX() { return center_x; };
	void	SetX(float newX) { x = newX; };
	Uint32  GetY() { return center_y; };
	void	SetY(float newY) { y = newY; };

	bool	GetVisible() { return visible; };
	//Uint32	GetIndex() { return myIndex; };
	Uint16	GetAnger() { return anger; }; // {!}
	bool	GetPossessTheGoal() { return has_the_goal; };
	int		GetFrags() { return frags; };
	float   GetMotionFrame();
	float   GetMotionFrameMirror();
	int	    GetCompareVal() { return lbs; }; //abs(vel); };
	float   GetVelocity() { return vel; };
	void    SetVelocity( float setvel ) { vel = setvel; };
	int		GetHitPoints() { return max_hitpoints; };
	CONST_VEHICLE_CONTROL GetControl() { return control; };
	Uint16	GetTeam() { return team; };
	
	void    SetControl( CONST_VEHICLE_CONTROL vcontrol );
	void    SetAttirbs( CONST_DIFFICULTY diff );
	void    SetTeam( Uint16 team_index ) { team = team_index; };
};

#endif
