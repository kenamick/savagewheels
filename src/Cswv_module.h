// Cswm_module.h - 



#ifndef __SWV_MODULE
#define __SWV_MODULE


#define SWV_MAJ_VERSION			   1
#define SWV_MIN_VERSION		 	   0
#define SWV_SUCCESS			 	   0
#define SWVERROR_OPENSWVFILE	  -1
#define SWVERROR_OPENGFXFILE	  -2
#define SWVERROR_NOFILES		  -3
#define SWVERROR_INCORRECTHEADER  -4
 

#define MAX_VEHICLES			16


/***_vehicle_attributes_******/

enum CONST_VACC 
{
	VACC_SLOW = 0,
	VACC_FAST,
	VACC_VERYFAST
};

enum CONST_VSPEED 
{
	VSPEED_VSLOW = 0,
	VSPEED_SLOW,
	VSPEED_FAST,
	VSPEED_VFAST,
	VSPEED_BLAZE
};

enum CONST_VROTSPEED 
{
	VRS_SLOW = 0,
	VRS_FAST,
	VRS_VFAST
};

enum CONST_VDAMAGE
{
	VDMG_LIGHT = 0,
	VDMG_MEDIUM,
	VDMG_HEAVY,
	VDMG_ENORMOUS
};

enum CONST_VARMOUR
{
	VARMOUR_LIGHT = 0,
	VARMOUR_MEDIUM,
	VARMOUR_HEAVY,
	VARMOUR_ENORMOUS
};

/*****************************/

struct SWV_FILES {
	//long pos;
	//long length;
	unsigned char pos[4];
	unsigned char length[4];
	char filename[255];
};


struct SWV_HEADER {
	char		     header[3];
	char		     filename[64];
	char			 vehiclename[8];
	CONST_VSPEED     max_vel;
	CONST_VACC 	     acc;
	int			     dec_acc;
	CONST_VROTSPEED  rot_speed;
	int			     lbs;
	CONST_VDAMAGE    damage;
	CONST_VARMOUR    hp;
	int			     hp_crash;
	int			     animation_frames;
	SWV_FILES	     *pfiles;
};


class CSwv_module
{

private:

	//SWV_HEADER		  *vehicles;
	unsigned int      num_vehicles;
	unsigned long	  pos_face;		// title screen of the vehicle
	
private:
	int Load( char *filename, SWV_HEADER *swv_file ); 

public:
	CSwv_module();
	~CSwv_module();

	SWV_HEADER		  *vehicles;

	int  Create( SWV_HEADER *swv );
	int  SearchAndLoad( char *search_dir );
	void Release();
	unsigned int GetVehicles() { return num_vehicles; };

	/* File Positions
	   {!} Need to add a check for valid car_index
	*/

	char* GetVehicleFilename( unsigned int car_index ) { return vehicles[car_index].filename; };
	char* GetVehicleName( unsigned int car_index ) { return vehicles[car_index].vehiclename; };
	long GetFacePos( unsigned int car_index );
	long GetFaceSize( unsigned int car_index );
	long GetDriverFacePos( unsigned int car_index );
	long GetDriverFaceSize( unsigned int car_index );
	long GetNamePos( unsigned int car_index );
	long GetNameSize( unsigned int car_index );
	long GetFramePos( unsigned int car_index, int frame );
	long GetFrameSize( unsigned int car_index, int frame );

};




#endif