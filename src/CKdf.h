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

#ifndef __CKDF_INCLUDED
#define __CKDF_INCLUDED


#define KDF_VER_MAJ			2
#define KDF_VER_MIN			1
#define SET_VER(x)			{ (x)->maj_ver = KDF_VER_MAJ; (x)->min_ver = KDF_VER_MIN; }

#define KDF_SUCCESS						1
#define KDF_ERROR_EMPTYDIR				10
#define KDF_ERROR_OPENTOSAVE			11
#define KDF_ERROR_OPENTOREAD			12
#define KDF_ERROR_NOHEADER				13
#define KDF_ERROR_UNSUPPORTED_VERSION	14
#define KDF_ERROR_FILEDOESNOTEXIST		-1

class CKdf_Packeger 
{
private:
	struct kdf_file 
	{
		char  filename[64];
		unsigned char  pos[4];
		unsigned char  size[4];
	};

	struct kdf_ver
	{
		unsigned char maj_ver;
		unsigned char min_ver;
	};

	kdf_file	*pfiles;				// array of file-info structures
	char		**paths;				// array of file-paths
	int			num_files;				// files added/opened
	char		kdf_pack_name[255];		// name of the packet to save/open
	bool		logfile;				// enable log

public:
	CKdf_Packeger();
	~CKdf_Packeger();

	//int  AddFile( char *file, char *file_dir );
	//int  AddDir( char *kdf );
	int CreateFromDir( const char *kdf_name, char *dir_name );
	int Open( const char *kdf_pathname );
	void Reset();

	long GetFilePosition( const char *file_name );
	long GetFilePosition( int file_index );
	long GetFileSize( const char *file_name );
	long GetFileSize( int file_index );
};


#endif
