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

typedef struct SWV_FILES {
	long pos;
	long length;
	char filename[255];
}; 


typedef struct SWV_HEADER {
	char		  header[3];
	char		  filename[64];
	int			  max_vel;
	int			  acc;
	int			  dec_acc;
	int			  rot_speed;
	int			  lbs;
	unsigned int  damage;
	int			  hp;
	int			  hp_crash;
	int			  animation_frames;
	SWV_FILES	  *pfiles;
};


class CSwv_module
{

private:

	SWV_HEADER		  *vehicles;
	unsigned int      num_vehicles;
	unsigned long	  pos_face;		// title screen of the vehicle
	
private:
	int Load( char *filename, SWV_HEADER *swv_file ); 

public:
	CSwv_module();
	~CSwv_module();

	int  Create( SWV_HEADER *swv );
	int  SearchAndLoad( char *seacrh_dir );
	void Release();

	/* File Positions
	   {!} Need to add a check for valid car_index
	*/

	long GetFacePos( unsigned int car_index ) { return vehicles[car_index].pfiles[0].pos; };
	long GetFaceSize( unsigned int car_index ) { return vehicles[car_index].pfiles[0].length; };
	long GetFramePos( unsigned int car_index, int frame ) { return vehicles[car_index].pfiles[frame+1].pos; };
	long GetFrameSize( unsigned int car_index, int frame ) { return vehicles[car_index].pfiles[frame+1].length; };
	char* GetFile( int fnum ) { return vehicles[fnum].filename; };

};




#endif