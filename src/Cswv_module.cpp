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

#ifdef SWV_CREATE
#	include "Cswv_module.h"
#else
#	include "Main.h"
#endif

#ifdef LINUX_BUILD
#	include <dirent.h>
#	include <sys/stat.h>
#	include <stdio.h>
#else
#	include <io.h>
#endif


///////////////////////////////////////////////////////////////////////////////
//// Name: Cswv_modile()
//// Desc: class-constructor
///////////////////////////////////////////////////////////////////////////////
CSwv_module::CSwv_module()
:	num_vehicles( 0 ),
	pos_face( -1L ),
	vehicles(NULL)
{

}


///////////////////////////////////////////////////////////////////////////////
//// Name: ~Cswv_modile()
//// Desc: class-destructor
///////////////////////////////////////////////////////////////////////////////
CSwv_module::~CSwv_module()
{
  
  // clean up allocated memory
  if ( vehicles != NULL )
  {
    delete[] vehicles;
    vehicles = NULL;
  }

}


///////////////////////////////////////////////////////////////////////////////
//// Name: Release()
//// Desc: free loaded modules memory
///////////////////////////////////////////////////////////////////////////////
void CSwv_module::Release()
{
	if ( num_vehicles != 0 )
	{
		for ( unsigned int i = 0; i < num_vehicles; i++ )
		{
			delete[] vehicles[i].pfiles;
			vehicles[i].pfiles = NULL;
		}

		delete[] vehicles;
		vehicles = NULL;
	}

}



///////////////////////////////////////////////////////////////////////////////
//// Name: Create()
//// Desc: create a savagewheels-vehicles file from a header
///////////////////////////////////////////////////////////////////////////////
int CSwv_module::Create( const char *destPath, SWV_HEADER *swm )
{

	FILE *fp = NULL;
	FILE *fp_bmp = NULL;
	char header[3] = { 'S', 'W', 'V' };
	char *buffer = NULL;
	int32_t file_pos = -1;
	int  num_anims;
	char fullpath[255];
	
	
	if ( destPath )
	{
	  sprintf( fullpath, "%s/%s", destPath, swm->filename );
	}
	else
	{
	  sprintf( fullpath, "%s", swm->filename );
	}
	
	if ( ( fp = fopen( fullpath, "wb" ) ) == NULL )
		return SWVERROR_OPENSWVFILE;

	// write headers
	//sprintf( swm->header, "%s", "SWV" );
	strncpy( swm->header, header, 3 );
	//fwrite( header, sizeof(header)*3, 1, fp );
	fwrite( swm, sizeof(SWV_HEADER), 1, fp );

	// get number of animations ( +2 face.bmp dface.bmp )
	num_anims = swm->animation_frames * 2 + 3;
	
	file_pos = ftell( fp );
	fwrite( swm->pfiles, sizeof(SWV_FILES) * (num_anims), 1, fp ); 
	
	// put in bitmap data
	for ( int i = 0; i < num_anims; i++ )
	{
		// open bmp file
		if ( ( fp_bmp = fopen( swm->pfiles[i].filename, "rb" ) ) == NULL )
			return SWVERROR_OPENGFXFILE;

		// save position info in mother file
		swm->pfiles[i].pos = ftell( fp );
		
		// allocate memory & read its contents
		buffer = new char[swm->pfiles[i].length];
		fread( buffer, swm->pfiles[i].length, 1, fp_bmp );

		// send data to SWV_m
		fwrite( buffer, swm->pfiles[i].length, 1, fp );

		delete[] buffer;

		fclose( fp_bmp );

	}

	// write back file positions
	fseek( fp, file_pos, SEEK_SET );
	fwrite( swm->pfiles, sizeof(SWV_FILES) * (num_anims), 1, fp ); 

	fclose( fp );

	return SWV_SUCCESS;
}




///////////////////////////////////////////////////////////////////////////////
//// Name: Load()
//// Desc: loads a SWV file into the vehicles array
///////////////////////////////////////////////////////////////////////////////
int CSwv_module::Load( char *filename, SWV_HEADER *swv_file )
{
	FILE *fp = NULL;
	
	if ( ( fp = fopen( filename, "rb" ) ) == NULL )
		return SWVERROR_OPENSWVFILE;

	// read header
	//CBufferedReader reader(fp);

	//reader.readCharArray( swv_file->header, 3 );
	fread( swv_file, sizeof(SWV_HEADER), 1, fp );
	if ( swv_file->header[0] != 'S' && swv_file->header[1] != 'W' && swv_file->header[2] != 'V' ) 
		return SWVERROR_INCORRECTHEADER;
	
	// read elements
// 	reader.readCharArray( swv_file->filename, 64 );
// 	reader.readCharArray( swv_file->vehiclename, 8 );
// 	swv_file->max_vel = static_cast<CONST_VSPEED>(reader.readInt16());
// 	swv_file->acc = static_cast<CONST_VACC>(reader.readInt16());
// 	swv_file->dec_acc = static_cast<CONST_VACC>(reader.readInt16());
// 	swv_file->rot_speed = static_cast<CONST_VROTSPEED>(reader.readInt16());
// 	swv_file->lbs = reader.readInt16();
// 	swv_file->damage = static_cast<CONST_VDAMAGE>(reader.readInt16());
// 	swv_file->hp = static_cast<CONST_VARMOUR>(reader.readInt16());
// 	swv_file->hp_crash = reader.readInt16();
// 	swv_file->animation_frames = reader.readInt16();
	
	// read file_positions
	int num_anims = swv_file->animation_frames * 2 + 3; // (+3 face.bmp dface.bmp, name.bmp)
	swv_file->pfiles = new SWV_FILES[num_anims];
	fread( swv_file->pfiles, sizeof(SWV_FILES) * (num_anims), 1, fp ); 

// 	for ( int i = 0; i < num_anims; i ++ )
// 	{
// 	  swv_file->pfiles[i].pos = reader.readInt64();
// 	  swv_file->pfiles[i].length = reader.readInt64();
// 	  reader.readCharArray(swv_file->pfiles[i].filename, 255 );
// 	}
	
	// copy module_filename
	sprintf( swv_file->filename, "%s", filename );

	if ( fp != NULL )
	  fclose( fp );

	return SWV_SUCCESS;
}



///////////////////////////////////////////////////////////////////////////////
//// Name: SearchAndLoad()
//// Desc: scans a folder for SWV files and loads them all
///////////////////////////////////////////////////////////////////////////////
int CSwv_module::SearchAndLoad( char *search_dir )
{
#ifdef LINUX_BUILD
	struct stat file_stat;
	struct dirent *pDirEntry = NULL;
	DIR	*pDir = NULL;	
#else
	struct _finddata_t  ffile;
	long   hFile;
#endif

	char   buf[255];
	
	// clear array from previous loadings
	if ( num_vehicles != 0 )
		delete[] vehicles;
		
	vehicles = new SWV_HEADER[16];

#ifndef LINUX_BUILD
	sprintf( buf, "%s/%s", search_dir, "*.swv" );

	if ( ( hFile = _findfirst( buf, &ffile ) ) != -1L )
#else
	pDir = opendir( search_dir );
	if ( pDir != NULL )
#endif
	{

#ifndef LINUX_BUILD
		sprintf( buf, "%s/%s", search_dir, ffile.name );

		//AppendToMultiLog( "Loading ...", buf, NULL );
		memset( &vehicles[num_vehicles], 0, sizeof(SWV_HEADER) );

		if ( Load( buf, &vehicles[num_vehicles] ) != SWV_SUCCESS )
			return SWVERROR_OPENSWVFILE;

		num_vehicles++;

		while ( _findnext( hFile, &ffile ) == 0 )
		{
			sprintf( buf, "%s/%s", search_dir, ffile.name );
			if ( Load( buf, &vehicles[num_vehicles] ) == SWV_SUCCESS )
				num_vehicles++;
		}
#else

		pDirEntry = readdir( pDir );
		while( pDirEntry != NULL )
		{
			if ( strcmp( pDirEntry->d_name, "." ) !=0 && strcmp( pDirEntry->d_name, ".." ) != 0 )
			{
				sprintf( buf, "%s/%s", search_dir, pDirEntry->d_name );

				#ifdef __DEBUG
					LOG( "[SWV] loading: " << buf );
				#endif
				
				if ( 0 == stat( buf, &file_stat ) )
				{
					if ( ! S_ISDIR( file_stat.st_mode ) )
					{
						sprintf( buf, "%s/%s", search_dir, pDirEntry->d_name );
						if ( Load( buf, &vehicles[num_vehicles] ) == SWV_SUCCESS )
							num_vehicles++;
					}
				}
			}

			pDirEntry = readdir( pDir );
		}
#ifdef __DEBUG
	sprintf( buf, "%d", num_vehicles );
	LOG( "[SWV] found modules=" << buf );
#endif

#endif

	}
	else
	{
		return SWVERROR_NOFILES;
	}

	return SWV_SUCCESS;
}

const SWV_HEADER* CSwv_module::GetVehiclesData()
{
  return static_cast<const SWV_HEADER *>(vehicles);
}

int32_t CSwv_module::GetFacePos( unsigned int car_index ) 
{ 
  return (vehicles[car_index].pfiles[0].pos); 
}

int32_t CSwv_module::GetFaceSize( unsigned int car_index ) 
{ 
  return (vehicles[car_index].pfiles[0].length); 
}

int32_t CSwv_module::GetDriverFacePos( unsigned int car_index ) 
{ 
  return (vehicles[car_index].pfiles[1].pos); 
}

int32_t CSwv_module::GetDriverFaceSize( unsigned int car_index ) 
{ 
  return (vehicles[car_index].pfiles[1].length); 
}

int32_t CSwv_module::GetNamePos( unsigned int car_index ) 
{ 
  return (vehicles[car_index].pfiles[2].pos); 
}

int32_t CSwv_module::GetNameSize( unsigned int car_index ) 
{ 
  return (vehicles[car_index].pfiles[2].length);
}

int32_t CSwv_module::GetFramePos( unsigned int car_index, int frame ) 
{ 
  return (vehicles[car_index].pfiles[frame+2].pos); 
}

int32_t CSwv_module:: GetFrameSize( unsigned int car_index, int frame ) 
{ 
  return (vehicles[car_index].pfiles[frame+2].length); 
}
