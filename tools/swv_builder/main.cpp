// main.cpp -


#include "main.h"

CSwv_module   CSwvm;

int create_swv_module(SWV_HEADER *swv)
{
	int ret = 0;
	char buf[255];
	FILE *fgd = NULL;

	if ( swv == NULL )
		return ret;

	// fill file array
	swv->pfiles = new SWV_FILES[swv->animation_frames * 2 + 3];

	// car_face
	sprintf ( swv->pfiles[0].filename, "%s", "build/face.bmp" );
	if (  (fgd = fopen( swv->pfiles[0].filename, "rb" )) == NULL )
	{
		cout << "\n\nError opening " << buf;
		goto swv_cleanup;
	}
	swv->pfiles[0].length = getfilesize( fgd );
	fclose( fgd );


	// driver_face
	sprintf ( swv->pfiles[1].filename, "%s", "build/dface.bmp" );
	if (  (fgd = fopen( swv->pfiles[1].filename, "rb" )) == NULL )
	{
		cout << "\n\nError opening driver_face.bmp" << buf;
		goto swv_cleanup;
	}
	swv->pfiles[1].length = getfilesize( fgd );
	fclose( fgd );


	// car_name
	sprintf ( swv->pfiles[2].filename, "%s", "build/name.bmp" );
	if (  (fgd = fopen( swv->pfiles[2].filename, "rb" )) == NULL )
	{
		cout << "\n\nError opening name.bmp" << buf;
		goto swv_cleanup;
	}
	swv->pfiles[2].length = getfilesize( fgd );
	fclose( fgd );

	
	for ( int i = 0; i < swv->animation_frames; i++ )
	{
		// normal faces
		sprintf( buf, "build/ok/%d.bmp", i );
		sprintf( swv->pfiles[i+3].filename, "%s", buf );
		

		if (  (fgd = fopen( buf, "rb" )) == NULL )
		{
			cout << "\n\nError opening " << buf;
			goto swv_cleanup;
		}
		swv->pfiles[i+3].length = getfilesize( fgd );
		fclose( fgd );

		
		// crash faces
		sprintf( buf, "build/crash/%d.bmp", i );
		sprintf( swv->pfiles[i + 3 + swv->animation_frames].filename, "%s", buf );

		if (  (fgd = fopen( buf, "rb" )) == NULL )
		{
			cout << "\n\nError opening " << buf;
			goto swv_cleanup;
		}
		swv->pfiles[i + 3 + swv->animation_frames].length = getfilesize( fgd );
		fclose( fgd );

	}
		
	//sprintf( swv->filename, carname );
	if ( CSwvm.Create( swv ) != SWV_SUCCESS ) 
	{
		cout << "\n\nFailed to create SWV module " << swv->filename << " !" << buf;
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

	return ret;
}

void test_car(int idx)
{
	CSwvm.SearchAndLoad( "/" );

	cout << CSwvm.GetFacePos( idx );
	cout << "\n" << CSwvm.GetFaceSize( idx );
	cout << "\n" << CSwvm.GetFramePos( idx, 0 );
	cout << "\n" << CSwvm.GetFrameSize( idx, 0 );
	cout << "\n" << CSwvm.GetFramePos( idx, 1 );
	cout << "\n" << CSwvm.GetFrameSize( idx, 1 );

//	cout << "\n" << CSwvm.GetFile( car );

	int rt;
	cin >> rt;

	CSwvm.Release();
}

long getfilesize( FILE *fp )
{
	fseek( fp, 0, SEEK_END );
	long f_size = ftell( fp );
	return f_size;
}

int main(int argc, char* argv[])
{
	cout << "\n 'Savage Wheels' SWV Vehicles Module Creator ver. " << SWV_MAJ_VERSION << "." << SWV_MIN_VERSION;
	cout << "\n\n";

/*	cout << "Please, fill in vehicle info: \n";
	cout << "speed (average-120;fast->260) : ";
	cin	 >> swvh.max_vel;

	cout << "acceleartion (very slow-4;fast->15) : ";
	cin	 >> swvh.acc;

	cout << "acceleartion_friction (usualy 1/2, 1/3 of acc) : ";
	cin	 >> swvh.dec_acc;

	cout << "rotation(turning) speed (average-15;fast-25) : ";
	cin >> swvh.rot_speed;

	cout << "weight (very_heavy-4;lite-1) : ";
	cin >> swvh.lbs;

	cout << "damage (it's up to u :P ) ";
	cin >> swvh.damage;

	cout << "hit_points : ";
	cin >> swvh.hp;

	cout << "hit_points crash (when to animate it crashed?!): ";
	cin >> swvh.hp_crash;

	cout << "animation_frames (36-no wheel rotation (*2 per frame): ";
	cin >> swvh.animation_frames;
*/

	SWV_HEADER	  swvh;
	swvh.acc = VACC_SLOW;
	swvh.max_vel = VSPEED_VSLOW;
	swvh.dec_acc = 50;
	swvh.rot_speed = VRS_SLOW;
	swvh.lbs = 3;
	swvh.hp = VARMOUR_ENORMOUS;
	swvh.hp_crash = 2;
	swvh.damage = VDMG_ENORMOUS;
	swvh.animation_frames = 72;
	sprintf( swvh.filename, "truck.swv" );
	sprintf( swvh.vehiclename, "DTRUCK" );
	//...

	CSwvm.Release();

	return 0;
}

