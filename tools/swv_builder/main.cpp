// main.cpp -


#include "main.h"


CSwv_module   CSwvm;
SWV_HEADER	  swvh;



void main()
{

	char buf[255];
	char carname[255];
	FILE *fgd;
	int car = 2;
	

	goto savs;

	CSwvm.SearchAndLoad( "/" );


	cout << CSwvm.GetFacePos( car );
	cout << "\n" << CSwvm.GetFaceSize( car );
	cout << "\n" << CSwvm.GetFramePos( car, 0 );
	cout << "\n" << CSwvm.GetFrameSize( car, 0 );
	cout << "\n" << CSwvm.GetFramePos( car, 1 );
	cout << "\n" << CSwvm.GetFrameSize( car, 1 );

//	cout << "\n" << CSwvm.GetFile( car );


	int rt;
	cin >> rt;

	CSwvm.Release();
	exit(0);


	cout << "\n 'Savage Wheels' Vehicles Module Creator ver. " << SWV_MAJ_VERSION << "." << SWV_MIN_VERSION;
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
savs:
	swvh.acc = VACC_SLOW;
	swvh.max_vel = VSPEED_VSLOW;
	swvh.dec_acc = 50;
	swvh.rot_speed = VRS_SLOW;
	swvh.lbs = 3;
	swvh.hp = VARMOUR_ENORMOUS;
	swvh.hp_crash = 2;
	swvh.damage = VDMG_ENORMOUS;
	swvh.animation_frames = 72;
	sprintf( carname, "truck.swv" );
	sprintf( swvh.vehiclename, "DTRUCK" );


	// fill file array
	swvh.pfiles = new SWV_FILES[swvh.animation_frames*2+3];

	// car_face
	sprintf ( swvh.pfiles[0].filename, "%s", "build/face.bmp" );
	if (  (fgd = fopen( swvh.pfiles[0].filename, "rb" )) == NULL )
	{
		cout << "\n\nError opening " << buf;
		return;
	}
	swvh.pfiles[0].length = getfilesize( fgd );
	fclose( fgd );


	// driver_face
	sprintf ( swvh.pfiles[1].filename, "%s", "build/dface.bmp" );
	if (  (fgd = fopen( swvh.pfiles[1].filename, "rb" )) == NULL )
	{
		cout << "\n\nError opening driver_face.bmp" << buf;
		return;
	}
	swvh.pfiles[1].length = getfilesize( fgd );
	fclose( fgd );


	// car_name
	sprintf ( swvh.pfiles[2].filename, "%s", "build/name.bmp" );
	if (  (fgd = fopen( swvh.pfiles[2].filename, "rb" )) == NULL )
	{
		cout << "\n\nError opening name.bmp" << buf;
		return;
	}
	swvh.pfiles[2].length = getfilesize( fgd );
	fclose( fgd );

	
	for ( int i = 0; i < swvh.animation_frames; i++ )
	{
		// normal faces
		sprintf( buf, "build/ok/%d.bmp", i );
		sprintf( swvh.pfiles[i+3].filename, "%s", buf );
		

		if (  (fgd = fopen( buf, "rb" )) == NULL )
		{
			cout << "\n\nError opening " << buf;
			return;
		}
		swvh.pfiles[i+3].length = getfilesize( fgd );
		fclose( fgd );

		
		// crash faces
		sprintf( buf, "build/crash/%d.bmp", i );
		sprintf( swvh.pfiles[i + 3 + swvh.animation_frames].filename, "%s", buf );

		if (  (fgd = fopen( buf, "rb" )) == NULL )
		{
			cout << "\n\nError opening " << buf;
			return;
		}
		swvh.pfiles[i + 3 + swvh.animation_frames].length = getfilesize( fgd );
		fclose( fgd );

	}

		
	sprintf( swvh.filename, carname );
	if ( CSwvm.Create( &swvh ) != SWV_SUCCESS ) exit(-22);
	
	
	// free mem
	delete[] swvh.pfiles;


	//...
	CSwvm.Release();


	cout << endl << "Building successful..." << endl;

}


long getfilesize( FILE *fp )
{

	long f_size;

	fseek( fp, 0, SEEK_END );
	f_size = ftell( fp );

	return f_size;

}
