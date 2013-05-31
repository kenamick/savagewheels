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

/*	 
 frags	     x:105 y:430  x:265 y:430  	x:425 y:430   x:585 y:430
 hitpoints - x:5 y:445    x:165 y:445   x:325 y:445   x:485 y:445 
 anger 	     x:5 y:460    x:165 y:460   x:325 y:460   x:485 y:460 
*/

static POINT  pos_frag[4]	= { 105, 428, 265, 428, 425, 428, 585, 428 };
static POINT  pos_hp[4]	= { 5, 445, 165, 445, 325, 445, 485, 445 };
static POINT  pos_anger[4] = { 5, 460, 165, 460, 325, 460, 485, 460 };
static POINT  pos_warp[4]	= { 30, 30, 30, 360, 535, 30, 535, 360 };

/*
 * Vehicle components (Extern)
 */
int attribACC[3]		= { 160, 220, 320 };
int	attribSPEED[5]	= { 180, 220, 260, 310, 340 };
int attribROT[3]		= { 15, 20, 25 };
int attribDAMAGE[4]	= { 40, 60, 70, 100 };
int attribARMOUR[4]	= { 210, 260, 300, 400 };

/*
 * 36 precalculated sin/cos values for each vehicle animation frame
 *
 * done by LordCIH using Matlab ;]
 */
static float g_dirx[] = { 1.0000f, 0.9848f, 0.9397f, 0.8660f, 0.7660f, 0.6428f,
		0.5000f, 0.3420f, 0.1736f, 0.0000f, -0.1736f, -0.3420f, -0.5000f,
		-0.6428f, -0.7660f, -0.8660f, -0.9397f, -0.9848f, -1.0000f, -0.9848f,
		-0.9397f, -0.8660f, -0.7660f, -0.6428f, -0.5000f, -0.3420f, -0.1736f,
		0.0000f, 0.1736f, 0.3420f, 0.5000f, 0.6428f, 0.7660f, 0.8660f, 0.9397f,
		0.9848f };

static float g_diry[] = { 0.0000f, 0.1736f, 0.3420f, 0.5000f, 0.6428f, 0.7660f,
		0.8660f, 0.9397f, 0.9848f, 1.0000f, 0.9848f, 0.9397f, 0.8660f, 0.7660f,
		0.6428f, 0.5000f, 0.3420f, 0.1736f, 0.0000f, -0.1736f, -0.3420f,
		-0.5000f, -0.6428f, -0.7660f, -0.8660f, -0.9397f, -0.9848f, -1.0000f,
		-0.9848f, -0.9397f, -0.8660f, -0.7660f, -0.6428f, -0.5000f, -0.3420f,
		-0.1736f };

///////////////////////////////////////////////////////////////////////
// Name: CVehicle()
// Desc: konstructor
///////////////////////////////////////////////////////////////////////
CVehicle::CVehicle()
: _game( NULL ),
visible(false),
released(true),
honk_status(0),
reset_frame(false),
skip_hit_timer(0U)
{
	driver_name = (SDL_Surface *)NULL;
	sprite = sprite_norm = sprite_crash = (SDL_Surface **)NULL;
	mask = mask_crash = mask_norm = NULL;
}


///////////////////////////////////////////////////////////////////////
// Name: ~CVehicle()
// Desc: destructor
///////////////////////////////////////////////////////////////////////
CVehicle::~CVehicle()
{

}

///////////////////////////////////////////////////////////////////////
// Name: Release()
// Desc: osvobodi sprite-ovete
///////////////////////////////////////////////////////////////////////
void CVehicle::Release()
{

	if ( released ) 
		return;

	AppendToLog( "Closing CVehicle() class..." );

	for ( int cn = 0; cn < MAX_ROTATION_FRAMES * tire_frames; cn++ )
	{
		RELEASE_SURF( sprite_norm[cn] );
		RELEASE_SURF( sprite_crash[cn] );

		if ( mask_norm[cn] )
			delete mask_norm[cn];

		if ( mask_crash[cn] )
			delete mask_crash[cn];
	}

	RELEASE_SURF( driver_name );

	if ( sprite_norm )
		delete[] sprite_norm;

	if ( sprite_crash )
		delete[] sprite_crash;

	if ( mask_norm )
		delete[] mask_norm;

	if ( mask_crash )
		delete[] mask_crash;

	released = true;
}


///////////////////////////////////////////////////////////////////////
// Name: Initialize()
// Desc: create vehicle from module
///////////////////////////////////////////////////////////////////////
bool CVehicle::Initialize( CGame *game, const SWV_HEADER *swv, Uint16 carIndex )
{

	int cn = 0;
	int frames = 0;
	int index = 0;

	this->_game = game;
	ASSERT(_game != NULL);

	LOG("Loading vehicle " << swv->vehiclename << "...");

	myIndex = carIndex;
	frames = swv->animation_frames;

	// get tire frames
	tire_frames = frames / MAX_ROTATION_FRAMES;
	frags = 0; // reset fragovete
	anger = 0; // reset anger-a

	// allocate mem for images
	sprite_norm = new SDL_Surface*[frames];
	sprite_crash = new SDL_Surface*[frames];
	mask_norm = new Uint32*[frames];
	mask_crash = new Uint32*[frames];

	char *vehicle_filename = const_cast<char *>(swv->filename);

	for (cn = 0; cn < frames; cn++) {
		// normal sprites
		index = cn + 3;

		if ((sprite_norm[cn] = _game->Sdl.LoadBitmap(vehicle_filename,
				swv->pfiles[index].pos, swv->pfiles[index].length, MAGENTA,
				NO_ALPHA)) == NULL) {
			return false;
		}

		// create a mask for this sprite
		_game->Sdl.MakeBoolMask(sprite_norm[cn], mask_norm[cn]);

		// crashed sprites (calc.offset to the crashed sprites)
		index = cn + 3 + frames;

		if ((sprite_crash[cn] = _game->Sdl.LoadBitmap(vehicle_filename,
				swv->pfiles[index].pos, swv->pfiles[index].length, MAGENTA,
				NO_ALPHA)) == NULL) {
			return false;
		}

		// create a mask for this sprite
		_game->Sdl.MakeBoolMask(sprite_crash[cn], mask_crash[cn]);
	}

	// load driver_name surface
	if ((driver_name = _game->Sdl.LoadBitmap(vehicle_filename,
			swv->pfiles[2].pos, swv->pfiles[2].length, BLACK, NO_ALPHA))
			== NULL) {
		return false;
	}

	display_frame = tire_frame = 0;

	// fill attribs
	acc = attribACC[(int) swv->acc];
	dec_acc = acc - swv->dec_acc;
	max_hitpoints = attribARMOUR[(int) swv->hp];
	hit_points_crash = max_hitpoints / swv->hp_crash;
	lbs = swv->lbs;
	max_vel = attribSPEED[(int) swv->max_vel];
	rot_speed = attribROT[(int) swv->rot_speed];
	damage = attribDAMAGE[(int) swv->damage];

	// set default-team
	team = carIndex;

	released = false;

	sprintf(carname, "%s", swv->vehiclename);

	return true;
}


///////////////////////////////////////////////////////////////////////
// Name: SetAttirbs()
// Desc: car-attribs based on game difficulty
///////////////////////////////////////////////////////////////////////
void CVehicle::SetAttirbs( CONST_DIFFICULTY diff )
{
	float diff_perc;

	switch( diff )
	{
	case DIFF_EASY:
		diff_perc	= 0.75f;
		break;

	case DIFF_NORMAL:
		diff_perc = 1.0f;
		return; 
		break;

	case DIFF_HARD:
		diff_perc	= 1.3f;
		break;
	
	default: 
		LOG("Invalid difficulty attribute!");
		return;
	}
	
	// change attribs
	acc				= (int)( (float)acc * diff_perc);
	dec_acc			= (int)( (float)dec_acc * diff_perc);
	max_hitpoints	= (int)( (float)max_hitpoints * diff_perc);
	hit_points_crash = max_hitpoints / 2;
	max_vel			 = (int)( (float)max_vel * diff_perc );
	damage			 = (int)( (float)damage * diff_perc );
}


/*///////////////////////////////////////////////////////////////////////
// Name: Initialize()
// Desc: inicializira avtomobil
///////////////////////////////////////////////////////////////////////
int CVehicle::Initialize( CONST_VEHICLE_TYPE vtype, Uint16 carIndex )
{

 int   cn;
 char  cardir[255];
 char  carname[16];
 
 myIndex = carIndex;
 me = vtype;
 frags = 0;

 switch( me )
 {
      case VT_KAMION:
		sprintf ( carname, "%s", "v4" );
	    tire_frames = 1;
	  break;   
   
	  case VT_FERRARI:
		sprintf ( carname, "%s", "fera" );
   	    tire_frames = 1;
	  break;
	  
	  case VT_MERCEDES:
		sprintf ( carname, "%s", "merc" );
		tire_frames = 1;
   	  break;

      case VT_BUGGY:
   	    sprintf ( carname, "%s", "bugi" );
		tire_frames = 2;
	  break;

	  default: return 0;
 }

 // ...
 sprite_norm = (SDL_Surface **) new SDL_Surface[MAX_ROTATION_FRAMES * tire_frames];
 sprite_crash = (SDL_Surface **) new SDL_Surface[MAX_ROTATION_FRAMES * tire_frames];



 // load animation 
 for( cn = 0; cn < MAX_ROTATION_FRAMES * tire_frames; cn++ )
 {
		 // normal 
		 sprintf ( cardir, "gfx/%s/ok/%d.bmp", carname, cn );
		 if ( (sprite_norm[cn] = _game->Sdl.LoadBitmap( cardir, MAGENTA )) == NULL ) return 0;
		 
		 // crashed
		 sprintf ( cardir, "gfx/%s/crash/%d.bmp", carname, cn+1 );
   		 if ( (sprite_crash[cn] = _game->Sdl.LoadBitmap( cardir, 0U )) == NULL ) return 0;
 }

 display_frame = tire_frame = 0;

 return 1;
}
*/

///////////////////////////////////////////////////////////////////////
// Name: Initialize()
// Desc: inicializira avtomobil
///////////////////////////////////////////////////////////////////////
void CVehicle::Create()
{
	Uint32 		dist = 0UL;
	CVehicle 	*ptr_veh = NULL;
	bool 		do_not_warp = true;
	int 		warpPos = 0;

	// get a safe vehicle spawn point
	// j - one of 4 possible warp points

	DBG("Creating vehicle idx=" << myIndex);

	while (do_not_warp)
	{
		warpPos = (int) (rand() % 4);
		do_not_warp = false;

		ptr_veh = _game->Auto;

		for (Uint16 i = 0; i < _game->game_num_cars; i++)
		{
			if (i != myIndex)
			{
				if (ptr_veh->GetVisible())
				{
					dist = GetDistanceNSR(ptr_veh->GetX(),
							ptr_veh->GetY(),
							pos_warp[warpPos].x,
							pos_warp[warpPos].y);

					if (dist < SAFE_WARP_DISTANCE) {
						do_not_warp = true;
						break;
					}
				}
			}

			ptr_veh++;
		}

		//if ( !do_not_warp ) break;
	}

	//if ( do_not_warp ) return;

	x_acc = 0.0f;
	y_acc = 0.0f;
//	hit_vel = 0.0f;
	landmines = 0;
	vmove = VM_NONE;
	vrot = VR_NONE;

	// point the current animations to the array of non-crashed vehicle animations
	sprite = sprite_norm;
	mask = mask_norm;
	bcrashlook = false;

	// reset vars
	x = pos_warp[warpPos].x;
	y = pos_warp[warpPos].y;
	speed_bonus = 0;
 
	if ( warpPos == 2 || warpPos == 3 )
	{
//		motion_frame = 18;
//		display_frame = motion_frame;
		motion_angle = PI;
		display_frame = 18;
	}
	else
	{
//		motion_frame = 0;
		motion_angle = 0.0f;
		display_frame = 0;
	}

//	center_x = (Uint32)x + (sprite[(int)motion_frame]->w / 2);
//	center_y = (Uint32)y + (sprite[(int)motion_frame]->h / 2);

	// check for memory access violation
	ASSERT(sprite_norm[(int)display_frame]->w < 500);
	ASSERT(sprite_norm[(int)display_frame]->h < 500);

	tire_frame = 0;
	vel = 0.0f;
	tire_trails = VTT_NONE;
	trails_time = 0;

	// ai_facing
	ai_cur_angle = motion_angle;
	ai_stuck = false;

	visible = true;
	self_destruct = self_destruction = false;
	warning_time = 0;
	
	hit_points = max_hitpoints;

	 // TEMP
	 /*acc = 160;
	 max_vel = 180;
	 dec_acc = acc - 50;
	 rot_speed = 15;
	 lbs = 1;
	 damage = 3;
	 max_hitpoints = 100;
	 hit_points = max_hitpoints;
	 hit_points_crash = 50;
	*/
}


///////////////////////////////////////////////////////////////////////
// Name: SetControl()
// Desc: Set Vehicle control operator
///////////////////////////////////////////////////////////////////////
void CVehicle::SetControl( CONST_VEHICLE_CONTROL vcontrol )
{
	control = vcontrol;
	
	// AI_setup
	waypoint.reached = true;
	if ( control == VC_AI )
	{
	
		if ( intGetRnd( 0, 10 ) < 5 )
		{
			avt = AVT_WARRIOR;
			//AppendToLog( "Create AI class : WARRIOR ");
		}
		else
		{
			avt = AVT_EXPLORER;
			//AppendToLog( "Created AI class : EXPLORER ");
		}
	}
}


///////////////////////////////////////////////////////////////////////
// Name: Move()
// Desc: Mradni napred/nazad
///////////////////////////////////////////////////////////////////////
void CVehicle::Move( CONST_VEHICLE_MOVEMENT mvt )
{
	vmove = mvt;
//	motion_frame = display_frame;

	if (reset_frame) {
		motion_angle = Deg2Rad(display_frame * 10.0f);
		motion_angle = FixRad(motion_angle);
	}
}


///////////////////////////////////////////////////////////////////////
// Name: Rotate()
// Desc: Varti kolata
///////////////////////////////////////////////////////////////////////
void CVehicle::Rotate( CONST_VEHICLE_ROTATION rot )
{
	vrot = rot;
//	motion_frame = display_frame;
	if (reset_frame) {
		motion_angle = Deg2Rad(display_frame * 10.0f);
		motion_angle = FixRad(motion_angle);
	}
}

 

///////////////////////////////////////////////////////////////////////
// Name: DoMotion()
// Desc: do vehicle motion
///////////////////////////////////////////////////////////////////////
void CVehicle::DoMotion()
{
	// Init defaults
	float tmp_df 		= display_frame;
//	float tmp_mf		= motion_frame;
	float tmp_x			= x;
	float tmp_y			= y;
	float tmp_maxvel	= (float)max_vel;
	float tmp_vel 		= vel;
	float abs_vel		= fabsf(vel);

	float rot_m 		= 1.0f;

	/*
	 * AI-steering
	 */

	if ( control == VC_AI )
	{
		rot_m = 2.0f;  // double rotation speed for AI controlled vehicles

		if ( ai_final_frame > display_frame ) 
			vrot = VR_LEFT;
		else if ( ai_final_frame < display_frame ) 
			vrot = VR_RIGHT;
		else
			vrot = VR_NONE;

		if ( ai_turning == VR_LEFT )
		{
			ai_cur_angle += ((rot_speed/2) * _game->getMpf());
			if ( ai_cur_angle >= ai_dest_angle )
			{
				ai_cur_angle = ai_dest_angle;
				ai_turning = VR_NONE;
			}

			//vrot = VR_LEFT;
		}
		else if ( ai_turning == VR_RIGHT )
		{
			ai_cur_angle -= ((rot_speed/2) * _game->getMpf());
			if ( ai_cur_angle <= ai_dest_angle )
			{
				ai_cur_angle = ai_dest_angle;
				ai_turning = VR_NONE;
			}
		}

	}

	/*
	 * Rotation
	 */
	
	if ( vrot == VR_LEFT && vel != 0 )
    {
		float step = (rot_speed * rot_m * _game->getMpf());
	    display_frame += step;
		if ( display_frame > MAX_ROTATION_FRAMES - 1 )
			display_frame = 0;

		// AI case
		if ( control == VC_AI && display_frame > ai_final_frame )
			display_frame = ai_final_frame;

	    motion_angle += (rot_speed * (2*PI / 36.0f)* _game->getMpf());
//	    motion_angle += PI;
	    motion_angle = FixRad(motion_angle);
	}
	else if ( vrot == VR_RIGHT && vel != 0 )
	{
		float step = (rot_speed * rot_m * _game->getMpf());
	    display_frame -= step;

		if ( display_frame < 0 )
			display_frame = MAX_ROTATION_FRAMES - 1;

		// AI case
		if ( control == VC_AI && display_frame < ai_final_frame )
			display_frame = ai_final_frame;

		motion_angle -= (rot_speed * (2*PI / 36.0f) * _game->getMpf());
//		motion_angle += PI;
		motion_angle = FixRad(motion_angle);
	}

	vrot = VR_NONE;
//	motion_frame = display_frame;

	/*
	 * Acceleration
	 */

	float fmaxvel_p = tmp_maxvel + speed_bonus;
	float fmaxvel_n = -(tmp_maxvel + speed_bonus);// / 2;

	if ( vmove == VM_FORWARD )
	{
		vel += (float)acc * _game->getMpf();
		if ( vel > fmaxvel_p )
			vel = fmaxvel_p;
	}
	else if ( vmove == VM_BACKWARD )
	{
		vel -= (float)acc * _game->getMpf();
		if ( vel < fmaxvel_n )
			vel = fmaxvel_n;
	}
	else
	{
		// (Newton's First law)
		// Speed always closes 0 due to friction defined by 'dec_acc'
		if ( vel > 0.0f ) 
		{
			vel -= (float)dec_acc * _game->getMpf();
			if ( vel < 0.0f )
				vel = 0.0f;
		}

		if ( vel < 0.0f ) 
		{
			vel += (float)dec_acc * _game->getMpf();
			if ( vel > 0.0f )
				vel = 0.0f;
		}
	}

	// Rotate vehicle tires (if moving)
	if (tire_frames > 1 && abs_vel > 0.0f)
	{
		tire_frame += 10 * _game->getMpf();
		if ( tire_frame >= tire_frames ) 
			tire_frame = 0;
	}

//	float mpf_vel = _game->getMpf() * vel;
//	float mpf_hitvel = _game->getMpf() * hit_vel;

	float vel_x, vel_y;
	float dir_angle;

	// position translation
	if ( control == VC_AI )
	{
//		x += (cosf(ai_cur_angle) * mpf_vel + x_acc * mpf_hitvel );
//		y -= (sinf(ai_cur_angle) * mpf_vel + y_acc * mpf_hitvel );
		vel_x = cosf(ai_cur_angle) * vel;
		vel_y = sinf(ai_cur_angle) * vel;

		dir_angle = ai_cur_angle;
	}
	else
	{
//		x += (g_dirx[(int)motion_frame] * mpf_vel + x_acc * mpf_hitvel );
//		y -= (g_diry[(int)motion_frame] * mpf_vel + y_acc * mpf_hitvel );
//		vel_x = g_dirx[(int)motion_frame] * vel;
//		vel_y = g_diry[(int)motion_frame] * vel;

//		float rrd = Deg2Rad(motion_frame * 10.0f);

		vel_x = cosf(motion_angle) * vel;
		vel_y = sinf(motion_angle) * vel;

		dir_angle = motion_angle; //acosf(g_dirx[(int)motion_frame]);
//		DBG(carname << " speed is " << vel << " motion_angle=" << motion_angle);// << " my_rad = " << rrd);
	}

	x += vel_x * _game->getMpf();
	y -= vel_y * _game->getMpf();


	/*
	 * Collisions Check
	 */

	SDL_Rect rPrey;
	SDL_Rect rMine;

	GetFrameRect( &rMine );
	CVehicle *ptr_veh = _game->Auto;

	for ( int j = 0; j < _game->game_num_cars; j++, ptr_veh++ )
	{
		if (j == myIndex)
			continue;

		ptr_veh->GetFrameRect( &rPrey );

		if ( _game->Sdl.Collide(&rMine, GetCurrentFrameMask(), &rPrey, ptr_veh->GetCurrentFrameMask()) )
		{
			// if (_game->Timer.Time() < skip_hit_timer)
			// 	continue;

			DBG( "[COLLIDE] ----- New Collision [" << carname << "] [" << j << "] -----" );

			// Get angle of collision
			float distX = GetX() - ptr_veh->GetX();
			float distY = GetY() - ptr_veh->GetY();
			float collision_angle = atan2f(distY, distX);

			DBG("col_angle = " << collision_angle);

			float my_vel = sqrtf(vel * vel);
			float enemy_vel = sqrtf(ptr_veh->GetVelocity() * ptr_veh->GetVelocity());

			// Get velocity vectors in (new) rotated coordinate system
			float my_ea = (dir_angle + collision_angle);
			float my_velx = my_vel * cosf(my_ea);
			float my_vely = my_vel * sinf(my_ea);

			float ea = (ptr_veh->GetDirectionAngle() + collision_angle);
			float enemy_velx = enemy_vel * cosf(ea);
			float enemy_vely = enemy_vel * sinf(ea);

			DBG("my_angle = " << dir_angle << " my_velx = " << my_velx << " my_vely = " << my_vely);
			DBG("enemy_angle = " << ptr_veh->GetDirectionAngle() << " enemy_velx = " << enemy_velx << " enemy_vely = " << enemy_vely);

			// Get final velocity vectors after collision
			float my_mass = GetCompareVal();
			float enemy_mass = ptr_veh->GetCompareVal();

			float my_fx = (my_velx * (my_mass - enemy_mass) + 2 * enemy_mass * enemy_velx)
					/ (my_mass + enemy_mass);

			float enemy_fx = (enemy_velx * (enemy_mass - my_mass) + 2 * my_mass * my_velx)
					/ (my_mass + enemy_mass);

			float my_fy = my_vely;
			float enemy_fy = enemy_vely;

			// convert vectors to normal Cartesian coordinate system

			float new_my_vel = sqrtf(my_fx * my_fx + my_fy * my_fy);
			float new_enemy_vel = sqrtf(enemy_fx * enemy_fx + enemy_fy * enemy_fy);

			float aa = atan2f(my_fy, my_fx);
			float bb = atan2f(enemy_fy, enemy_fx);
			float new_my_dir = aa + collision_angle;
			float new_enemy_dir = bb + collision_angle;
//
			new_my_dir = FixRad(new_my_dir);
			new_enemy_dir = FixRad(new_enemy_dir);

			// Final

			DBG("new_my_dir = " << new_my_dir << " my_fx= " << my_fx << "  my_fy = " << my_fy);
			DBG("new_enemy_dir = " << new_enemy_dir << " enemy_fx=" << enemy_fx << "  enemy_fy = " << enemy_fy );
			DBG("---------------------------------------------");

			x = tmp_x;
			y = tmp_y;
//			display_frame = tmp_df;
//			motion_frame = tmp_mf;
//			motion_frame = new_my_dir *  57.2957795f; //rad2deg
			SetDirectionAngle(new_my_dir);
			SetVelocity(new_my_vel);

			ptr_veh->SetDirectionAngle(new_enemy_dir);
			ptr_veh->SetVelocity(new_enemy_vel);

			if ( control == VC_AI )
			{
				DBG( "[COLLIDE] Step #8" );
				waypoint.do_reverse = true;
				waypoint.do_reverseTime = _game->Timer.Time() + 500;
//					waypoint.do_precalculate = true;
			}

			break;

			// ----------------------------------


			DBG( "[COLLIDE] ----- New Collision [" << j << "] -----" );
//				DBG("mpf_vel = " << mpf_vel << " mpf_hitvel = " << mpf_hitvel << " vmove=" << vmove);

			bool bHit = true;  // we have impact
			bool no_damage = false;
			x = tmp_x;
			y = tmp_y;
//			motion_frame = tmp_mf;
//			display_frame = tmp_mf;

//				if ( !fIsZero(hit_vel) )
//				{
//					DBG( "[COLLIDE] Step #2| hit_vel = " << hit_vel );
//					ptr_veh->Repulse( (int)rep_frame, hit_vel / (float)ptr_veh->GetCompareVal() );
//				}
//				else
//				{
				DBG( "[COLLIDE] Step #3" );

//					// is velocity 0 ?
//					if ( !fIsZero(vel) )
//					{
//						DBG( "[COLLIDE] Step #4| vel = " << vel );
//						ptr_veh->Repulse( (int)motion_frame, vel / (float)ptr_veh->GetCompareVal() );
//					}
//					else
//					{
//						DBG( "[COLLIDE] Step #5" );
//						/*if ( control == VC_AI || ptr_veh->GetControl() == VC_AI )
//						{
//							//Repulse( (int)GetMotionFrameMirror(), 100 );
//							x = ptr_veh->GetX() + 50;
//							y = ptr_veh->GetY() + 50;
//
//						}*/
//						//ptr_veh->Repulse( (int)motion_frame, 60 ); // !!!!!!!!!
//						//ptr_veh->SetVelocity( 0 );
//						//no_damage = true;
//					}
//				}


			// TODO: fix this because it disbalances the game!
			if ( abs_vel <= MIN_DAMAGE_VELOCITY )
			{
				no_damage = true;
				DBG( "[COLLIDE] Step #6" );
			}

			// enemy vehicle damage calculations
			if ( !no_damage )
			{
				float perc = ( (float)abs_vel / (float)max_vel ) * 100.0f;
				Uint16 tmp_anger = (int)(((float)anger / 100.0f ) * perc);

				anger -= tmp_anger;
				anger = anger < 0 ? 0 : anger;
				anger = anger > 110 ? 110 : anger;

				float speed_damage = (float)damage + 0.10f * (float)abs_vel;
				float armour_absorb = (0.20f * (float)ptr_veh->GetHitPoints());
				DBG( "[DODAMAGE] speed_damage=" << speed_damage << " armour_absorb=" << armour_absorb);
				int fulldmg = speed_damage - armour_absorb + tmp_anger;
				fulldmg = fulldmg < 0 ? 0 : fulldmg;

				DBG( "[DODAMAGE] fulldamage=" << fulldmg << " damage=" << damage << " anger=" << anger << " tmpanger=" << tmp_anger << " abs_vel=" << abs_vel << "  enemyHP=" << ptr_veh->GetHitPoints() );

				ptr_veh->DoDamage( fulldmg, myIndex );

				// PLAY CRASH SOUND
				if ( intGetRnd( 0, 50 ) < 25 )
					_game->Snd.Play( SND_CRASHLIGHT1, (int)x );
				else
					_game->Snd.Play( SND_CRASHLIGHT2, (int)x );
					
			}

			/*
			 * FIXME:
			 * This is a problem right here! Both vehicles will collide thus none
			 * will move. This is the main reason for the nasty physics bug.
			 */
//				this->set_stop = true; //remove this 12.nov
//				vel = vel * -0.6f;

			if ( control == VC_AI )
			{
				DBG( "[COLLIDE] Step #8" );
				waypoint.do_reverse = true;
				waypoint.do_reverseTime = _game->Timer.Time() + 500;

//					vel = vel * 0.5f;

//					waypoint.do_precalculate = true;
//
//					if ( waypoint.goal_type == WAYPOINT_VEHICLE  )
//					{
//						waypoint.do_reverse = true;
//						DBG( "[COLLIDE] Step #9" );
//					}
			}
			else
			{
//					vel = vel * -0.6f;
			}

			DBG( "[COLLIDE] newvelocity=" << vel << " ACC is " << acc);
		}
	}

	// should we leave trails
	if ( trails_time > _game->Timer.Time() )
	{
		if ( tire_trails == VTT_BLOOD && vmove != VM_NONE )
		{
			if ( intGetRnd( 0, 1000 ) < 200 )
				_game->Anims.Create( GetX(), GetY(), ANIM_TIRESPLAT );
		}

		/*if ( tire_trails == VTT_BLOOD && vmove != VM_NONE )
		{
			rdest.x = GetXT() + 4;
			rdest.y = GetYT() + 5;
			SDL_BlitSurface( sprite_tire[0], NULL, back[_game->cur_map], &rdest );
			rdest.x = GetXT() + 8;
			rdest.y = GetYT() + 5;
			SDL_BlitSurface( sprite_tire[0], NULL, back[_game->cur_map], &rdest );
		}
		*/	//if ( intGetRnd( 0, 1000 ) < 200 ) _game->Anims.Create( GetX(), GetY(), ANIM_TIRETRAILS );


		/*if ( tire_trails == VTT_TIRES && vmove != VM_NONE )
			if ( intGetRnd( 0, 1000 ) < 200 ) _game->Anims.Create( GetX(), GetY(), ANIM_TIRESPLAT );
			*/

	}
	else
	{
 		tire_trails = VTT_NONE;
	}
	
	// reset move var
	vmove = VM_NONE;
	
	/*
	 *  Game arena bounds check
	 */
	bool high_speed = fabsf(vel) > 50;
	bool play_sound = false;

    if ( rMine.x < 24 )
	{
		x = 25;
		vel = 0;
		play_sound = true;
	}
    else if ( rMine.w > 614 ) 
	{
		x = 613 - (rMine.w - x);
		vel = 0;
		play_sound = true;
	}
    
	if ( y < 19) 
	{
		y = 20;
		vel = 0;
		play_sound = true;
	}
    else if ( rMine.h > 400 ) 
	{
		y = 399 - (rMine.h - y);
		vel = 0;
		play_sound = true;
	}

	if (play_sound && high_speed) {
		// PLAY SOUND
		if (high_speed)
			_game->Sdl.PlaySound( intGetRnd( 0, 50 ) > 25 ? SND_TIRES1 : SND_TIRES2,
					rMine.x );
	}

	/*
	 * Test for bonuses & items hits (Deadtoys)
	 */

	SDL_Rect rMe;
	SDL_Rect rToy;
	Uint32   dx = 0U, 
			 dy = 0U;

	GetFrameRect( &rMe );

	for( int i = 0; i < DT_MAX_CHILDS; i++ )
	{
		if ( _game->Dtoys.GetToyVisible( i ) )
		{

		 _game->Dtoys.GetToyRect( i, &rToy );		

		 if ( _game->Sdl.Collide( NULL, &rMe, &rToy ) )
		 {
//			if ( abs(vel) >= max_vel / 2 ) // do not hit if speed less than half the max
//			{
				// PLAY SOUND
				_game->Sdl.PlaySound( SND_SPLAT1, rMe.x );
				
				// compute position of blood spill
				dx = rToy.x + (( rToy.w - rToy.x ) / 2);
				dy = rToy.y + (( rToy.h - rToy.y ) / 2);
				
  			    switch( _game->Dtoys.GetToyKind( i ) )
				{
				   case DT_BEARGOAL:

					has_the_goal = true;
					// indicate this vehicle has taken the fraggoal
					_game->Dtoys.SetToyGoalState( DTG_TAKEN );
					goal_time = _game->Timer.Time() + GOALEXPIRE_TIME;

					// show slider
					if ( control == VC_PLAYER1 )
						_game->Anims.Create( ANIM_FRAGMASTER, PS_LEFT );
					else if ( control == VC_PLAYER2 )
						_game->Anims.Create( ANIM_FRAGMASTER, PS_RIGHT );

				   break;

				   case DT_BEARSPEED:

					speed_bonus = max_vel / 3;
					speed_time = _game->Timer.Time() + SPEEDEXPIRE_TIME;
					_game->Anims.Create( dx, dy, ANIM_BLOOD );
					_game->Anims.Create( dx, dy, ANIM_SPLAT );
					tire_trails = VTT_BLOOD;
					trails_time = _game->Timer.Time() + TIRETRAILS_TIME;

					// show slider
					if ( control == VC_PLAYER1 )
						_game->Anims.Create( ANIM_SPEED, PS_LEFT );
					else if ( control == VC_PLAYER2 )
						_game->Anims.Create( ANIM_SPEED, PS_RIGHT );

				   break;

				   case DT_BEARHP:

					hit_points += _game->Dtoys.GetToyValue( i );
					
					if ( hit_points >= hit_points_crash && bcrashlook )
					{
						sprite = sprite_norm;
						mask = mask_norm;
						bcrashlook = false;
					}
					if ( hit_points > max_hitpoints ) 
						hit_points = max_hitpoints;

					_game->Anims.Create( dx, dy, ANIM_BLOOD );
					_game->Anims.Create( dx, dy, ANIM_SPLAT );
				    tire_trails = VTT_BLOOD;
					trails_time = _game->Timer.Time() + TIRETRAILS_TIME;

					// show slider
					if ( control == VC_PLAYER1 )
						_game->Anims.Create( ANIM_REPAIR, PS_LEFT );
					else if ( control == VC_PLAYER2 )
						_game->Anims.Create( ANIM_REPAIR, PS_RIGHT );

				   break;
				 
				   case DT_BEARANGER:
					anger += _game->Dtoys.GetToyValue( i );
					if ( anger > 130 ) anger = 130; // bound anger value to match surface width

					//damage += anger;
					anger_time = _game->Timer.Time() + ANGEREXPIRE_TIME;
					_game->Anims.Create( dx, dy, ANIM_BLOOD );
					_game->Anims.Create( dx, dy, ANIM_SPLAT );
				    tire_trails = VTT_BLOOD;
					trails_time = _game->Timer.Time() + TIRETRAILS_TIME;

					// show slider
					if ( control == VC_PLAYER1 )
						_game->Anims.Create( ANIM_ANGER, PS_LEFT );
					else if ( control == VC_PLAYER2 )
						_game->Anims.Create( ANIM_ANGER, PS_RIGHT );

				   break;

				   case DT_BEARLANDMINE:
					landmines += _game->Dtoys.GetToyValue( i );
					_game->Anims.Create( dx, dy, ANIM_BLOOD );
					_game->Anims.Create( dx, dy, ANIM_SPLAT );
				    tire_trails = VTT_BLOOD;
					trails_time = _game->Timer.Time() + TIRETRAILS_TIME;

					// show slider
					if ( control == VC_PLAYER1 )
						_game->Anims.Create( ANIM_LANDMINE, PS_LEFT );
					else if ( control == VC_PLAYER2 )
						_game->Anims.Create( ANIM_LANDMINE, PS_RIGHT );

				   break;

				}

				// kill toy index
				_game->Dtoys.KillToy( i );
			 //}
			 // else
			 // {
				// // speed is not high enough to smash this bonus
				// vel = 0;
				// x = tmp_x;
				// y = tmp_y;
			 // }
		 	}
		} // vis_check...
	} 


	// Hit_Test landmines...
	for ( int i = 0; i < LANDMINE_CHILDS; i++ )
	{
		if ( _game->Mines.GetMineVisible( i ) && _game->Mines.GetMineIndex( i ) != myIndex)
		{
			_game->Mines.GetMineRect( i, &rToy );
			
			if ( _game->Sdl.Collide( NULL, &rMe, &rToy ) ) //_game->Sdl.Collide( &rMe, GetCurrentFrame(), &rToy, _game->Mines.GetMineCurrentFrame( i ) ) )
			{
				// do vehicle damage
				DoDamage( LANDMINE_DAMAGE, _game->Mines.GetMineIndex( i ) );
				_game->Mines.KillMine( i );
				_game->Snd.Play( SND_EXPLOSION1, (int)x ); // PLAYSOUND
			}
		}
	}

}


/////////////////////////////////////////////////////////////////////////
//// Name: Repulse()
//// Desc:
/////////////////////////////////////////////////////////////////////////
//void CVehicle::Repulse( int frame_angle, float speed )
//{
//
////	motion_frame = (float)frame_angle;
//	rep_frame = (float)frame_angle;
////	hit_vel = speed;
//	x_acc = g_dirx[frame_angle];
//	y_acc = g_diry[frame_angle];
//	//vel = vel + (float)hit_vel;
//
//	if ( control == VC_AI )
//		waypoint.do_precalculate = true;
//
//
//}

///////////////////////////////////////////////////////////////////////
// Name: GetCurrentFrame()
// Desc:
///////////////////////////////////////////////////////////////////////
SDL_Surface* CVehicle::GetCurrentFrame()
{
	return sprite[(int)display_frame + MAX_ROTATION_FRAMES * (int)tire_frame];
}

///////////////////////////////////////////////////////////////////////
// Name: GetCurrentFrameMask()
// Desc:
///////////////////////////////////////////////////////////////////////
Uint32* CVehicle::GetCurrentFrameMask()
{
	return mask[(int)display_frame + MAX_ROTATION_FRAMES * (int)tire_frame];
}

///////////////////////////////////////////////////////////////////////
// Name: GetFrameRect()
// Desc: kvadrat za stylknovenie
///////////////////////////////////////////////////////////////////////
void CVehicle::GetFrameRect( SDL_Rect *rect )
{
	SDL_Surface *currentSurf = GetCurrentFrame();

	rect->x = (Uint32)x;
	rect->y = (Uint32)y;
	rect->w = rect->x + currentSurf->w;
	rect->h = rect->y + currentSurf->h;
}

/////////////////////////////////////////////////////////////////////////
//// Name: GetMotionFrame()
//// Desc:
/////////////////////////////////////////////////////////////////////////
//float CVehicle::GetMotionFrame()
//{
//	return motion_frame;
//};
//
/////////////////////////////////////////////////////////////////////////
//// Name: GetMotionFrameMirror()
//// Desc: Get an opposite (to the current) movement angle.
/////////////////////////////////////////////////////////////////////////
//float CVehicle::GetMotionFrameMirror()
//{
//	float tmp_frame = motion_frame;
//
//	tmp_frame += HALF_ROTATION_FRAMES;
//	if ( tmp_frame > MAX_ROTATION_FRAMES )
//		tmp_frame -= MAX_ROTATION_FRAMES;
//
//	//tmp_frame--;
//
//	return tmp_frame;
//}


///////////////////////////////////////////////////////////////////////
// Name: GetDirectionAngle()
// Desc: Get angle of movement relative to local coord. system
///////////////////////////////////////////////////////////////////////
float CVehicle::GetDirectionAngle()
{
	if ( control == VC_AI )
	{
		return ai_cur_angle;
	}

	return motion_angle;
//	return Deg2Rad(motion_frame * 10.0f); //(motion_frame ) * 10.0f * 0.0174532925f;
//	return acosf(g_dirx[(int)motion_frame]);
}

///////////////////////////////////////////////////////////////////////
// Name: SetDirectionAngle()
// Desc:
///////////////////////////////////////////////////////////////////////
void CVehicle::SetDirectionAngle(float rad)
{
	if ( control == VC_AI )
	{
		ai_cur_angle = rad;
	}
	else
	{
		motion_angle = rad;
//		motion_frame = (Rad2Deg(rad) / 10.0f) ; //- 1.0f;
		DBG(" new motion_frame = " << rad);
	}

	skip_hit_timer = _game->Timer.Time() + 800;
	reset_frame = true;
}

///////////////////////////////////////////////////////////////////////
// Name: GetX()
// Desc:
///////////////////////////////////////////////////////////////////////
float CVehicle::GetX()
{
	SDL_Surface *surf = GetCurrentFrame();
	return x + surf->w * 0.5f;
}

///////////////////////////////////////////////////////////////////////
// Name: GetY()
// Desc:
///////////////////////////////////////////////////////////////////////
float CVehicle::GetY()
{
	SDL_Surface *surf = GetCurrentFrame();
	return y + surf->h * 0.5f;
}

///////////////////////////////////////////////////////////////////////
// Name: DoDamage()
// Desc: Substract given damage from vehicle HP. Destroy vehicle if
//       HP is too low.
///////////////////////////////////////////////////////////////////////
void CVehicle::DoDamage( int damageAmount, Uint32 attackerIndex )
{
	hit_points -= damageAmount;
	if (hit_points <= 0)
	{
		visible = false;

		_game->Anims.Create(GetX(), GetY(), ANIM_EXPLOSION);
		// PLAYSOUND
		_game->Snd.Play(SND_EXPLOSION1, (int) x);

		// give or take a frag...
		if (attackerIndex == myIndex) {
			if (i_self_destruct) {
				i_self_destruct = false;

				frags--;
				if (frags < 0)
					frags = 0;
			}
		}
		else if (attackerIndex != NO_ATTACKER && _game->Auto[attackerIndex].GetTeam() != team) {
			_game->Auto[attackerIndex].AddFrags(1);
		}

		// in case this vehicle held 'the goal'
		if (has_the_goal) {
			_game->Dtoys.SetToyGoalState(DTG_ONTHEROAD);
			has_the_goal = false;
		}

	} else if (hit_points <= hit_points_crash && !bcrashlook)
	{
		// show smashed vehicle sprites
		sprite = sprite_crash;
		mask = mask_crash;
		bcrashlook = true;

		// PLAYSOUND
		_game->Snd.Play(SND_CRASHBRAKE, (int) x);
	}
}


///////////////////////////////////////////////////////////////////////
// Name: Update()
// Desc: obnovqva classa i parametrite na unit-a
///////////////////////////////////////////////////////////////////////
void CVehicle::Update()
{
	// respawn vehicle (if not visible, e.g., destroyed)
	if (!visible) {
		Create();
		//return;
	}

	DoMotion();
//	if ( control != VC_AI ) DoMotion();
 
	// expire 1 anger point
	if (anger_time < _game->Timer.Time() && anger > 0)
	{
		anger--;
		anger_time = _game->Timer.Time() + ANGEREXPIRE_TIME;
	}
	// kill speed_bonus
	if (speed_time < _game->Timer.Time() && speed_bonus != 0)
	{
		speed_bonus = 0;
	}

	// goal time expired?
	if (goal_time < _game->Timer.Time() && has_the_goal)
	{
		// give 3 frags and set goal back
		has_the_goal = false;
		_game->Dtoys.SetToyGoalState(DTG_ONTHEROAD);
		frags += 1;
	}

	Uint32 *cur_mask = GetCurrentFrameMask();
	SDL_Surface *surf = GetCurrentFrame();
	SDL_Rect rect = {0, 0, surf->w, surf->h};

	// blit shadow and vehicle
	_game->Sdl.BlitShadow(x + 1, y + 4, cur_mask, &rect);
	_game->Sdl.AddToBlit(x, y, surf);
 
 
	// blit status
	char buf[8];

	sprintf(buf, "%d", frags);
	_game->Sdl.DrawNum(pos_frag[myIndex].x, pos_frag[myIndex].y, buf);

	float perc = ((float) hit_points / (float) max_hitpoints) * 100.0f;
	_game->scales[0]->w = (Uint32) ( /*(130.0f / 100.0f )*/1.3f * perc);
	_game->Sdl.BlitNow(pos_hp[myIndex].x, pos_hp[myIndex].y, _game->scales[0]);
	_game->scales[1]->w = anger;
	_game->Sdl.BlitNow(pos_anger[myIndex].x, pos_anger[myIndex].y, _game->scales[1]);

	// update controls
	switch (control)
	{
	case VC_PLAYER1:

		if (_game->Sdl.keys[_game->Bindings.GetP1Key(CBindings::BK_LEFT)])
			Rotate(VR_LEFT);

		if (_game->Sdl.keys[_game->Bindings.GetP1Key(CBindings::BK_RIGHT)])
			Rotate(VR_RIGHT);

		if (_game->Sdl.keys[_game->Bindings.GetP1Key(CBindings::BK_ACC)])
			Move(VM_FORWARD);

		if (_game->Sdl.keys[_game->Bindings.GetP1Key(CBindings::BK_BREAK)])
			Move(VM_BACKWARD);

		if (_game->Sdl.keys[_game->Bindings.GetP1Key(CBindings::BK_HONK)])
		{
			honk_status = 1;
		}
		else
		{
			honk_status = honk_status == 1 ? 2 : 0;
		}

		if (_game->Sdl.keys[_game->Bindings.GetP1Key(CBindings::BK_BLOWUP)]) {
			self_destruct = true;
			i_self_destruct = true;
		}

		//{!}
		if (_game->Sdl.JoystickHatState == SDL_HAT_LEFT)
			Rotate(VR_LEFT);
		if (_game->Sdl.JoystickHatState == SDL_HAT_RIGHT)
			Rotate(VR_RIGHT);
		if (_game->Sdl.GetJoystickButtonPressed(1))
			Move(VM_FORWARD);
		if (_game->Sdl.GetJoystickButtonPressed(2))
			Move(VM_BACKWARD);

		if (_game->Sdl.keys[_game->Bindings.GetP1Key(CBindings::BK_MINE)])
		{
			if (!bputmine)
				bputminekey = true;
		}
		else if (!_game->Sdl.keys[_game->Bindings.GetP1Key(CBindings::BK_MINE)])
		{
			bputminekey = false;
			bputmine = false;
		}

		break;

	case VC_PLAYER2:
		if (_game->Sdl.keys[_game->Bindings.GetP2Key(CBindings::BK_LEFT)])
			Rotate(VR_LEFT);

		if (_game->Sdl.keys[_game->Bindings.GetP2Key(CBindings::BK_RIGHT)])
			Rotate(VR_RIGHT);

		if (_game->Sdl.keys[_game->Bindings.GetP2Key(CBindings::BK_ACC)])
			Move(VM_FORWARD);

		if (_game->Sdl.keys[_game->Bindings.GetP2Key(CBindings::BK_BREAK)])
			Move(VM_BACKWARD);

		if (_game->Sdl.keys[_game->Bindings.GetP2Key(CBindings::BK_BLOWUP)])
		{
			self_destruct = true;
			i_self_destruct = true;
		}

		if (_game->Sdl.keys[_game->Bindings.GetP2Key(CBindings::BK_HONK)])
		{
			honk_status = 1;
		}
		else
		{
			honk_status = honk_status == 1 ? 2 : 0;
		}

		if (_game->Sdl.keys[_game->Bindings.GetP2Key(CBindings::BK_MINE)])
		{
			if (!bputmine)
				bputminekey = true;
		}
		else if (!_game->Sdl.keys[_game->Bindings.GetP2Key(CBindings::BK_MINE)])
		{
			bputminekey = false;
			bputmine = false;
		}

		break;

		// case VC_PLAYER3:
		// 	if ( _game->Sdl.keys[SDLK_j] ) Rotate( VR_LEFT );
		//if ( _game->Sdl.keys[SDLK_l] ) Rotate( VR_RIGHT );
		//if ( _game->Sdl.keys[SDLK_i] ) Move( VM_FORWARD );
		//if ( _game->Sdl.keys[SDLK_k] ) Move( VM_BACKWARD );
		// break;

	case VC_AI:
		AI_Update();

		if (ai_putmine)
		{
			ai_putmine = false;
			bputminekey = true;
		}

		break;

	default:
		break;

	}

	// put down a landmine?
	if (bputminekey)
	{
		bputmine = true;
		if (landmines > 0) {
			landmines--;
			_game->Mines.Create(GetX(), GetY(), myIndex);
		}

		bputminekey = false;
	}

	// if vehicle is stuck run automated destruction timer
	if (fIsZero(vel) && !self_destruct)
	{
		if (!ai_stuck) {
			ai_stuck = true;
			ai_stucktime = _game->Timer.Time() + 4000 + (rand() % 2000);
		}
	}
	else
	{
		ai_stuck = false;
	}

	if (ai_stuck)
	{
		if (ai_stucktime < _game->Timer.Time())
		{
//			self_destruct = true;
//			ai_stuck = false;
		}
	}

	// check self-destruct flags
	if (self_destruct && !self_destruction)
	{
		self_destruction = true;
		self_destruct = false;
		destruct_time = _game->Timer.Time() + 3000;
	}

	if (self_destruction)
	{
		if (destruct_time > _game->Timer.Time())
		{
			//_game->Sdl.DrawNum( (int)x, (int)y, "1" );
			_game->Sdl.BlitNow((int) x, (int) y, _game->self_dest);

			// PLAY SOUND
			if (warning_time < _game->Timer.Time()) {
				warning_time = _game->Timer.Time() + 1000;
				_game->Snd.Play(SND_WARNING, (int) x);
			}
		}
		else
		{
			DoDamage(1000U, myIndex);
			// clear local var
			self_destruction = false;
		}
	}

	if (honk_status == 2)
	{
		_game->Snd.Play( SND_MENU_HONK1, (int)x );
		honk_status = 0;
	}

} 

///////////////////////////////////////////////////////////////////////
// Name: AI_Update()
// Desc: Update AI state
///////////////////////////////////////////////////////////////////////
void CVehicle::AI_Update()
{

	float		    distance;
/*	static float    tmp_x, tmp_y;
	
	tmp_x = x;
	tmp_y = y;*/

/*	// proveri dali kolata ne e zasednala, ako e taka vkl. "_game->self_destruction"
	if ( vel == 0 )
	{
		if ( !ai_stuck )
		{
			ai_stuck = true;
			ai_stucktime = _game->Timer.Time() + 2000 + (rand()%2000);
		}
	}
	else
		ai_stuck = false;

	if ( ai_stuck )
	{
		if ( ai_stucktime < _game->Timer.Time() )
			_game->self_destruct = true;
	}
*/

	// ako e udaril avtomobil to vyrni go nazad za 'n' kadyra 
	if ( waypoint.do_reverse )
	{
		Move( VM_BACKWARD );

		// 25% chance da pusni mina pri zaden hod
		if ( intGetRnd( 0, 100 ) < 25 )
			ai_putmine = false;
		
		if (waypoint.do_reverseTime < _game->Timer.Time())
		{
			waypoint.do_reverse = false;
			waypoint.reached = true;
		}
		else
		{
			return;
		}
	}

	// proveri dali nqkoi veche ne vzel bonus-a ili dali ne e izcheznal
	if ( waypoint.goal_type == WAYPOINT_BONUS )
	{
		if ( !_game->Dtoys.GetToyVisible( waypoint.index ) )
			waypoint.reached = true;
	}
	else if ( waypoint.goal_type == WAYPOINT_VEHICLE )
	{
		// proveri dali avtomobila ne e unishtojen
		if ( !_game->Auto[waypoint.index].GetVisible() )
			waypoint.reached = true;
	}



	// proveri dali sme dostignali zadadenata poziciq
	distance = fGetDistanceNSR( GetX(), GetY(), waypoint.x, waypoint.y );
	if ( distance <= WAYPOINT_RADIUS ) 
		waypoint.reached = true;

	// ako sme dostignali poziciqta to generirai nov waypoint
	if ( waypoint.reached )
	{
		AI_GenerateWaypoint();
		return;
	}
	

	if ( !waypoint.static_pos || waypoint.do_precalculate )
	{
		// ako e avtomobil to toi se e pridvijil i waypoint-a trqbwa da byde pre-def
		if ( waypoint.goal_type == WAYPOINT_VEHICLE )
		{
			waypoint.x = (float)_game->Auto[waypoint.index].GetX();
			waypoint.y = (float)_game->Auto[waypoint.index].GetY();
		}

		AI_ProcessWaypoint();
	}

	Move( VM_FORWARD );

}


///////////////////////////////////////////////////////////////////////
// Name: AI_GenerateWaypoint()
// Desc: Generate new AI action.
///////////////////////////////////////////////////////////////////////
void CVehicle::AI_GenerateWaypoint()
{

	Uint16    action		= 0U;
	Uint32	  bonus_index	= 0U, 
  			  car_index		= 0U;
	Uint32	  i				= 0U;
	Uint32	  distance		= 0L, 
			  tmp_dist		= 0L;
	bool	  bonus_found	= false;
	SDL_Rect  rtoy;
	Uint32	  bonus_list_index[DT_MAX_CHILDS];
	Uint16	  bonus_list[DT_MAX_DEADTOYS];
	Uint16	  bonus_list_warrior[DT_MAX_DEADTOYS]	= { 30, 25, 15, 10, 20 };
	Uint16	  bonus_list_explorer[DT_MAX_DEADTOYS]	= { 45, 15, 25, 12, 8 };
	CVehicle  *ptr_veh = _game->Auto;
	
	memset( bonus_list, 0U, DT_MAX_DEADTOYS * sizeof(Uint16) );

	if ( avt == AVT_WARRIOR )				 // TYPE _WARRIOR
	{
		if ( intGetRnd( 0, 1000 ) > 500 )
			action = ACTION_ATTACK;
		else if ( intGetRnd( 0, 1000 ) < 250 )
			action = ACTION_DROPMINE;
		else
		{
			action = ACTION_TAKEBONUS;

			// proveri za bonusi i systavi spisyk
			for( i = 0; i < DT_MAX_CHILDS; i++ )
			{
				if ( _game->Dtoys.GetToyVisible( i ) )
				{
					bonus_list[_game->Dtoys.GetToyKind( i )] = bonus_list_warrior[_game->Dtoys.GetToyKind( i )];
					bonus_list_index[_game->Dtoys.GetToyKind( i )] = i;
					bonus_found = true;
				}
			}

			// v sluchai che nqma vidimi bonusi
			if ( !bonus_found ) action = ACTION_ATTACK;
		}


	}
	else if ( avt == AVT_EXPLORER )			 // TYPE _EXPLORER
	{

		if ( intGetRnd( 0, 1000 ) < 250 )
			action = ACTION_ATTACK;
		else if ( intGetRnd( 0, 1000 ) > 850 )
			action = ACTION_DROPMINE;
		else
		{
			action = ACTION_TAKEBONUS;

			for( i = 0; i < DT_MAX_CHILDS; i++ )
			{
				if ( _game->Dtoys.GetToyVisible( i ) )
				{
					bonus_list[_game->Dtoys.GetToyKind( i )] = bonus_list_warrior[_game->Dtoys.GetToyKind( i )];
					bonus_list_index[_game->Dtoys.GetToyKind( i )] = i;
					bonus_found = true;
				}
			}

			if ( !bonus_found ) action = ACTION_ATTACK;
		}
	}


	// izvyrshi izbranoto deistvie
	switch( action )
	{
	case ACTION_ATTACK:
		
		// vzemi razstoqnieto do 1viq avtomobil
		i = car_index = 0;
		distance = UINT_MAX;
		
		while( i < _game->game_num_cars )
		{
			if ( i != myIndex )
			{
				// Get nearest (non-team) car
				
				//tmp_dist = GetDistance( GetX(), GetY(), ptr_veh->GetX(), ptr_veh->GetY() );
				tmp_dist = GetDistanceNSR( GetX(), GetY(), ptr_veh->GetX(), ptr_veh->GetY() );
				
				if ( ( distance > tmp_dist || ptr_veh->GetPossessTheGoal() ) 
					&& ptr_veh->GetVisible() 
					&& ( ptr_veh->GetTeam() != team ) ) 
				{
					car_index	= i;
					distance	= tmp_dist; 
				}
			}

			ptr_veh++;
			i++;
		}
		
		// zapylni info-to za waypoint-a
		ptr_veh = &_game->Auto[car_index];
		
		waypoint.x				 = ptr_veh->GetX();
		waypoint.y				 = ptr_veh->GetY();
		waypoint.reached		 = false;
		waypoint.static_pos		 = false;
		waypoint.index			 = car_index;
		waypoint.goal_type		 = WAYPOINT_VEHICLE;
		waypoint.do_reverse		 = false;
		waypoint.do_precalculate = false;

	break;

	
	case ACTION_DROPMINE:
		ai_putmine = true;
	break;


	case ACTION_TAKEBONUS:
		
		// vzemi "proizvolen" bonus ot spisyka
		Uint16 bonus_action;

		bonus_action = AI_doFSM( bonus_list, DT_MAX_DEADTOYS );
		bonus_index = bonus_list_index[bonus_action];

		_game->Dtoys.GetToyRect( bonus_index, &rtoy );
		
		waypoint.x = rtoy.x;
		waypoint.y = rtoy.y;
		waypoint.reached = false;
		waypoint.static_pos = true;
		waypoint.index = bonus_index;
		waypoint.goal_type = WAYPOINT_BONUS;
		waypoint.do_reverse = false;
		waypoint.do_precalculate = false;

	break;

	
	default:
		LOG( "GAME_AI_ERROR: " << __FILE__ << " at line: " << __LINE__ );
	break;
	
	}

	AI_ProcessWaypoint();
}



///////////////////////////////////////////////////////////////////////
// Name: AI_ProcessWaypoint()
// Desc: adjust sprite direction based on the direction of movement
///////////////////////////////////////////////////////////////////////
void CVehicle::AI_ProcessWaypoint()
{

	float x_dist, y_dist;
	float la;
	float tmp_dest, tmp_cur;

	// get angle to the next waypoint
	x_dist = (waypoint.x - x);
	y_dist = (waypoint.y - y);
		
	if ( y_dist > 0.0f )
	{
		ai_dest_angle = atanf( x_dist / y_dist ) - PI_2;
	}
	else if ( y_dist < 0.0f )
	{
		//st_angle = atan2( x_dist, y_dist );// + PI_2;
		ai_dest_angle = atanf( x_dist / y_dist ) + PI_2;
	}
	else
	{
		ai_dest_angle = atanf( x_dist /y_dist ) - PI_2;
	}

	// check if display frame must be recalculated
	if ( ai_dest_angle == ai_cur_angle ) 
	{
		ai_turning = VR_NONE;
		return;
	}

	// get sensible turning
	tmp_dest	= ai_dest_angle;
	tmp_cur		= ai_cur_angle;
	FixAngle( &tmp_dest );
	FixAngle( &tmp_cur );

	if ( ( PI2 - tmp_dest ) + tmp_cur < tmp_dest )
	{
		ai_turning = VR_RIGHT;
	}
	else 
	{
		ai_turning = VR_LEFT;
	}


	/*// izchisli max skorost pri zavoi
	distance = fGetDistance( (float)GetX(), (float)GetY(), waypoint.x, waypoint.y );
	ai_maxvel = sqrt( acc * distance );  // a = (v*v)/R
	*/

	/*char buf[64];
	sprintf( buf, "da: %f ca: %f  max_vel: %d ", ai_dest_angle, ai_cur_angle, ai_maxvel );
	AppendToLog( buf );*/

	
	//-------display _frame
	la = (float)(ai_dest_angle * 180 / PI); 
	if ( la < 0 ) 
	{
		la += 360;
	}
	
	if ( la > 360 ) la -= 90;
	
	ai_final_frame = la * 0.1f; //la / 10;

	//display_frame = la/10;
	//motion_frame = display_frame;*/

}


///////////////////////////////////////////////////////////////////////
// Name: AI_doFSM()
// Desc: choose (F)inite-(S)tate-(M)achines based action
///////////////////////////////////////////////////////////////////////
Uint16 CVehicle::AI_doFSM( Uint16 *proActions, Uint16 max_actions )
{

	Uint16 index	= 0U, 
		   i		= 0U, 
		   j		= 0U;
	int	   val		= 0, 
		   tmp_val  = 0;
	bool   proa_ok  = false;

	// get a rnd num
	while ( val == 0 || val < 10 || !proa_ok )
	{
		val = rand()%100;
		
		// at least one value that is higher than rnd
		for ( i = 0; i < max_actions; i++ )
		{
			if ( val > proActions[i] && proActions[i] != 0 )
			{
				proa_ok = true;
				break;
			}
		}

	}

	// find action with positive subs difference
	for ( j = 0; j < max_actions; j++ )
	{
		if ( ( val - proActions[j] ) >= 0 && proActions[j] != 0 )
		{
			index = j;
			break;
		}
	}

	// find closest action
	for ( i = j; i < max_actions; i++ )
	{
		tmp_val = val - proActions[i];
		
		if ( tmp_val >= 0 && tmp_val < ( val - proActions[index] ) && proActions[i] != 0 )
			index = i;
	}

	return index;
}

