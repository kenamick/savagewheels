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

#define LANDMINE_DAMAGE      	35U
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
	bool		 visible;		// is the car warped/spawned
	bool		 released;		// is class released

	int          max_vel;		// maximum velocity
	float		 vel;			// current velocity
	int          acc, dec_acc;  // acceleration, deceleration
	float		 hit_vel;
	float		 x_acc, y_acc;  // hit - acceleration vector

	int			 velocity_bonus;		// bonus to velocity
	Uint32		 velocity_bonus_time;	// bonus activity time

	int			 lbs;			// weight of the vehicle
	int			 rot_speed;		// rotation speed
//	Uint32		 center_x, center_y;

	float	     x, y;			// current position (top-left)
	float		 center_x, center_y; // current sprite center coordinates
	float		 rep_frame;		// hit impulse direction/sprite animation frame
	float 	     display_frame;	// sprite index to render
	float		 motion_frame;  // movement sprite index
	bool		 reset_frame;
	float		 motion_angle;
	float		 tire_frame;	// offset to moving tires sprite
	Uint16		 max_tire_frames;	// maximum moving tires animation frames

	int			 max_hitpoints;
	int 	     hit_points;
	int			 hit_points_crash;
	bool		 crashed_look;
	int			 frags;			// current (round) kills
	Uint32		 goal_time;		// amount of time this vehicle kept the goal bonus
	bool		 has_the_goal;  // has the goal bonus?

	int			 landmines_count;	// qty. of available landmines
	Uint16		 damage;
	int   		 anger;
	Uint32		 anger_time;

	bool		 bputmine;
	bool		 bputminekey;
	bool		 i_self_destruct;
	bool		 self_destruct;
	bool		 self_destruction;
	Uint32		 destruct_time;
	Uint32		 warning_time;
	Uint32		 honk_status;		// 0 - none, 1 - down, 2 - up

	SDL_Surface  **sprite;			// current (visible) array ptr.
	SDL_Surface  **sprite_norm;		// array of normal (not crashed) sprites
	SDL_Surface  **sprite_crash;	// array of crashed sprites
	Uint32 	     **mask;			// current mask
	Uint32		 **mask_norm;		// mask for normal sprites
	Uint32		 **mask_crash;		// mask for crashed sprites
	SDL_Surface  *driver_name;		// driver sprite

	char		 vehicle_name[8];	// name of vehicle in data file
	//CONST_VEHICLE_TYPE		me;
	CONST_VEHICLE_CONTROL   control;
	CONST_VEHICLE_MOVEMENT  vmove;
	CONST_VEHICLE_ROTATION	vrot;

	CONST_TIRETRAILS		tire_trails;
	Uint32					trails_time;

	// AI
	CONST_AI_VEHICLE_TYPE	avt;
	CWaypoint				waypoint;
	float					ai_cur_angle;		// AI movement angle
	float					ai_dest_angle;		// AI destination angle to reach
	CONST_VEHICLE_ROTATION	ai_turning;			// AI rotation angle
	int						ai_maxvel;
	float					ai_final_frame;
	bool					ai_putmine;
	Uint32					ai_stucktime;		// car-stuck timer
	bool					ai_stuck;

	Uint16					team;				// vehicle Team

	bool					set_stop;

private:
	void DoMotion();
	void Move( CONST_VEHICLE_MOVEMENT mvt );
    void Rotate( CONST_VEHICLE_ROTATION rot );
	
	void AI_Update();
	void AI_GenerateWaypoint();
	void AI_ProcessWaypoint();
	int AI_doFSM( Uint32 *proActions, Uint32 max_actions );
	//void AI_AddWaypoint();

public:
	CVehicle();  
    ~CVehicle() {};
	
	void Repulse( int, float );
	//int	 Initialize( CONST_VEHICLE_TYPE vtype, Uint16 carIndex );
	int	 Initialize( CGame *game, const SWV_HEADER *swv, Uint16 carIndex );
	void Release();
	void Create();
	void GetFrameRect( SDL_Rect *rect );
	void DoDamage( int damageAmount, Uint32 attackerIndex );
	void Update();
	void UpdateStops();
	void AddFrags( int fragnum ) { frags += fragnum; }; 

	SDL_Surface*	GetCurrentFrame();
	Uint32*			GetCurrentFrameMask();
	SDL_Surface*	GetDriverNameSurface() { return driver_name; }; 
	Uint32  GetCX() { return center_x; };
	Uint32  GetCY() { return center_y; };

	// ...
  	//Uint16  GetXT() { return (Uint16)x + g_dirx[(int)display_frame]; };
	//Uint16  GetYT() { return (Uint16)y + g_diry[(int)display_frame]; };  

	bool	GetVisible() { return visible; };
	//Uint32	GetIndex() { return myIndex; };
	Uint16	GetAnger() { return anger; }; // {!}
	bool	GetPossessTheGoal() { return has_the_goal; };
	int		GetFrags() { return frags; };
	float   GetMotionFrame() { return motion_frame; };
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
