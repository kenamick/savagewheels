// ckdf.cpp - 

#include "CKdf.h"

#ifdef LINUX_BUILD
#	include <dirent.h>
#	include <sys/stat.h>
#else
#	include <io.h>
#endif

#include <stdio.h>
#include <string.h>


///////////////////////////////////////////////////////////////////////////////////////
//// Name: CKdf_Packeger()
//// Desc:
///////////////////////////////////////////////////////////////////////////////////////
CKdf_Packeger::CKdf_Packeger()
:	pfiles(NULL),
	num_files(0)
{


}


///////////////////////////////////////////////////////////////////////////////////////
//// Name: ~CKdf_Packeger()
//// Desc:
///////////////////////////////////////////////////////////////////////////////////////
CKdf_Packeger::~CKdf_Packeger()
{
	Reset();
}


///////////////////////////////////////////////////////////////////////////////////////
//// Name: Reset()
//// Desc:
///////////////////////////////////////////////////////////////////////////////////////
void CKdf_Packeger::Reset()
{

	if ( pfiles != NULL )
	{
		delete[] pfiles;
		pfiles = NULL;
	}

	num_files = 0;
}



///////////////////////////////////////////////////////////////////////////////////////
//// Name: Open()
//// Desc: open package and read contents
///////////////////////////////////////////////////////////////////////////////////////
int CKdf_Packeger::Open( const char *kdf_pathname )
{

	FILE	 *fpKDF;
	char	 header[3];
	kdf_ver  ver;

	// open packet
	if ( (fpKDF = fopen( kdf_pathname, "rb" )) == NULL )
		return KDF_ERROR_OPENTOREAD;


	// save header & version
	fread( header, 3 * sizeof(char), 1, fpKDF );
	if ( header[0] != 'K' && header[1] != 'D' && header[2] != 'F' )
		return KDF_ERROR_NOHEADER;

	fread( &ver, sizeof(ver), 1, fpKDF );
	/* {
		..
	}
	*/

	// read file info-header
	fread( &num_files, sizeof(num_files), 1, fpKDF );
	pfiles = new kdf_file[num_files];
	fread( pfiles, sizeof(kdf_file) * num_files, 1, fpKDF );

	fclose( fpKDF );

	return KDF_SUCCESS;

}



///////////////////////////////////////////////////////////////////////////////////////
//// Name: CreateFromDir()
//// Desc: createing a data_file from a dir contents
///////////////////////////////////////////////////////////////////////////////////////
int CKdf_Packeger::CreateFromDir( const char *kdf_name, char *dir_name ) 
{


#ifndef LINUX_BUILD
	struct   _finddata_t  ffile;
	long     hFile;
#else
	DIR      *pDir = NULL;
        struct dirent *pDirEntry = NULL;  
	struct stat file_stat;
#endif

	int	 filenumber = 0;
	char	 filemask[255];
	
	FILE     *fpKDF, *fpkid;
	char     header[3] = { 'K', 'D', 'F' };
	kdf_ver  ver;
	long	 fh_pos; 
	char	 *buf;


	// make room for 600 entries
	pfiles = new kdf_file[600];
	//paths = (char **) new char[600][128];


#ifdef LINUX_BUILD
	pDir = opendir( dir_name );
	if ( pDir != NULL )
#else
	sprintf( filemask, "%s/%s", dir_name, "*.bmp" );	

	// get total files and fill info
	if ( ( hFile = _findfirst( filemask, &ffile ) ) != -1L )
#endif
	{

#ifndef LINUX_BUILD

		filenumber++;
		strcpy( pfiles[filenumber-1].filename, ffile.name );
		pfiles[filenumber-1].size = ffile.size;

		while ( _findnext( hFile, &ffile ) == 0 )
		{
			filenumber++;
			strcpy( pfiles[filenumber-1].filename, ffile.name );
			pfiles[filenumber-1].size = ffile.size;
		}
#else
		pDirEntry = readdir( pDir );
		while( pDirEntry != NULL )
		{
			if ( strcmp( pDirEntry->d_name, "." ) != 0 && strcmp( pDirEntry->d_name, ".." ) != 0 )
			{
				if ( stat( pDirEntry->d_name, &file_stat ) == 0 )
				{
					if ( ! S_ISDIR( file_stat.st_mode ) )
					{
	
						filenumber++;
						strcpy( pfiles[filenumber-1].filename, pDirEntry->d_name );
						pfiles[filenumber-1].size = file_stat.st_size;
					}
				}
			}
		}
#endif
				
	}
	else
	{
		if ( pfiles )
			delete[] pfiles;

		return KDF_ERROR_EMPTYDIR;
	}
	

	// save packet
	if ( (fpKDF = fopen( kdf_name, "wb" )) == NULL )
	{
		if ( pfiles )
			delete[] pfiles;

		return KDF_ERROR_OPENTOSAVE;
	}


	// save header & version
	fwrite( header, sizeof(header), 1, fpKDF );
	SET_VER(&ver);
	fwrite( &ver, sizeof(ver), 1, fpKDF );

	// save file info-header
	fwrite( &filenumber, sizeof(int), 1, fpKDF );
	// get file header position
	fh_pos = ftell( fpKDF ); 
	fwrite( pfiles, sizeof(kdf_file) * filenumber, 1, fpKDF );

	// save all in mother
	for ( int i = 0; i < filenumber; i++ )
	{

		sprintf( filemask, "%s/%s", dir_name, pfiles[i].filename );	

		fpkid = fopen( filemask, "rb" );

		pfiles[i].pos = ftell( fpKDF );

		buf = new char[pfiles[i].size];
		fread( buf, pfiles[i].size, 1, fpkid );

		fwrite( buf, pfiles[i].size, 1, fpKDF );

		delete[] buf;

		fclose( fpkid );
	}

	// pre-save file-header with file-positions
	fseek( fpKDF, fh_pos, SEEK_SET );
	fwrite( pfiles, sizeof(kdf_file) * filenumber, 1, fpKDF );

	fclose( fpKDF );

	if ( pfiles )
		delete[] pfiles;

	return KDF_SUCCESS;
}



///////////////////////////////////////////////////////////////////////////////////////
//// Name: GetFilePosition()
//// Desc: 
///////////////////////////////////////////////////////////////////////////////////////
long CKdf_Packeger::GetFilePosition( const char *file_name )
{
	kdf_file *ptr = pfiles;

	for ( int i = 0; i < num_files; i++ )
	{
		if ( ! strcmp( file_name, ptr->filename ) )
			return ptr->pos;

		ptr++;
	}

	return KDF_ERROR_FILEDOESNOTEXIST;
}




///////////////////////////////////////////////////////////////////////////////////////
//// Name: GetFileSize()
//// Desc: 
///////////////////////////////////////////////////////////////////////////////////////
long CKdf_Packeger::GetFileSize( const char *file_name )
{
	kdf_file *ptr = pfiles;

	for ( int i = 0; i < num_files; i++ )
	{
		if ( !strcmp( file_name, ptr->filename ) )
			return ptr->size;

		ptr++;
	}

	return KDF_ERROR_FILEDOESNOTEXIST;
}
