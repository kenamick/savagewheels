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


#include "main.h"

#define BUILD_PATH "build"

int create_swv_module(SWV_HEADER *swv)
{
	int 		ret = 0;
	char		swv_path[64];
	char		buf[255];
	FILE 		*fgd = NULL;
	CSwv_module   	CSwvm;

	if ( swv == NULL )
	  return ret;
	
	memset(swv_path, NULL, sizeof(char) * 64);
	strncpy( swv_path, swv->filename, strlen(swv->filename) - 4 ); // -".swv"

	// fill file array
	swv->pfiles = new SWV_FILES[swv->animation_frames * 2 + 3];

	// car_face
	sprintf ( swv->pfiles[0].filename, "build/%s/face.bmp", swv_path );
	if (  (fgd = fopen( swv->pfiles[0].filename, "rb" )) == NULL )
	{
		cout << "\n\nError opening face " <<  swv->pfiles[0].filename;
		goto swv_cleanup;
	}
	swv->pfiles[0].length = getfilesize( fgd );
	fclose( fgd );


	// driver_face
	sprintf ( swv->pfiles[1].filename, "build/%s/dface.bmp", swv_path );
	if (  (fgd = fopen( swv->pfiles[1].filename, "rb" )) == NULL )
	{
		cout << "\n\nError opening driver_face.bmp" << swv->pfiles[1].filename;
		goto swv_cleanup;
	}
	swv->pfiles[1].length = getfilesize( fgd );
	fclose( fgd );


	// car_name
	sprintf ( swv->pfiles[2].filename, "build/%s/name.bmp", swv_path );
	if (  (fgd = fopen( swv->pfiles[2].filename, "rb" )) == NULL )
	{
		cout << "\n\nError opening name.bmp" << swv->pfiles[2].filename;
		goto swv_cleanup;
	}
	swv->pfiles[2].length = getfilesize( fgd );
	fclose( fgd );

	
	for ( int i = 0; i < swv->animation_frames; i++ )
	{
		// normal faces
		sprintf( buf, "build/%s/ok/%d.bmp", swv_path, i );
		sprintf( swv->pfiles[i+3].filename, "%s", buf );
		
		if (  (fgd = fopen( buf, "rb" )) == NULL )
		{
			cout << "\n\nError opening frame " << swv->pfiles[i+3].filename;
			goto swv_cleanup;
		}
		swv->pfiles[i+3].length = getfilesize( fgd );
		fclose( fgd );

		
		// crash faces
		sprintf( buf, "build/%s/crash/%d.bmp", swv_path, i );
		sprintf( swv->pfiles[i + 3 + swv->animation_frames].filename, "%s", buf );

		if (  (fgd = fopen( buf, "rb" )) == NULL )
		{
			cout << "\n\nError opening crash frame " << swv->pfiles[i + 3 + swv->animation_frames].filename;
			goto swv_cleanup;
		}
		swv->pfiles[i + 3 + swv->animation_frames].length = getfilesize( fgd );
		fclose( fgd );

	}
		
	//sprintf( swv->filename, carname );
	if ( CSwvm.Create( BUILD_PATH, swv ) != SWV_SUCCESS ) 
	{
		cout << "\n\nFailed to create SWV module " << swv->filename << " !";
		goto swv_cleanup;
	}
	
	ret = 1;

	// free mem
swv_cleanup:

	if ( swv->pfiles )
	{
		delete[] swv->pfiles;
		swv->pfiles = NULL;
	}
	
	CSwvm.Release();

	if ( ret )
	{
		cout << "\nCreating " << swv->filename << " [ok]" ;
	}
	else
	{
		cout << "\nCreating " << swv->filename << " [error] !" ;
	}

	return ret;
}

int populate_swv_module(CONST_VEHICLES vehicle, SWV_HEADER *phdr)
{
  switch( vehicle )
  {
    case CV_BMW:
	phdr->acc = VACC_FAST;
	phdr->max_vel = VSPEED_FAST;
	phdr->dec_acc = 50;
	phdr->rot_speed = VRS_FAST;
	phdr->lbs = 1;
	phdr->hp = VARMOUR_MEDIUM;
	phdr->hp_crash = 2;
	phdr->damage = VDMG_MEDIUM;
	phdr->animation_frames = 36;
	sprintf( phdr->filename, "bmw.swv" );
	sprintf( phdr->vehiclename, "BMW" );      
      break;
      
    case CV_DIABLO:
	phdr->acc = VACC_FAST;
	phdr->max_vel = VSPEED_FAST;
	phdr->dec_acc = 50;
	phdr->rot_speed = VRS_FAST;
	phdr->lbs = 1;
	phdr->hp = VARMOUR_MEDIUM;
	phdr->hp_crash = 2;
	phdr->damage = VDMG_MEDIUM;
	phdr->animation_frames = 36;
	sprintf( phdr->filename, "diablo.swv" );
	sprintf( phdr->vehiclename, "DIABLO" );      
      break;
      
    case CV_DJUGAN:
	phdr->acc = VACC_SLOW;
	phdr->max_vel = VSPEED_SLOW;
	phdr->dec_acc = 50;
	phdr->rot_speed = VRS_FAST;
	phdr->lbs = 2;
	phdr->hp = VARMOUR_MEDIUM;
	phdr->hp_crash = 2;
	phdr->damage = VDMG_HEAVY;
	phdr->animation_frames = 36;
	sprintf( phdr->filename, "djugan.swv" );
	sprintf( phdr->vehiclename, "DJUGAN" );      
      break;
      
    case CV_DODGE:
	phdr->acc = VACC_VERYFAST;
	phdr->max_vel = VSPEED_VFAST;
	phdr->dec_acc = 50;
	phdr->rot_speed = VRS_FAST;
	phdr->lbs = 1;
	phdr->hp = VARMOUR_MEDIUM;
	phdr->hp_crash = 2;
	phdr->damage = VDMG_LIGHT;
	phdr->animation_frames = 36;
	sprintf( phdr->filename, "dodge.swv" );
	sprintf( phdr->vehiclename, "DODGE" );      
      break;
      
    case CV_DTRUCK:
	phdr->acc = VACC_SLOW;
	phdr->max_vel = VSPEED_VSLOW;
	phdr->dec_acc = 50;
	phdr->rot_speed = VRS_SLOW;
	phdr->lbs = 3;
	phdr->hp = VARMOUR_ENORMOUS;
	phdr->hp_crash = 2;
	phdr->damage = VDMG_ENORMOUS;
	phdr->animation_frames = 72;
	sprintf( phdr->filename, "truck.swv" );
	sprintf( phdr->vehiclename, "DTRUCK" );      
      break;
      
    case CV_F1:
	phdr->acc = VACC_VERYFAST;
	phdr->max_vel = VSPEED_BLAZE;
	phdr->dec_acc = 50;
	phdr->rot_speed = VRS_FAST;
	phdr->lbs = 1;
	phdr->hp = VARMOUR_LIGHT;
	phdr->hp_crash = 2;
	phdr->damage = VDMG_LIGHT;
	phdr->animation_frames = 36;
	sprintf( phdr->filename, "f1.swv" );
	sprintf( phdr->vehiclename, "F1" );      
      break;
      
    case CV_JEEP:
	phdr->acc = VACC_FAST;
	phdr->max_vel = VSPEED_FAST;
	phdr->dec_acc = 50;
	phdr->rot_speed = VRS_SLOW;
	phdr->lbs = 2;
	phdr->hp = VARMOUR_HEAVY;
	phdr->hp_crash = 2;
	phdr->damage = VDMG_MEDIUM;
	phdr->animation_frames = 36;
	sprintf( phdr->filename, "jeep.swv" );
	sprintf( phdr->vehiclename, "JEEP" );      
      break;
      
    case CV_MERCEDES:
	phdr->acc = VACC_VERYFAST;
	phdr->max_vel = VSPEED_VFAST;
	phdr->dec_acc = 50;
	phdr->rot_speed = VRS_FAST;
	phdr->lbs = 1;
	phdr->hp = VARMOUR_MEDIUM;
	phdr->hp_crash = 2;
	phdr->damage = VDMG_LIGHT;
	phdr->animation_frames = 36;
	sprintf( phdr->filename, "merc.swv" );
	sprintf( phdr->vehiclename, "MERC" );      
      break;
      
    default:
      return 0;
      break;
  }
  
  return 1;
}

int test_car(CONST_VEHICLES vehicle)
{
  
	SWV_HEADER header;
	if ( !populate_swv_module( vehicle, &header) )
	  return 0;
    
	CSwv_module   	CSwvm;
	char		path[255];
	
	sprintf( path, "%s", BUILD_PATH );
	if ( SWV_SUCCESS != CSwvm.SearchAndLoad( path ) )
	{
	  cout << "Failed to load autos !";
	  return 0;
	}
	
	SWV_HEADER *phdr = &header;
	for( int i = 0; i < CSwvm.GetVehicles(); i++ )
	{
	  if ( ! strcmp( CSwvm.GetVehicleName(i), phdr->vehiclename ) )
	  {
	    cout << "--- File:   Original: ---" << std::endl;
	    
	    cout << "File: ";
	    cout << CSwvm.GetVehicleFilename(i);
	    cout << "  ";
	    cout << phdr->filename;
	    cout << std::endl;
	    
	    cout << "Name: ";
	    cout << CSwvm.GetVehicleName(i);
	    cout << "  ";
	    cout << phdr->vehiclename;
	    cout << std::endl;
	    
	    cout << "Acceleration: ";
	    cout << CSwvm.GetVehiclesData()[i].acc;
	    cout << "  ";
	    cout << phdr->acc;
	    cout << std::endl;	    
	    
	    cout << "Max Velocity: ";
	    cout << CSwvm.GetVehiclesData()[i].max_vel;
	    cout << "  ";
	    cout << phdr->max_vel;
	    cout << std::endl;	
	    
	    cout << "Acceleration Dec: ";
	    cout << CSwvm.GetVehiclesData()[i].dec_acc;
	    cout << "  ";
	    cout << phdr->dec_acc;
	    cout << std::endl;	
	    
	    cout << "Rotation speed: ";
	    cout << CSwvm.GetVehiclesData()[i].rot_speed;
	    cout << "  ";
	    cout << phdr->rot_speed;
	    cout << std::endl;	 
	    
	    cout << "LBS: ";
	    cout << CSwvm.GetVehiclesData()[i].lbs;
	    cout << "  ";
	    cout << phdr->lbs;
	    cout << std::endl;	
	    
	    cout << "HP: ";
	    cout << CSwvm.GetVehiclesData()[i].hp;
	    cout << "  ";
	    cout << phdr->hp;
	    cout << std::endl;		    
	    
	    cout << "HP Crash: ";
	    cout << CSwvm.GetVehiclesData()[i].hp_crash;
	    cout << "  ";
	    cout << phdr->hp_crash;
	    cout << std::endl;	
	    
	    cout << "Damage: ";
	    cout << CSwvm.GetVehiclesData()[i].damage;
	    cout << "  ";
	    cout << phdr->damage;
	    cout << std::endl;	
	    
	    cout << "Anim Frames: ";
	    cout << CSwvm.GetVehiclesData()[i].animation_frames;
	    cout << "  ";
	    cout << phdr->animation_frames;
	    cout << std::endl;		    
	  }
	}

	CSwvm.Release();
	
	return 1;
}

int32_t getfilesize( FILE *fp )
{
	fseek( fp, 0, SEEK_END );
	int32_t f_size = ftell( fp );
	return f_size;
}

int main(int argc, char* argv[])
{
	cout << "\nSavage Wheels SWV builder ver. " << SWV_MAJ_VERSION << "." << SWV_MIN_VERSION;
	cout << "\n\n";
	
	SWV_HEADER header;
	int vehicles[] = { CV_BMW, CV_DIABLO, CV_DJUGAN, CV_DODGE, CV_DTRUCK, CV_F1, CV_JEEP, CV_MERCEDES, -1 };

	bool build_vehicles = false;
	bool test_vehicles = false;
	if ( argc > 1 )
	{
		for( int i = 1; i < argc; i++ )
		{
			if ( !strcmp(argv[i], "test") )
			{
				test_vehicles = true;
			}
			else if ( !strcmp(argv[i], "build" ) )
			{
				build_vehicles = true;
			}
		}
	}
	else
	{
		cout << "Usage: swv_builder [build] [test]" << endl;
		cout << "\tbuild - build all modules" << endl;
		cout << "\ttest - tests already build modules against known values" << endl;
		cout << endl;
		return 0;
	}
	
	if ( build_vehicles )
	{
		// create modules
		cout << endl << "=== BUILDING ===" << endl << endl;
		for( int i = 0; vehicles[i] != -1; i++ )
		{
		  memset( &header, NULL, sizeof(SWV_HEADER) );
		  if ( populate_swv_module( static_cast<CONST_VEHICLES>(vehicles[i]), &header) )
			create_swv_module(&header);
		}
	}

	if ( test_vehicles )
	{
		// test created stuff
		cout << endl << "=== TESTING ===" << endl << endl;
		for( int i = 0; vehicles[i] != -1; i++ )
		  test_car(static_cast<CONST_VEHICLES>(vehicles[i]));
	}

	//cout << "Please, fill in vehicle info: \n";
	//cout << "speed (average-120;fast->260) : ";
	//cin	 >> swvh.max_vel;

	//cout << "acceleartion (very slow-4;fast->15) : ";
	//cin	 >> swvh.acc;

	//cout << "acceleartion_friction (usualy 1/2, 1/3 of acc) : ";
	//cin	 >> swvh.dec_acc;

	//cout << "rotation(turning) speed (average-15;fast-25) : ";
	//cin >> swvh.rot_speed;

	//cout << "weight (very_heavy-4;lite-1) : ";
	//cin >> swvh.lbs;

	//cout << "damage (it's up to u :P ) ";
	//cin >> swvh.damage;

	//cout << "hit_points : ";
	//cin >> swvh.hp;

	//cout << "hit_points crash (when to animate it crashed?!): ";
	//cin >> swvh.hp_crash;

	//cout << "animation_frames (36-no wheel rotation (*2 per frame): ";
	//cin >> swvh.animation_frames;

	return 0;
}

