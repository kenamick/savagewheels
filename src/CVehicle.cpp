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

static int attribACC[3] = { 160, 220, 320 };
static int attribSPEED[5] = { 180, 220, 260, 310, 340 };
static int attribROT[3] = { 15, 20, 25 };
static int attribDAMAGE[4] = { 40, 60, 70, 100 };
static int attribARMOUR[4] = { 210, 260, 300, 400 };

/*
 frags	     x:105 y:430  x:265 y:430  	x:425 y:430   x:585 y:430
 hitpoints - x:5 y:445    x:165 y:445   x:325 y:445   x:485 y:445
 anger 	     x:5 y:460    x:165 y:460   x:325 y:460   x:485 y:460
*/
static POINT  pos_frag[4]  = { 105, 428, 265, 428, 425, 428, 585, 428 };
static POINT  pos_hp[4]  = { 5, 445, 165, 445, 325, 445, 485, 445 };
static POINT  pos_anger[4] = { 5, 460, 165, 460, 325, 460, 485, 460 };
static POINT  pos_warp[4]  = { 30, 30, 30, 360, 535, 30, 535, 360 };

/*
 * 36 precalculated sin/cos values for each vehicle animation frame
 * calced by LordCIH using Matlab ;]
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
// Desc:
///////////////////////////////////////////////////////////////////////
CVehicle::CVehicle()
: _game( NULL ),
   released(true),
   visible(false),
   honk_status(0)
{
	driver_name = (SDL_Surface *)NULL;
	sprite = sprite_norm = sprite_crash = (SDL_Surface **)NULL;

	mask = mask_crash = mask_norm = NULL;
}


///////////////////////////////////////////////////////////////////////
// Name: Release()
// Desc: Release allocated objects
///////////////////////////////////////////////////////////////////////
void CVehicle::Release()
{
	if ( released ) 
		return;

	AppendToLog( "Closing CVehicle() class..." );

	for ( int cn = 0; cn < MAX_ROTATION_FRAMES * max_tire_frames; cn++ )
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
// Desc: Create vehicle from data module
///////////////////////////////////////////////////////////////////////
int CVehicle::Initialize( CGame *game, const SWV_HEADER *swv, Uint16 carIndex )
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
	max_tire_frames = frames / MAX_ROTATION_FRAMES;
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
			return 0;
		}

		// create a mask for this sprite
		_game->Sdl.MakeBoolMask(sprite_norm[cn], mask_norm[cn]);

		// crashed sprites (calc.offset to the crashed sprites)
		index = cn + 3 + frames;

		if ((sprite_crash[cn] = _game->Sdl.LoadBitmap(vehicle_filename,
				swv->pfiles[index].pos, swv->pfiles[index].length, MAGENTA,
				NO_ALPHA)) == NULL) {
			return 0;
		}

		// create a mask for this sprite
		_game->Sdl.MakeBoolMask(sprite_crash[cn], mask_crash[cn]);
	}

	// load driver_name surface
	if ((driver_name = _game->Sdl.LoadBitmap(vehicle_filename,
			swv->pfiles[2].pos, swv->pfiles[2].length, BLACK, NO_ALPHA))
			== NULL) {
		return 0;
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
//	set_stop = false;

	return 1;
}


///////////////////////////////////////////////////////////////////////
// Name: SetAttirbs()
// Desc: car-attribs based on game difficulty
///////////////////////////////////////////////////////////////////////
void CVehicle::SetAttirbs( CONST_DIFFICULTY difficulty )
{
	float diff_perc = 1.0f;

	switch( difficulty )
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
	    max_tire_frames = 1;
	  break;   
   
	  case VT_FERRARI:
		sprintf ( carname, "%s", "fera" );
   	    max_tire_frames = 1;
	  break;
	  
	  case VT_MERCEDES:
		sprintf ( carname, "%s", "merc" );
		max_tire_frames = 1;
   	  break;

      case VT_BUGGY:
   	    sprintf ( carname, "%s", "bugi" );
		max_tire_frames = 2;
	  break;

	  default: return 0;
 }

 // ...
 sprite_norm = (SDL_Surface **) new SDL_Surface[MAX_ROTATION_FRAMES * max_tire_frames];
 sprite_crash = (SDL_Surface **) new SDL_Surface[MAX_ROTATION_FRAMES * max_tire_frames];



 // load animation 
 for( cn = 0; cn < MAX_ROTATION_FRAMES * max_tire_frames; cn++ )
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
// Desc: Initialize new vehicle instance
///////////////////////////////////////////////////////////////////////
void CVehicle::Create()
{
	bool 		do_not_warp = true;
	int 		warpPos = 0;

	// get a safe vehicle spawn point
	// warpPos - one of 4 possible warp points
	while (do_not_warp) {
		warpPos = (int) (rand() % 4);
		do_not_warp = false;

		for (int i = 0; i < _game->game_num_cars; i++)
		{
			if (i != myIndex)
			{
				if (_game->Auto[i].GetVisible())
				{
					Uint32 dist = GetDistanceNSR(
							_game->Auto[i].GetCX(),
							_game->Auto[i].GetCY(),
							pos_warp[warpPos].x,
							pos_warp[warpPos].y);

					if (dist < SAFE_WARP_DISTANCE)
					{
						do_not_warp = true;
						break;
					}
				}
			}
		}

		//if ( !do_not_warp )
		//	break;
	}

	//if ( do_not_warp ) return;

	x_acc = 0.0f;
	y_acc = 0.0f;
	hit_vel = 0.0f;
	landmines_count = 0;
	vmove = VM_NONE;
	vrot = VR_NONE;

	// point the current animations to the array of non-crashed vehicle animations
	sprite = sprite_norm;
	mask = mask_norm;
	crashed_look = false;

	// reset vars
	x = pos_warp[warpPos].x;
	y = pos_warp[warpPos].y;
	velocity_bonus = 0;
 
	if ( warpPos == 2 || warpPos == 3 )
	{
		motion_frame = 18;
		display_frame = motion_frame;
	}
	else
	{
		motion_frame = 0;
		display_frame = 0;
	}

	center_x = (Uint32)x + (sprite[(int)motion_frame]->w / 2);
	center_y = (Uint32)y + (sprite[(int)motion_frame]->h / 2);

	// check for memory access violation
	ASSERT(sprite_norm[(int)motion_frame]->w < 500);
	ASSERT(sprite_norm[(int)motion_frame]->h < 500);

	tire_frame = 0;
	vel = 0.0f;
	tire_trails = VTT_NONE;
	trails_time = 0;

	// ai_facing
	ai_cur_angle = motion_frame * 10;
	ai_stuck = false;

	visible = true;
	self_destruct = self_destruction = false;
	warning_time = 0;
	
	hit_points = max_hitpoints;

	honk_status = 0;

	// reset clipping check
	collisions.check = true;
	collisions.last_check_time = 0;
	collisions.disabled = false;
}


///////////////////////////////////////////////////////////////////////
// Name: SetControl()
// Desc: Set Vehicle control operator
///////////////////////////////////////////////////////////////////////
void CVehicle::SetControl( CONST_VEHICLE_CONTROL vcontrol )
{
	control = vcontrol;
	
	// AI type selection
	waypoint.reached = true;
	if ( control == VC_AI )
	{
		if ( intGetRnd( 0, 10 ) < 5 )
		{
			avt = AVT_WARRIOR;
		}
		else
		{
			avt = AVT_EXPLORER;
		}
	}
}


///////////////////////////////////////////////////////////////////////
// Name: Move()
// Desc:
///////////////////////////////////////////////////////////////////////
void CVehicle::Move( CONST_VEHICLE_MOVEMENT mvt )
{
	vmove = mvt;
	motion_frame = display_frame;
}


///////////////////////////////////////////////////////////////////////
// Name: Rotate()
// Desc:
///////////////////////////////////////////////////////////////////////
void CVehicle::Rotate( CONST_VEHICLE_ROTATION rot )
{
	vrot = rot;
	motion_frame = display_frame;
}


///////////////////////////////////////////////////////////////////////
// Name: DoMotion()
// Desc: Do vehicle motion and collisions tests
///////////////////////////////////////////////////////////////////////
void CVehicle::DoMotion()
{
	float tmp_mf 		= motion_frame;
	float tmp_x 		= x;
	float tmp_y 		= y;
	float tmp_maxvel 	= (float) max_vel;
	float tmp_vel 		= vel;
	float abs_vel 		= fabsf(vel);
	float rot_m			= 1.0f;

	/*
	 * AI steering
	 */

	if ( control == VC_AI )
	{
		rot_m = 2.0f;  // double rotation speed for AIs

		tmp_maxvel = max_vel; //ai_maxvel;

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
				tmp_maxvel = max_vel;
			}
		}
		else if ( ai_turning == VR_RIGHT )
		{
			ai_cur_angle -= ((rot_speed/2) * _game->getMpf());
			if ( ai_cur_angle <= ai_dest_angle )
			{
				ai_cur_angle = ai_dest_angle;
				ai_turning = VR_NONE;
				tmp_maxvel = max_vel;
			}
		}
	}

	/*
	 * Rotation
	 */

	if ( vrot == VR_LEFT && vel != 0 )
    {
	    display_frame += (rot_speed * rot_m * _game->getMpf());

		if ( display_frame > MAX_ROTATION_FRAMES - 1 )
			display_frame = 0;

		if ( control == VC_AI ) // ai_case
		  if ( display_frame > ai_final_frame )
			  display_frame = ai_final_frame;

	}
	else if ( vrot == VR_RIGHT && vel != 0 )
	{
		display_frame -= (rot_speed * rot_m * _game->getMpf());

		if ( display_frame < 0 )
			display_frame = MAX_ROTATION_FRAMES - 1;

		if ( control == VC_AI ) // ai_case
		  if ( display_frame < ai_final_frame )
			  display_frame = ai_final_frame;

	}

	vrot = VR_NONE;
	motion_frame = display_frame;

	/*
	 * Acceleration
	 */

	float maxvel_p = tmp_maxvel + velocity_bonus;
	// 50% of allowed velocity when going backwards
	float maxvel_n = -maxvel_p * 0.5f;

	if ( vmove == VM_FORWARD )
	{
		vel += (float)acc * _game->getMpf();
		if ( vel > maxvel_p )
			vel = maxvel_p;
	}
	else if ( vmove == VM_BACKWARD )
	{
		vel -= (float)acc * _game->getMpf();
		if ( vel < maxvel_n )
			vel = maxvel_n;
	}
	else
	{
		// imitate friction (Newton's first law)
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

	/*
	 * Impact friction
	 */
	if ( hit_vel > 0.0f )
	{
		hit_vel -= _game->getMpf() * (float)dec_acc;
		if ( hit_vel < 0.0f )
			hit_vel = 0.0f;
	}
	else if ( hit_vel < 0.0f )
	{
		hit_vel += _game->getMpf() * (float)dec_acc;
		if ( hit_vel > 0.0f )
			hit_vel = 0.0f;
	}

	// rotate vehicle tires (if moving)
	if (max_tire_frames > 1 && abs_vel > 0.0f)
	{
		tire_frame += 10 * _game->getMpf();
		
		if ( tire_frame >= max_tire_frames )
			tire_frame = 0;
	}

	/*
	 * Position translation
	 */

	float mpf_vel = _game->getMpf() * vel;
	float mpf_hitvel = _game->getMpf() * hit_vel;

	if ( control == VC_AI )
	{
		x += cosf(ai_cur_angle) * mpf_vel + x_acc * mpf_hitvel;
		y -= sinf(ai_cur_angle) * mpf_vel + y_acc * mpf_hitvel;
	}
	else
	{
		x += g_dirx[(int)motion_frame] * mpf_vel + x_acc * mpf_hitvel;
		y -= g_diry[(int)motion_frame] * mpf_vel + y_acc * mpf_hitvel;
		//x += (g_dirx[(int)motion_frame] * vel + x_acc * hit_vel ) * _game->getMpf(); 
		//y -= (g_diry[(int)motion_frame] * vel + y_acc * hit_vel ) * _game->getMpf();
	}


	/*
	 * Collision Test
	 */

	SDL_Rect rPrey;
	SDL_Rect rMine;
	CVehicle *ptr_veh = _game->Auto;

    GetFrameRect( &rMine );
	
	for ( Uint32 j = 0; j < _game->game_num_cars; j++ )
	{
		if ( j != myIndex )
		{
			if (collisions.check && collisions.disabled) {
				break;
			}

			// setup rectangles
			ptr_veh->GetFrameRect( &rPrey );
	
			//if ( _game->Sdl.Collide( NULL, &rMine, &rPrey ) )
			if ( _game->Sdl.Collide(&rMine, GetCurrentFrameMask(), &rPrey, ptr_veh->GetCurrentFrameMask()) )
			{
				DBG( "[COLLIDE] ----- New Collision [" << j << "] -----" );
				DBG("mpf_vel = " << mpf_vel << " mpf_hitvel = " << mpf_hitvel << " vmove=" << vmove);

				x = tmp_x;
				y = tmp_y;
				motion_frame = tmp_mf;
				display_frame = tmp_mf;

				if ( !fIsZero(hit_vel) )
				{
					DBG( "[COLLIDE] Step #2| hit_vel = " << hit_vel );
					ptr_veh->Repulse( (int)rep_frame, hit_vel / (float)ptr_veh->GetCompareVal() );
				}
				else
				{
					DBG( "[COLLIDE] Step #3" );

					// is velocity 0 ?
					if ( !fIsZero(vel) )
					{
						DBG( "[COLLIDE] Step #4| vel = " << vel );
						ptr_veh->Repulse( (int)motion_frame, vel / (float)ptr_veh->GetCompareVal() );
					}
					else
					{
						DBG( "[COLLIDE] Step #5" );
						/*if ( control == VC_AI || ptr_veh->GetControl() == VC_AI )
						{
							//Repulse( (int)GetMotionFrameMirror(), 100 );
							x = ptr_veh->GetX() + 50;
							y = ptr_veh->GetY() + 50;
							
						}*/
						//ptr_veh->Repulse( (int)motion_frame, 60 ); // !!!!!!!!! 
						//ptr_veh->SetVelocity( 0 );
						//no_damage = true;
					}
				}


				bool no_damage = false;

				// TODO: fix this because it disbalances the game!
				if ( abs_vel < MIN_DAMAGE_VELOCITY )
				{
					no_damage = true;
					DBG( "[COLLIDE] Step #6" );
				}

				// enemy vehicle damage calculations
				if ( !no_damage )
				{
//					anger = anger > MAX_ANGER ? MAX_ANGER : anger;

					float perc = abs_vel / (float)max_vel * 100.0f;
					int tmp_anger = ((float)anger / 100.0f ) * perc * (80.0f / damage);

					anger -= tmp_anger;
					anger = anger < 0 ? 0 : anger;
					
					float speed_damage = (float)damage + 0.12f * abs_vel;
					float armour_absorb = (0.10f * (float)ptr_veh->GetHitPoints());
					DBG( "[DODAMAGE] speed_damage=" << speed_damage << " armour_absorb=" << armour_absorb);

					int fulldmg = speed_damage - armour_absorb + tmp_anger;
					fulldmg = fulldmg < 0 ? 0 : fulldmg;
					DBG( "[DODAMAGE] fulldamage=" << fulldmg << " damage=" << damage << " anger=" << anger << " tmpanger=" << tmp_anger << " tmp_vel=" << tmp_vel << "  enemyHP=" << ptr_veh->GetHitPoints() );

					ptr_veh->DoDamage( fulldmg, myIndex );
				
					// PLAY RANDOM CRASH SOUND
					int sound = intGetRnd(0, 3);
					DBG("crash sound = " << sound);
					CONST_SOUNDS crash_sound;
					switch(sound) {
					case 0:
						crash_sound = SND_CRASHLIGHT1;
						break;
					case 1:
						crash_sound = SND_CRASHLIGHT2;
						break;
					case 2:
					default:
						crash_sound = SND_CRASHLIGHT3;
						break;
					}
					_game->Sounds.Play( crash_sound, (int)x );
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

					vel = vel * 0.5f;

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
					vel = vel * -0.6f;
				}

				DBG( "[COLLIDE] newvelocity=" << vel << " ACC is " << acc);
			}
		} 

		ptr_veh++;
	}

	// should we leave trails
	if ( trails_time > _game->Timer.Time() )
	{
		if ( tire_trails == VTT_BLOOD && vmove != VM_NONE )
		{
			if ( intGetRnd( 0, 1000 ) < 200 )
				_game->Anims.Create( GetCX(), GetCY(), ANIM_TIRESPLAT );
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
	
	// Game arena bounding

    if ( rMine.x < 24 )
	{
		x = 25;
		if ( abs(vel) > 50 ) 
		{
			if ( intGetRnd( 0, 50 ) > 25 ) 
				_game->Sdl.PlaySound( SND_TIRES1, rMine.x );
			else
				_game->Sdl.PlaySound( SND_TIRES2, rMine.x );
		}

		vel = 0;
	}
    else if ( rMine.w > 614 ) 
	{
		x = 613 - (rMine.w - x);
		if ( abs(vel) > 50 ) 
		{
			if ( intGetRnd( 0, 50 ) > 25 ) 
				_game->Sdl.PlaySound( SND_TIRES1, rMine.x );
			else
				_game->Sdl.PlaySound( SND_TIRES2, rMine.x );
		}

		vel = 0;
	}
    
	if ( y < 19) 
	{
		y = 20;
		if ( abs(vel) > 50 ) 
		{
			if ( intGetRnd( 0, 50 ) > 25 ) 
				_game->Sdl.PlaySound( SND_TIRES1, rMine.x );
			else
				_game->Sdl.PlaySound( SND_TIRES2, rMine.x );
		}

		vel = 0;
	}
    else if ( rMine.h > 400 ) 
	{
		y = 399 - (rMine.h - y);
		if ( abs(vel) > 50 ) 
		{
			if ( intGetRnd( 0, 50 ) > 25 ) 
				_game->Sdl.PlaySound( SND_TIRES1, rMine.x );
			else
				_game->Sdl.PlaySound( SND_TIRES2, rMine.x );
		}

		vel = 0;
	}
	
	/*
	 * Bonuses hit tests
	 */

	SDL_Rect rMe;
	SDL_Rect rToy;
	Uint32 dx = 0U;
	Uint32 dy = 0U;

	GetFrameRect( &rMe );

	for(int i = 0; i < DT_MAX_CHILDS; i++ )
	{
		if ( _game->Dtoys.GetToyVisible( i ) )
		{

		 _game->Dtoys.GetToyRect( i, &rToy );		

		 if ( _game->Sdl.Collide( NULL, &rMe, &rToy ) )
		 {
//			 if ( abs(vel) >= max_vel / 2 ) // do not hit if speed less than half the max
//			 {
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

					velocity_bonus = max_vel / 3;
					velocity_bonus_time = _game->Timer.Time() + SPEEDEXPIRE_TIME;
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
					
					if ( hit_points >= hit_points_crash && crashed_look )
					{
						sprite = sprite_norm;
						mask = mask_norm;
						crashed_look = false;
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
					if ( anger > MAX_ANGER )
						anger = MAX_ANGER;

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
					landmines_count += _game->Dtoys.GetToyValue( i );
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
//			 }
//			 else
//			 {
//				// speed is not high enough to smash this bonus
//				vel = 0;
//				x = tmp_x;
//				y = tmp_y;
//			 }
			 
		 }
		} // vis_check...
	} 

	/*
	 * landmines_count hit test
	 */
	for ( int i = 0; i < LANDMINE_CHILDS; i++ )
	{
		if ( _game->Mines.GetMineVisible( i ) && _game->Mines.GetMineIndex( i ) != myIndex)
		{
			_game->Mines.GetMineRect( i, &rToy );
			if ( _game->Sdl.Collide( NULL, &rMe, &rToy ) )
			{
				// do vehicle damage
				DoDamage( LANDMINE_DAMAGE, _game->Mines.GetMineIndex( i ) );
				_game->Mines.KillMine( i );
				_game->Sounds.Play( SND_EXPLOSION1, (int)x ); // PLAYSOUND
			}
		}
	}

}


///////////////////////////////////////////////////////////////////////
// Name: Repulse()
// Desc:
///////////////////////////////////////////////////////////////////////
void CVehicle::Repulse( int frame_angle, float speed )
{
//	motion_frame = (float)frame_angle;
	rep_frame = (float)frame_angle;
	hit_vel = speed;
	x_acc = g_dirx[frame_angle];
	y_acc = g_diry[frame_angle];
	//vel = vel + (float)hit_vel;

	if ( control == VC_AI ) 
		waypoint.do_precalculate = true;
}

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


///////////////////////////////////////////////////////////////////////
// Name: GetMotionFrameMirror()
// Desc: Get an opposite (to the current) movement angle.
///////////////////////////////////////////////////////////////////////
float CVehicle::GetMotionFrameMirror()
{
	float tmp_frame;

	tmp_frame = motion_frame;

	tmp_frame += HALF_ROTATION_FRAMES;
	if ( tmp_frame > MAX_ROTATION_FRAMES )
		tmp_frame -= MAX_ROTATION_FRAMES;
	
	return tmp_frame;
}


///////////////////////////////////////////////////////////////////////
// Name: DoDamage()
// Desc: Substract given damage from vehicle HP.
//       Destroys vehicle if HP is < 0.
///////////////////////////////////////////////////////////////////////
void CVehicle::DoDamage( int damageAmount, Uint32 attackerIndex  )
{
	hit_points -= damageAmount;
	if ( hit_points <= 0 )
	{
		visible = false;

		_game->Anims.Create( GetCX(), GetCY(), ANIM_EXPLOSION );

		// PLAYSOUND
		_game->Sounds.Play( SND_EXPLOSION1, (int)x );

		// give/lose a frag...
		if ( attackerIndex == myIndex )
		{
			if ( i_self_destruct )
			{
				i_self_destruct = false;
				frags--;
				if ( frags < 0 ) frags = 0;
			}
		}
		else if ( attackerIndex != NO_ATTACKER && _game->Auto[attackerIndex].GetTeam() != team )
		{
			_game->Auto[attackerIndex].AddFrags( 1 );
		}

		// in case this vehicle held 'the goal'
		if ( has_the_goal )
		{
			_game->Dtoys.SetToyGoalState( DTG_ONTHEROAD );
			has_the_goal = false;
		}
	}
	else if ( hit_points <= hit_points_crash && !crashed_look )
	{
		// show smashed vehicle sprites
		sprite = sprite_crash;
		mask = mask_crash;
		crashed_look = true;
	}
}


///////////////////////////////////////////////////////////////////////
// Name: Update()
// Desc: Update vehicle movement, collisions and animations
///////////////////////////////////////////////////////////////////////
void CVehicle::Update()
{
	// respawn vehicle (if not visible, e.g., destroyed)
	if (!visible) {
		Create();
		//return;
	}

	// recalculate vehicle center pos given current surface frame

	Uint32 *cur_mask = GetCurrentFrameMask();
	SDL_Surface *surf = sprite[(int) display_frame + MAX_ROTATION_FRAMES * (int) tire_frame];

	Uint32 width = surf->w;
	Uint32 height = surf->h;
	center_x = x + (width >> 1);
	center_y = y + (height >> 1);

	// Physics calculations
	DoMotion();
	//if ( control != VC_AI ) DoMotion();

	width = surf->w;
	height = surf->h;
	center_x = x + (width >> 1);
	center_y = y + (height >> 1);

	SDL_Rect rect = {0, 0, width, height};

	/*
	 * Update bonuses states
	 */

	// expire 1 anger point
	if (anger_time < _game->Timer.Time() && anger > 0) {
		anger--;
		anger_time = _game->Timer.Time() + ANGEREXPIRE_TIME;
	}
	// kill velocity_bonus
	if (velocity_bonus_time < _game->Timer.Time() && velocity_bonus != 0) {
		velocity_bonus = 0;
	}
	// expire goal
	if (goal_time < _game->Timer.Time() && has_the_goal) {
		// give 3 frags and put bonus back on map
		has_the_goal = false;
		_game->Dtoys.SetToyGoalState(DTG_ONTHEROAD);
		frags += 1;
	}

	/*
	 * Display vehicle
	 */

	// shadow & sprite
	//_game->Sdl.BlitShadow( (int)x + 1, (int)y + 4, surf );
	_game->Sdl.BlitShadow(x + 1.0f, y + 4.0f, cur_mask, &rect);
	_game->Sdl.AddToBlit(x, y, surf);

	// status, hp, frags, etc.
	char buf[8];
	sprintf(buf, "%d", frags);
	float perc = ((float) hit_points / (float) max_hitpoints) * 100.0f;

	_game->Sdl.DrawNum(pos_frag[myIndex].x, pos_frag[myIndex].y, buf);
	_game->scales[0]->w = (Uint32) ( /*(130.0f / 100.0f )*/1.3f * perc);
	_game->Sdl.BlitNow(pos_hp[myIndex].x, pos_hp[myIndex].y, _game->scales[0]);
	// bound anger value to match surface width of 130px
	_game->scales[1]->w = RangeGetXY(anger, 0, MAX_ANGER, 0, 130);
	_game->Sdl.BlitNow(pos_anger[myIndex].x, pos_anger[myIndex].y,_game->scales[1]);

	switch (control) {

	case VC_PLAYER1:

		if (_game->Sdl.IsKeyPressed(_game->Bindings.GetP1Key(CBindings::BK_LEFT)))
			Rotate(VR_LEFT);

		if (_game->Sdl.IsKeyPressed(_game->Bindings.GetP1Key(CBindings::BK_RIGHT)))
			Rotate(VR_RIGHT);

		if (_game->Sdl.IsKeyPressed(_game->Bindings.GetP1Key(CBindings::BK_ACC)))
			Move(VM_FORWARD);

		if (_game->Sdl.IsKeyPressed(_game->Bindings.GetP1Key(CBindings::BK_BREAK)))
			Move(VM_BACKWARD);

		if (_game->Sdl.IsKeyPressed(_game->Bindings.GetP1Key(CBindings::BK_HONK))) {
			honk_status = 1;
		} else {
			honk_status = honk_status == 1 ? 2 : 0;
		}

		if (_game->Sdl.IsKeyPressed(_game->Bindings.GetP1Key(CBindings::BK_BLOWUP))) {
			self_destruct = true;
			i_self_destruct = true;
		}

		//{!}
		if (_game->Sdl.IsJoystickHatState(SDL_HAT_LEFT))
			Rotate(VR_LEFT);
		if (_game->Sdl.IsJoystickHatState(SDL_HAT_RIGHT))
			Rotate(VR_RIGHT);
		if (_game->Sdl.GetJoystickButtonPressed(1))
			Move(VM_FORWARD);
		if (_game->Sdl.GetJoystickButtonPressed(2))
			Move(VM_BACKWARD);

		if (_game->Sdl.IsKeyPressed(_game->Bindings.GetP1Key(CBindings::BK_MINE))) {
			if (!bputmine)
				bputminekey = true;
		} else if (!_game->Sdl.IsKeyPressed(_game->Bindings.GetP1Key(CBindings::BK_MINE))) {
			bputminekey = false;
			bputmine = false;
		}

		break;

	case VC_PLAYER2:

		if (_game->Sdl.IsKeyPressed(_game->Bindings.GetP2Key(CBindings::BK_LEFT)))
			Rotate(VR_LEFT);

		if (_game->Sdl.IsKeyPressed(_game->Bindings.GetP2Key(CBindings::BK_RIGHT)))
			Rotate(VR_RIGHT);

		if (_game->Sdl.IsKeyPressed(_game->Bindings.GetP2Key(CBindings::BK_ACC)))
			Move(VM_FORWARD);

		if (_game->Sdl.IsKeyPressed(_game->Bindings.GetP2Key(CBindings::BK_BREAK)))
			Move(VM_BACKWARD);

		if (_game->Sdl.IsKeyPressed(_game->Bindings.GetP2Key(CBindings::BK_BLOWUP))) {
			self_destruct = true;
			i_self_destruct = true;
		}

		if (_game->Sdl.IsKeyPressed(_game->Bindings.GetP2Key(CBindings::BK_HONK))) {
			honk_status = 1;
		} else {
			honk_status = honk_status == 1 ? 2 : 0;
		}

		if (_game->Sdl.IsKeyPressed(_game->Bindings.GetP2Key(CBindings::BK_MINE))) {
			if (!bputmine)
				bputminekey = true;
		} else if (!_game->Sdl.IsKeyPressed(_game->Bindings.GetP2Key(CBindings::BK_MINE))) {
			bputminekey = false;
			bputmine = false;
		}

		break;

		// case VC_PLAYER3:
		// 	if ( _game->Sdl.IsKeyPressed(SDLK_j] ) Rotate( VR_LEFT );
		//if ( _game->Sdl.IsKeyPressed(SDLK_l] ) Rotate( VR_RIGHT );
		//if ( _game->Sdl.IsKeyPressed(SDLK_i] ) Move( VM_FORWARD );
		//if ( _game->Sdl.IsKeyPressed(SDLK_k] ) Move( VM_BACKWARD );
		// break;

	case VC_AI:
		AI_Update();

		if (ai_putmine) {
			ai_putmine = false;
			bputminekey = true;
		}

		break;

	default:
		break;

	}

	// put a landime mine down?
	if (bputminekey)
	{
		bputmine = true;
		if (landmines_count > 0)
		{
			landmines_count--;
			_game->Mines.Create(GetCX(), GetCY(), myIndex);
		}

		bputminekey = false;
	}

	// beep
	if (honk_status == 2)
	{
		_game->Sounds.Play(SND_MENU_HONK1, (int) x);
		honk_status = 0;
	}

	// self destruction if vehicle does not move/has 'stuck'
	if ((int)vel == 0 && !self_destruct)
	{
		if (!ai_stuck)
		{
			ai_stuck = true;
			ai_stucktime = _game->Timer.Time() + SELF_DESTRUCT_CHECK_TIME;
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
			self_destruct = true;
			ai_stuck = false;
		}
	}

	// check if sself destruction is on
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
			if (warning_time < _game->Timer.Time())
			{
				/*
				 * 1176sec - that's how long the warning beep sound is.
				 * Normally we would like to play this sound looped and stop it after
				 * the vehicle is destroyed. However, currently there is no Stop() call
				 * implemented in the CSounds class.
				 */
				warning_time = _game->Timer.Time() + 1176;
				_game->Sounds.Play(SND_WARNING, (int) x);
			}
		}
		else
		{
			// destroy vehicle and clear flag
			DoDamage(9999U, myIndex);
			self_destruction = false;
		}
	}

	// XXX
	// This is a hack to prevent vehicles from sticking (for too long)
	// If position does not change for several seconds, then just disable collisions check
	// for this vehicle. Let it pass through enemies.
	if (collisions.check)
	{
		if (collisions.disabled)
		{
			if (collisions.disabled_time < _game->Timer.Time())
			{
				collisions.disabled = false;
				collisions.last_check_time = 0;
			}
		}
		else if (collisions.last_check_time < _game->Timer.Time())
		{
			float dist = fGetDistanceNSR(x, y, collisions.last_x, collisions.last_y);
			if (dist < 4.0f)
			{
				DBG(" clipping DISABLED");
				collisions.disabled = true;
				collisions.disabled_time = _game->Timer.Time() + 2500;

				// XXX either this or leave to go to the waypoint selected
				if (control == VC_AI)
				{
					waypoint.do_reverse = true;
					waypoint.do_reverseTime = _game->Timer.Time() + 500;
				}
			}
			else
			{
				// reset check
				collisions.last_x = x;
				collisions.last_y = y;
				collisions.last_check_time = _game->Timer.Time() + 2000;
			}
		}
	}


} 

// TODO: Remove
//void CVehicle::UpdateStops()
//{
//	if ( set_stop )
//	{
//		SetVelocity( 0.0f );
//		set_stop = false;
//		// XXX
//		hit_vel = 0.0f;
//	}
//}
//

///////////////////////////////////////////////////////////////////////
// Name: AI_Update()
// Desc: Update waypoints and general AI decision taking process
///////////////////////////////////////////////////////////////////////
void CVehicle::AI_Update()
{
	// This will be called if AI has hit someone.
	// Then we need to move the AI controlled vehicle back for a defined period of time
	if ( waypoint.do_reverse )
	{
		Move( VM_BACKWARD );

		// 25% chacne to put a landmine when moving backwards
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

	// check if bonus is still on map
	if ( waypoint.goal_type == WAYPOINT_BONUS )
	{
		if ( !_game->Dtoys.GetToyVisible( waypoint.index ) )
			waypoint.reached = true;
	}
	else if ( waypoint.goal_type == WAYPOINT_VEHICLE )
	{
		// check if enemy target is still on map
		if ( !_game->Auto[waypoint.index].GetVisible() )
			waypoint.reached = true;
	}

	// check if waypoint has been reached
	float distance = fGetDistanceNSR( GetCX(), GetCY(), waypoint.x, waypoint.y );
	if ( distance <= WAYPOINT_RADIUS ) 
		waypoint.reached = true;

	// get a new waypoint
	if ( waypoint.reached )
	{
		AI_GenerateWaypoint();
		return;
	}
	
	// we need to recalculate the destination every now and then
	if ( !waypoint.static_pos || waypoint.do_precalculate )
	{
		if ( waypoint.goal_type == WAYPOINT_VEHICLE )
		{
			waypoint.x = (float)_game->Auto[waypoint.index].GetCX();
			waypoint.y = (float)_game->Auto[waypoint.index].GetCY();
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
	Uint32	  bonus_list_index[DT_MAX_CHILDS];
	Uint32	  bonus_list[DT_MAX_DEADTOYS];
	Uint32	  bonus_list_warrior[DT_MAX_DEADTOYS]	= { 30, 25, 15, 10, 20 };
	Uint32	  bonus_list_explorer[DT_MAX_DEADTOYS]	= { 45, 15, 25, 12, 8 };
	CVehicle  *ptr_veh = NULL;
	
	memset( bonus_list_index, 0U, DT_MAX_CHILDS * sizeof(Uint32) );
	memset( bonus_list, 0U, DT_MAX_DEADTOYS * sizeof(Uint32) );

	if ( avt == AVT_WARRIOR )				 // TYPE _WARRIOR
	{
		if ( intGetRnd( 0, 1000 ) > 500 )
			action = ACTION_ATTACK;
		else if ( intGetRnd( 0, 1000 ) < 250 )
			action = ACTION_DROPMINE;
		else
		{
			action = ACTION_TAKEBONUS;

			// check for available bonuses on map and create a list
			for( i = 0; i < DT_MAX_CHILDS; i++ )
			{
				if ( _game->Dtoys.GetToyVisible( i ) )
				{
					bonus_list[_game->Dtoys.GetToyKind( i )] = bonus_list_warrior[_game->Dtoys.GetToyKind( i )];
					bonus_list_index[_game->Dtoys.GetToyKind( i )] = i;
					bonus_found = true;
				}
			}

			// in case no bonuses are available
			if ( !bonus_found )
				action = ACTION_ATTACK;
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


	SDL_Rect  rBonus;

	switch( action )
	{
	case ACTION_ATTACK:
		
		// distance to closest enemy vehicle
		i = car_index = 0;
		distance = UINT_MAX;
		
		ptr_veh = _game->Auto;

		while( i < _game->game_num_cars )
		{
			if ( i != myIndex )
			{
				// Get nearest (non-team) car
				
				//tmp_dist = GetDistance( GetX(), GetY(), ptr_veh->GetX(), ptr_veh->GetY() );
				tmp_dist = GetDistanceNSR( GetCX(), GetCY(), ptr_veh->GetCX(), ptr_veh->GetCY() );
				
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
		
		// fill-in new waypoint information
		ptr_veh = &_game->Auto[car_index];
		
		waypoint.x				 = ptr_veh->GetCX();
		waypoint.y				 = ptr_veh->GetCY();
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
		
		// take random bonus from the list
		bonus_index = bonus_list_index[AI_doFSM( bonus_list, DT_MAX_DEADTOYS )];

		_game->Dtoys.GetToyRect( bonus_index, &rBonus );
		
		waypoint.x = rBonus.x;
		waypoint.y = rBonus.y;
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

	// Adjust waypoint coordinates
	AI_ProcessWaypoint();
}


///////////////////////////////////////////////////////////////////////
// Name: AI_ProcessWaypoint()
// Desc: Get angle to waypoint destination and adjust turning
///////////////////////////////////////////////////////////////////////
void CVehicle::AI_ProcessWaypoint()
{
	// get angle to the next waypoint
	float x_dist = (waypoint.x - x);
	float y_dist = (waypoint.y - y);
		
	if ( y_dist > 0 ) 
	{
		ai_dest_angle = (float)(atan( x_dist / y_dist ) - PI_2);
	}
	else if ( y_dist < 0 )
	{
		//st_angle = atan2( x_dist, y_dist );// + PI_2;
		ai_dest_angle = (float)(atan( x_dist / y_dist ) + PI_2);
	}
	else
	{
		ai_dest_angle = (float)(atan( x_dist /y_dist ) - PI_2);
	}

	// check if display frame must be recalculated
	if ( fabsf(ai_dest_angle - ai_cur_angle) < 0.1f )
	{
		ai_turning = VR_NONE;
		return;
	}

	// check where to rotate to
	float tmp_dest = ai_dest_angle;
	float tmp_cur = ai_cur_angle;
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

	/*// calculate radial speed
	distance = fGetDistance( (float)GetX(), (float)GetY(), waypoint.x, waypoint.y );
	ai_maxvel = sqrt( acc * distance );  // a = (v*v)/R
	*/

	//-------display _frame
	float la = ai_dest_angle * RAD1;
	if ( la < 0.0f )
	{
		la += 360.0f;
	}
	
	if ( la > 360.0f )
		la -= 90.0f;
	
	ai_final_frame = la * 0.1f; //la / 10;

	//display_frame = la/10;
	//motion_frame = display_frame;*/
}


///////////////////////////////////////////////////////////////////////
// Name: AI_doFSM()
// Desc: Use (F)inite-(S)tate-(M)achines to choose an action
///////////////////////////////////////////////////////////////////////
int CVehicle::AI_doFSM( Uint32 *proActions, Uint32 max_actions )
{
	int index = 0U;
	int i = 0U;
	int j = 0U;
	int val = 0;
	int tmp_val = 0;
	bool found  = false;

	// get a rnd num
	while ( val == 0 || val < 10 || !found )
	{
		val = rand() % 100;
		
		// at least one value that is higher than rnd
		for ( i = 0; i < max_actions; i++ )
		{
			if ( val > proActions[i] && proActions[i] != 0 )
			{
				found = true;
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

int CVehicle::GetAttributeAcceleration(int index)
{
	return attribACC[index];
}

int CVehicle::GetAttributeSpeed(int index)
{
	return attribSPEED[index];
}

int CVehicle::GetAttributeRotation(int index)
{
	return attribROT[index];
}

int CVehicle::GetAttributeDamage(int index)
{
	return attribDAMAGE[index];
}

int CVehicle::GetAttributeArmour(int index)
{
	return attribARMOUR[index];
}
