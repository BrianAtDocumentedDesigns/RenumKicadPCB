/*
 * RenumKiCadPCB.c v0.351
 *
 */

/*
 *      Author: Brian Piccioni DocumentedDesigns.com
 *      (c) Bian Piccioni
 *
 *      This is free software made available under the GNU General Public License(GPL) version 3 or greater.
 *      see https://www.gnu.org/licenses/gpl-3.0.en.html
 *
 *		It can be modified, copied, etc. provided:
 *		1) Original authorship of Brian Piccioni and DocumentDesigns.com is prominently noted
 *		2) Any derivative work or copy also be released under GPL V3 or later with the attribution mentioned in 1)
 *
 *		This software is release "as is" and with no warranty.
 *
 *		Please contact brian@documenteddesigns.com with any feature requests/bug notifications
 *		If you want me to fix a problem or add a feature I will expect you to provide sample files showing
 *		the problem or feature need.
 *
 * Changes:
 *  V0.300				Do all changes in memory before writing to files.
 *  					If there are warnings, ask whether to commit changes
 *  					Then backup all files, write out new ones.
 *  					Fixed ability to have a non-file extent "." i.e TERES_PCB1-A64-MAIN_Rev.C.pro
 *  					Fixed a number of bugs associated with/exposed by weird file content in TERES_PCB1-A64-MAIN_Rev.C.pro
 *
 *	V0.350				Try and prep for eventual intgration with KiCad
 *						Separate UI fro the actual function software
 *						Removed command line parsing (redundent given remembered parameters)
 *
 * ## Side : top
 * # Ref     Val         Package                PosX       PosY       Rot  Side
 * C1        0.1uF       C_1206             201.4460   -98.171
 *
 *
 *
 *	To do				Generate a footprint file0  180.0000  top
 *
 *		 				Figure out a GUI
 *
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<time.h>
#include 	<sys/time.h>
#include 	<math.h>
#include 	<unistd.h>

#ifdef		_WIN64
	#define		Windows
#endif

#ifdef		_WIN32
	#define		Windows
#endif


#ifdef		Windows					//Windows
#include	<conio.h>
char        SLASHCHR = '\\';          //Windows file path slash
#else	//Linux


#include <termios.h>
#include <unistd.h>
char        SLASHCHR = '/';          //Linux/Unix/Apple file path slash
int	getch(void);
#endif


#define	MAXREFDES 64			//Max characters to copy
#define	MAXSTRING	100		//Maximum size of an error string

#define	SORTX	0			//Sort on X
#define	SORTY	1			//Sort on Y

#define	ASCENDING 0			//Sort low to high
#define	DESCENDING 1		//Sort high to low

/*
 * New sort definitions
 */
#define	SORTXFIRST	0b000			//Sort on X
#define	SORTYFIRST	0b100			//Sort on Y

#define	ASCENDINGFIRST 		0b00				//Sort low to high
#define	DESCENDINGFIRST  	0b10				//Sort high to low
#define	ASCENDINGSECOND 	0b00				//Sort low to high
#define	DESCENDINGSECOND	0b01				//Sort high to low

#define	MINGRID 0.001
#define	DEFAULT_GRID 1.0

#define	TRUE 				1
#define FALSE 				0

#ifdef		Windows					//Windows
#define	EASCII				0xe0
#define	CR					0x0d
#define	BS					0x08
#else	//Linux
#define	EASCII				0x1b
#define	CR					0x0a
#define	BS					0x7f

#endif

#define	ABORT				0x03
#define	KEYESCAPE			0X400
#define	DEL					( 0x053 | KEYESCAPE )
#define	LEFTARROW			( 0x04b | KEYESCAPE )
#define	RIGHTARROW			( 0x04d | KEYESCAPE )

#define	SKIPREFDES			-1	//Skip this refdes

#define FILENOTFOUND 				1		//File not found error code
#define MALLOCERROR 				2		//Can't allocate memory error code
#define READERROR  					3		//Can't read the file error code
#define	NOMODULESERROR 				4		//No (modules field in file
#define	ERRNOCOORD					5		//Module had no coordinate
#define	MODULEMISSMATCH				6		//Different number modules from declaration
#define	OUTFILEOPENERROR 			7		//Can't open the output file
#define	OUTFILEWRITEERROR			8		//Can't write to the outputfile
#define	NOCHANGERROR				9		//Can't find the refdes in the change array
#define	PCBMISSMATCHERROR			10		//Mismatch in modules found during update
#define	SCHWRITECREATEERROR			11		//Can't create schematic output file
#define	SCHMISSINGLERROR			12		//Missing L in schematic file
#define	SCHWRITEERROR				13		//Can't write to schematic output file
#define	ARGERROR 					14		//Not enough arguments
#define	PCBBACKUPERROR				15		//Can't back up the PCB file
#define	SCHBACKUPERROR				16		//Can't back up the SCH file
#define NOINPUTFILEERROR			17		//No input file specified
#define	ERRNABORT					18		//User aborted
#define	ERRINVALIDEXIT				19		//Somehow exit main without clean exit
#define	PARAMWRITECREATEERROR 		20		//Can't write the parameter file
#define	SCHNOARREFERROR				21		//Missing the "Ref= in AR code
#define	TOPBOTTOMDEFINITIONERROR 	22		//Top/bottom definition mismatch
#define	TOOMANYMODULELAYERS			23		//More than 2 module layers
#define	NETBACKUPERROR				24		//Can't back up the NETLIST file
#define	NETWRITECREATEERROR			25
#define	BADNETLIST					26

#define	MAXPATH		2048

struct	KiCadModule {
	char	layer;							//What layer the module is on (usually 1 or 2)
	float	coord[2];						//X ad Y coordinate
	char 	RefDesType[MAXREFDES+1];		//What the RefDes Preamble is (i.e. U, R VR, etc.)
	char 	RefDesString[MAXREFDES+1];		//What the RefDes Preamble is (i.e. U, R VR, etc.)
	int 	RefDesNum;							//And what the RefDes is (i.e. 23)
	int		index;							//Used for sorting
};


struct	RefDes
{
	char 	RefDesType[MAXREFDES+1];			//What the RefDes Preamble is (i.e. U, R VR, etc.)
	char	OldRefDesString[MAXREFDES+1];		//What the old refdes preamble + number was
	char	*prepend;							//The prepend string
	int		OldRefDesNum;
	int 	NewRefDesNum;							//And what the RefDes is (i.e. 23)
	int		Found;								//Found the ref des in the schematic
};

struct	KiCadFileInMemory {						//How Kicad files are stored
char	filename[MAXPATH];						//The full file name
long	maxfilesize;							//The maximum size of the file (includes room for modules, etc
char	*filebuffer;							//And the pointer to the buffer
};


struct	ParameterType {
		char	*Parametername;					//i.e. "File Name"
		char	Parametertype;					//Text or number for read
		void	*Pointertoparameter;			//Where it goes
	};


struct	RenumParams
{
char	*projectfilename;	//Name of the project file (.pro stripped)
char    *projectpath;
char	*pcbbuffer;			//Where the PCB is in memory
char	*TopPrepend;		//The Top Prepend string
char	*BotPrepend;		//The Bottom Prepend string
int		topsortcode;		//The Top sort code (left to right, etc.)
int		bottomsortcode;		//The Bottom sort code (left to right, etc.)
int		topstartrefdes;		//The starting top ref des
int		bottomstartrefdes;	//The starting bottom ref des
int		sortonmodules;		//Sort on modules/ref des
float	grid;				//The sort grid

//struct	ProjectFileStruct	*ProjectFile;
//struct 	ProjectFileStruct 	*SchFile;
struct 	KiCadModule 		*ModuleArray;
struct 	KiCadModule 		*TopModuleArray;						//Pointer to top side ModuleArray
struct 	KiCadModule 		*BottomModuleArray;					//Pointer to bottom side ModuleArray
struct 	RefDes 				*RefDesChangeArray; 						//Pointer to changes array
struct 	RefDes 				*RefDesTypeArray;							//Pointer to refdes type array

int		NumberOfSheets;
int		Modules;
int		TopModules;
int		BottomModules;		//The number of modules in the PCB, Top and Bottom
int		NumRefDesTypes;
char	*paramfilename;		//Parameter file name to read/write

int		errcount;			//The error count
struct 	timeval	StartTime;	//Program run start time
struct 	timeval	EndTime;	//Program run end time

FILE    *loghandle;       //Used for tracking of issues

int     log;

};

#define LOGCOORD    0X01
#define LOGPLAN     0X02
#define LOGCHANGE   0X04
//
// Function prototypes
//

int 	RenumKiCadPCB( struct RenumParams *Params );
int		FindSchematicComponent( struct RenumParams *Params , char *dest, char *field, int *err_report );
int		FlushFile( struct KiCadFileInMemory *FileList );
char	*CopyBuffer( char *outbuffer, char *input );
char 	*CopyHereToThere( char *dest, char *from, char *to );
void	ClearKiCadFileInMemoryStruct( struct KiCadFileInMemory *FileList, int NumberOfEntries );
int		CountStrings( char *buffer, char *string );
void	CrawlSheets( char *path, char *SheetName, struct KiCadFileInMemory *FileList, int *NumberOfSheets );
char	*ScanForSheets( char *scanner,  struct KiCadFileInMemory *SheetStruct );
char	*LoadFile( char *filename, long *sizepoint );
int     CheckInputFileName( struct RenumParams *Params, char *extension );
int		MakeBackupFile( char *filename );
void	MakeFileName( char *dest, char *path, char *filename, char *extension );
void    TrimExtension( char *filename, char *extension );

void	LoadModuleArray( struct RenumParams *Params ); 		//Load up the module array
void	FreeMemoryAndExit( int code );
void	RenumError( char *message, int code );
char	*FindAndSkip( char *dest, char *buffer, char *instr, char *limit );
char	*CopyText( char *dest, char *source, int size );		//Copy the text part size limited
void	RoundToGrid( struct RenumParams *Params );
void	SortOnXY( struct KiCadModule*sortarray, int XY, int sortdirection, float grid  );
void	SortKiCadModules( struct RenumParams *Params, char Side );
void	MakeRefDesTypesArray( struct RenumParams *Params, char CountOrFill );
void	SetRefDesTypeStart( struct RenumParams *Params, char Side );					//Set to the starting top refdes
void	MakeRefDesChangeArray( struct RenumParams *Params, char Side );
int		GetModuleCount( char *buffer );
void	UpdatePCBFileRefDes( struct RenumParams *Params, struct	KiCadFileInMemory *FileList );
void	UpdateSchematicFileRefDes( struct RenumParams *Params, struct KiCadFileInMemory *FileList );
void	UpdateNetlistRefDes( struct RenumParams *Params, struct KiCadFileInMemory *FileList );
void	ProcessComponent( struct RenumParams *Params , char *dest, char *source );
void	CopyKiCadModuleArrayElement( struct KiCadModule *dest, struct KiCadModule *source, float grid );
void	CopyKiCadModuleArray( struct KiCadModule *dest, struct KiCadModule *source, int modules, float grid );
int		CountModules( struct KiCadModule *ModuleArray,  int Modules, char layer );
void	SplitTopBottom( struct RenumParams *Params );
void	PullFieldString( char *dest, char *buffer, char *instr );
void	SafeStringCopy( char *dest, char *source, int bufsize );
void	SafeStringConcatinate( char *dest, char *source, int maxlen );
void	AddSheetName( char *SheetName, char *SheetNameList );
int		mygetch( void );
void	RefDesToString( char *outstr, int refdes );
void	MakeNewRefDes( char *NewRefDes, struct RefDes *RefDesChangeArray );
void	SortSide( struct RenumParams *Params, char Side );
void    LineItUp( struct RenumParams *Params, char *text, int tab  );


void	LoadParameterFile( struct RenumParams *Params );
void	WriteParameterFile( struct RenumParams *Params );
void	ShowAndGetParameters( struct RenumParams *Params );
void	ShowMenu( struct RenumParams *Params );

/*
 * The Say Hello String
 */
char	G_HELLO[] =
"\nRenumKiCadPCB V0.351\nReleased under GPL 3.0 or later. See source for details.\
\n**** No warranty : use at your own risk ****\
\nWritten by Brian Piccioni. \nEmail brian@documenteddesigns.com with bug reports or feature requests";


char	G_ERRSTR[MAXPATH];

#define	PARAMETERFILENAME	"RenumParameters.txt"

/*
 * Let us begin
*/
int main( int argc, char *argv[] )
{
struct	RenumParams	Params;
char	projectfilename[ MAXPATH ] = "";
char    projectpath[ MAXPATH ] = "";
char	TopPrepend[ MAXREFDES ] = "";
char	BotPrepend[ MAXREFDES ] = "";

		setvbuf (stdout, NULL, _IONBF, 0);			//direct output to eclipse console
//
// Set up defaults
//
        Params.projectfilename = projectfilename;      //Name of the PCB/project file
        Params.projectpath= projectpath;               //Path to the PCB/project file
		Params.TopPrepend = TopPrepend;				//The Top Prepend string
		Params.BotPrepend = BotPrepend;				//The Bottom Prepend string

		Params.topsortcode = SORTYFIRST + ASCENDINGFIRST + ASCENDINGSECOND;			//The Top sort code (left to right, etc.)
		Params.bottomsortcode = SORTYFIRST + ASCENDINGFIRST + DESCENDINGSECOND;	//The Bottom sort code (left to right, etc.)
		Params.topstartrefdes = 1;						//The starting top ref des
		Params.bottomstartrefdes = 0;					//The starting bottom ref des
		Params.sortonmodules = 1;						//Sort on modules/ref des
		Params.grid = DEFAULT_GRID;					//The sort grid
		Params.paramfilename = PARAMETERFILENAME;		//Where the parameters are stored.

		Params.loghandle = NULL;
//		Params.log = LOGPLAN + LOGCHANGE + LOGCOORD;

		ShowAndGetParameters( &Params );			//Show what you got and allow them to change

		if( Params.grid < MINGRID ) Params.grid = MINGRID;	//Make sure sort grid is > 0
		WriteParameterFile( &Params );				//Save these for later

		Params.errcount = 0;						//Nothing wrong yet
		RenumKiCadPCB( &Params );	//Do this

		gettimeofday( &Params.EndTime, NULL );				//Get the start time
		printf("\nRun time of %10.3f Seconds", (float) ((Params.EndTime.tv_sec - Params.StartTime.tv_sec ) +
										((float)(Params.EndTime.tv_usec - Params.StartTime.tv_usec )/1000000.))) ;

		printf("\nDone\n");
		FreeMemoryAndExit( EXIT_SUCCESS );
		return( ERRINVALIDEXIT );
}	//main()

//
//
//  Renumber the PCB and backannotate the schematic and netlist (if present)
//
//
int 	RenumKiCadPCB( struct RenumParams *Params )
{
int		i;
//
// First thing I want to do is create a list of schematics in the project
//
		if( 0 != Params->log )              //If making a log file
		{
char    outfilename[ MAXPATH ];

		    MakeFileName( outfilename, Params->projectpath, "renum_logfile", ".txt" );  //Make the debug file name
		    Params->loghandle = fopen ( outfilename,"wb");

		    if ( Params->loghandle == NULL)                     //Not found
		           RenumError("Can't open  file!", FILENOTFOUND );     //I am out of here
		    fprintf(Params->loghandle, "RenumKicadPCB Log File\n");
		}

		Params->NumberOfSheets = 1;
		CrawlSheets( Params->projectpath, Params->projectfilename, NULL, &Params->NumberOfSheets );		//Get the number of sheets in the schematic

struct	KiCadFileInMemory FileList[ Params->NumberOfSheets + 5 ];								//Room for each schematic sheet, PCB, netlist
		ClearKiCadFileInMemoryStruct( FileList, Params->NumberOfSheets + 4 );
//
//FileList[0] is reserved for the PCB
//FileList[1] is reserved for the Netlist
//FileList[2 ...] are the schematic sheets
//
		Params->NumberOfSheets = 1;
		CrawlSheets( Params->projectpath, Params->projectfilename, &FileList[2], &Params->NumberOfSheets );	//Get the number and total storage of the sheets
		printf("\nThere are %d unique sheets in the schematic.", Params->NumberOfSheets  );
//
//Now I know the names and sizes of the schematics in the project. The sizes are adjusted for component substitution
//
		MakeFileName( FileList[0].filename, Params->projectpath, Params->projectfilename, ".kicad_pcb" );				//Make the full file name
		Params->pcbbuffer = LoadFile( FileList[0].filename, &(FileList[0].maxfilesize) );		//Load this file and extension, get file size
//
// Adjust the output file size for the .kicad_pcb file
//
		FileList[0].maxfilesize += (2 * MAXREFDES * ((CountStrings( Params->pcbbuffer,
						"fp_text reference" ) 											//How many text reference fields
								+ CountStrings( Params->pcbbuffer, "\"Net-(") )));					//How many nets in this PCB
//
		Params->Modules = GetModuleCount( Params->pcbbuffer );						//Extract the (modules field
struct 	KiCadModule ModuleArray[ Params->Modules + 1];								//Allocate memory for the ModuleArray
		Params->ModuleArray = ModuleArray;
		LoadModuleArray( Params );							 						//Load up the module array

		RoundToGrid( Params );

		Params->TopModules = CountModules( ModuleArray, Params->Modules, 'F' );			//How many modules on the top?
		Params->BottomModules = CountModules( ModuleArray, Params->Modules, 'B' );			//How many modules on the top?
		if(( Params->BottomModules + Params->TopModules ) != Params->Modules )
			RenumError("Error: module layer setup problem: check top and bottom layer definition",
					TOPBOTTOMDEFINITIONERROR );		//I am out of here

struct 	KiCadModule TopModuleArray[ Params->TopModules + 1 ];					//Allocate memory for the top side ModuleArray
struct 	KiCadModule BottomModuleArray[ Params->BottomModules + 1 ];				//Allocate memory for the bottom side ModuleArray

		Params->TopModuleArray = TopModuleArray;
		Params->BottomModuleArray = BottomModuleArray;
		SplitTopBottom( Params );		//Create separate top and bottom array

struct 	RefDes RefDesChangeArray[ Params->Modules + 1 ]; 						//Allocate for changes array
		Params->RefDesChangeArray = RefDesChangeArray;
		MakeRefDesTypesArray( Params, 'C' );									//Go through and count the number of types of refdeses

struct 	RefDes RefDesTypeArray[ Params->NumRefDesTypes + 1 ];			 		//Allocate for refdes type array
		Params->RefDesTypeArray = RefDesTypeArray;
		MakeRefDesTypesArray( Params, 'F'  );									//Now fill up the array
//
// Now make the change plan and related file
//
		if( 0 != (Params->log & LOGPLAN))
		{
		    fprintf( Params->loghandle, "\n\nReference Designator Change Plan for \n     %s", Params->projectfilename );
		    fprintf( Params->loghandle, "\n\nThere are %d types of reference designations", Params->NumRefDesTypes );
		    for( i = 0; i < Params->NumRefDesTypes; i++ )
		    {
		        if( 0 == i%8 ) fprintf( Params->loghandle, "\n" );              //On a new line
		        LineItUp( Params, RefDesTypeArray[i].RefDesType, 10 );            //Pad spaces
		    }
		}

		if( Params->TopModules != 0 )			//Only do if there are modules on the top
			SortSide( Params, 'F');

		if( Params->BottomModules != 0 )		//Only do if there are modules on the bottom
			SortSide( Params, 'B');
/*
 * Note that the PCB file is already in memory @buffer
 */
		UpdatePCBFileRefDes( Params, &FileList[0] );							//Update the PCBs and the nets (all done in memory
		printf("\n\nUpdating %s", FileList[0].filename );						//Put this here to separate from log files
//
		for( i = 0; i < Params->NumberOfSheets; i++)
		{
			UpdateSchematicFileRefDes( Params, &FileList[ i + 2] );
			printf("\nUpdating Sheet #%d %s ", i, FileList[i+2].filename );		//Put this here to separate from log files
		}

		for( i = 0; i < Params->Modules; i++ )
			if(( RefDesChangeArray[i].Found == 0 ) && ( RefDesChangeArray[i].OldRefDesNum != SKIPREFDES ))
			{	printf("\nWarning PCB component %s not found in schematic!", RefDesChangeArray[i].OldRefDesString  );
				++Params->errcount;
			}

		UpdateNetlistRefDes( Params, &FileList[1] );							//Now update the netlist names
		if( FileList[1].filebuffer != NULL ) printf("\nUpdating %s", FileList[1].filename );
//
// All the file processing is done. If there are errors, ask to commit.
//		If no errors or OK to commit, create backup files for all the file names and write out the new files
//
		if( 0 != Params->errcount )		//If there were errors
		{
			printf("\n**** Warning: %d errors in files ****\n				/"
					"\n**** Proceed to write files despite errors (backups will be made)? (Y/N)?", Params->errcount );
			i = toupper( mygetch());			//Get the reply
			if( i == 'Y' ) Params->errcount = 0;
		}

		if( 0 == Params->errcount)		//If no errors or decides to commit
		{
			printf("\n\nMaking backups ... ");
			i = 0;
			printf("\n\nWriting files ... ");

			for( i = 0; i < (Params->NumberOfSheets + 4); i++)
			if( FileList[i].filename[0] != '\0')					//Just in case no netlist file
			{
				if( MakeBackupFile( FileList[i].filename ) != 0 )
					printf("\n   Can't create backup file for %s", FileList[i].filename );
				else if( FlushFile( &FileList[i] ) != 0 )
					printf("\n   Can't write %s", FileList[i].filename );
			}
		}

		if( NULL != Params->loghandle ) fclose( Params->loghandle );
		for( i = 0; i < (Params->NumberOfSheets + 2); i++)		//One for PCB, one for Netlist, then x for sheets
			if( FileList[ i ].filebuffer != NULL ) free(FileList[ i ].filebuffer );
		return(0);
}//RenumKiCadPCB

//
// Write out the file stored in memory. Ret NZ if error
//

int	FlushFile( struct KiCadFileInMemory *FileList )
{
int	retval;
FILE	*handle = fopen ( FileList->filename,"wb");
		printf("\nWrite %s", FileList->filename );
		if( NULL == handle ) return( 1 );
		retval = ( fwrite( FileList->filebuffer, sizeof(char), FileList->maxfilesize, handle ) != FileList->maxfilesize ? 2 : 0 );
		fclose( handle );
		return( retval );
}


/*
 * This is where we usually leave: ensures the memory is freed up
 */
void	FreeMemoryAndExit( int code )
{
	printf("\n\nHit any key to exit ");
	mygetch();
	printf("\n\n");
	exit( code );
}

/*,
 * Common error exist routine
 */
void	RenumError( char *message, int code )
{
	printf("\n%s", message );					//Say the word
	FreeMemoryAndExit( code );					//ByeBye
}
/*
 * A safe version of strcpy which no overwrite
 */

void	SafeStringCopy( char *dest, char *source, int bufsize )
{
int		srclen = strlen( source );
		--bufsize;							//Make room for the '\0'
		if( bufsize > 0 )
		{
			srclen = ( srclen > bufsize ? bufsize : srclen );
			while( srclen-- > 0 ) *dest++ = *source++;
		}
		*dest = '\0';
}

/*
 * A safe version of strcat which no overwrite
 */

void	SafeStringConcatinate( char *dest, char *source, int maxlen )
{
int		srclen = strlen( source );
int		destlen = strlen( dest );

		dest += destlen;					//Skip to the end of the destination

		srclen = (( srclen + destlen ) > maxlen ) ? ( maxlen - ( srclen + destlen ) - 1 ) : srclen;


		if( srclen >= 0 )								//If something to do
			while( srclen-- >= 0 ) *dest++ = *source++;	//And concatenate
}


//
// Make a backupfile by renaming the oldfile
//
// Enter with an filename (with or without extension), path to the file, and the extension
//

int		MakeBackupFile( char *filename )
{
char	newfilename[ MAXPATH ];			//what the remaned file will be (.ext_RenumBak)
		strcpy( newfilename, filename );
		strcat( newfilename, "_RenumBack");
		remove( newfilename );
		return( rename( filename, newfilename ));
}


//
//  Make a file name with the path, filename, extension
//  This trims any path from the filename before copying it
//
void    MakeFileName( char *dest, char *path, char *filename, char *extension )
{
char    *lastslash = strrchr( filename , SLASHCHR );     //Path ends with the first slash from the right

        lastslash = ( lastslash == NULL ? filename : lastslash + 1 );   //if no slash then the
        strcpy( dest, path );                   //Get the path
        strcat( dest, lastslash );               //Use this file name
        strcat( dest, extension );              //And add this extension
}

//
//	Assumes the file name has the full path
//
char	*LoadFile( char *filename, long *sizepoint )
{
long 	filesize, readbytes;
char	*buffer;

FILE	*readhandle = fopen ( filename,"rb");						//Open the file read only binary

	if (readhandle == NULL)											//Not found
	{
		printf("\nLoading %s ", filename  );
		RenumError("LoadFile: File not found!", FILENOTFOUND );		//I am out of here
	}

	fseek(readhandle, 0L, SEEK_END);								//Go to the end of the file
	filesize = sizeof(char) * (ftell( readhandle ));				//Where am I?

	if(sizepoint != NULL )
		*sizepoint = filesize;										//Needed for certain functions.

	fseek(readhandle, 0L, SEEK_SET);								//Go to the start of the file

	buffer = (char*) calloc ( filesize + 1, sizeof(char)); 	 	// allocate memory to contain the whole file plus a '\0'

	if ( buffer == NULL)
		RenumError("Can't allocate memory for file!", MALLOCERROR );		//I am out of here

	readbytes = fread (buffer, sizeof( char ), filesize, readhandle);

int	fer = ferror( readhandle );
int	feo = feof( readhandle );

	fclose( readhandle );

	if( readbytes != filesize )
	{
		printf("\nRead %ld, filesize %ld, FERROR %d, FEOF %d", readbytes, filesize, fer, feo );
		RenumError("Read error!", READERROR );		//I am out of here
	}
	return( buffer );
}	//LoadFile()


/*
 * Get a float from a zero  terminated string
 * Return characters scanned until a space after the float
 */
char	*GetFloatFromString( float *dest, char *floatstring )
{
int 	i = 0;

		*dest = 0;

		while(( floatstring[i] <= ' ' ) && ( floatstring[i] != '\0')) i++;
		if( floatstring[ i ] != '\0')
		{
			*dest = atof( floatstring );
			while(( floatstring[i] != ' ') && ( floatstring[i] != ',' ) && ( floatstring[i] != '\0')) i++;
		}
		return( floatstring + i );
}

/*
 * Dertermine the number of modules in the PCB file
 */
int	GetModuleCount( char *buffer )
{
char	*found = strstr( buffer, "(modules ");				//Where the next module is
int		modules = 0;

	if( found != NULL ) modules = atoi( found + sizeof( "(modules" ));	//Get how many modules there arre
	if( modules == 0 )
		RenumError("No modules count in file!", NOMODULESERROR );		//I am out of here

	printf("\nThere are %d modules on the PCB\n", modules );
	return( modules );
} //GetModuleCount( char *buffer )


/*
 * 		LoadModuleArray( ModuleArray, Modules, Params->pcbbuffer ); 				//Load up the module array
		LoadModuleArray( ModuleArray, Modules, Params );	 						//Load up the module array
 *
 * Scan the PCB file in memory and fill in the Module Array
 */

void	LoadModuleArray( struct RenumParams *Params ) 		//Load up the module array
{
char	*found, *nextmodule, *txtfound, *atfound;				//Where the next module is
char	workbuffer[2*MAXREFDES+1];		//Where the strings go for now
char	*anglepnt;

int 	i = 0, j;						//And what the RefDes is (i.e. 23)
float	ModXCoordinate, ModYCoordinate;				//What the X and Y Coordinate of the module is
float	modangle, sinmodangle, cosmodangle;

/*
 * I want to sort on the absolute position of the text reference. This avoids issues with, say resistors, where the module may be rotated
 * If you think about it you are going to look for the text reference.
 *
 * The tricky bit is that the text reference coordinates are relative to the module, so I get the module X, Y, then add the
 * text reference to XY to that
 */

char	FrontLayerString[MAXREFDES];		//Room for the front layer name (Kicad is 20 chars)
char	BackLayerString[MAXREFDES];			//Room for the back layer name (Kicad is 20 chars)
char	TMPLayerString[MAXREFDES];			//Scratch
int		FrontModuleCount = 0;
int		BackModuleCount = 0;

		FrontLayerString[0] = 0;		//Start with nothing
		BackLayerString[0] = 0;

char layer;

	found = Params->pcbbuffer;								//Got to start somewhere
	do
	{
		found = strstr( found, "(module ");		//Find the "(module " token
		if( found != NULL )
		{
			found += sizeof("(module")/sizeof(char);			//Skip this
			nextmodule = strstr( found, "(module ");			//Use this to find where the next module starts

			found = FindAndSkip( workbuffer, found, "(layer ", nextmodule );	//get the layer of the refdes

			CopyText( TMPLayerString, workbuffer, sizeof(TMPLayerString) );		//Get the layer name

			layer = 0;
			if( strcmp( FrontLayerString, TMPLayerString ) == 0 )				//This is the front string
				layer = 'F';
			else
				if( strcmp( BackLayerString, TMPLayerString ) == 0 )
					layer = 'B';

				else //Not found
				{
					if( FrontLayerString[0] == 0 )
					{
						CopyText( FrontLayerString, TMPLayerString, sizeof(FrontLayerString) );		//Get the layer name for front layer
						layer = 'F';												//This module is on the front layer
					}

					else if( BackLayerString[0] == 0 )
					{
						CopyText( BackLayerString, TMPLayerString, sizeof(FrontLayerString) );		//Get the layer name for front layer
						layer = 'B';												//This module is on the back layer
					}

					else //Get here and an error condition: more than 2 module layers ...
					{
						printf("F %s, B %s, T %s", FrontLayerString, BackLayerString, TMPLayerString );
							RenumError("Error: More than 2 module layers",
									TOOMANYMODULELAYERS );		//I am out of here
					}
				}

			Params->ModuleArray[i].layer = layer;
			if( layer == 'F') ++FrontModuleCount; else ++BackModuleCount;

			found = FindAndSkip( workbuffer, found, "(at ", nextmodule );

			anglepnt = GetFloatFromString( &Params->ModuleArray[i].coord[1], GetFloatFromString( &Params->ModuleArray[i].coord[0], workbuffer ) );
			GetFloatFromString( &modangle, anglepnt );

			found = FindAndSkip( workbuffer, found, "(fp_text reference ", nextmodule );	//Find the reference designator

			txtfound = CopyText( Params->ModuleArray[i].RefDesType, workbuffer, sizeof(Params->ModuleArray[i].RefDesType ));					//Copy the text part of the refdes
			Params->ModuleArray[i].RefDesNum = atoi( txtfound );

			j = 0;
			while(( workbuffer[j] != ' ' ) && ( j < MAXREFDES ))
			{
				Params->ModuleArray[i].RefDesString[j] = workbuffer[j];
				j++;
			}
			Params->ModuleArray[i].RefDesString[j] = '\0';

			if(( Params->ModuleArray[i].RefDesString[j-1] < '0' ) ||  ( Params->ModuleArray[i].RefDesString[j-1] > '9' )) //If it doesn't end with a number (i.e. *
				Params->ModuleArray[i].RefDesNum = SKIPREFDES;					//Skip it

			atfound = strstr( workbuffer, "(at ");
			if( atfound  == NULL )
			{	snprintf( G_ERRSTR, MAXSTRING, "Error: Missing coordinates for module %s%d", Params->ModuleArray[i].RefDesType, Params->ModuleArray[i].RefDesNum  );
				RenumError( G_ERRSTR, ERRNOCOORD );
			}

			GetFloatFromString( &ModYCoordinate, GetFloatFromString( &ModXCoordinate, atfound + sizeof("(at")));
			if( Params->sortonmodules == 0 )						//Only if sorting by reference designator coordinate
			{
				modangle = modangle * ( 3.14159265 / 180.0 );	//Convert angle to radians
				sinmodangle = sin( modangle  );				//Find the sine of the module angle
				cosmodangle = cos( modangle  );				//Find the sine of the module angle
				Params->ModuleArray[i].coord[0] += (( ModXCoordinate * cosmodangle )  + ( ModYCoordinate * sinmodangle ));	//Save the coordinate (done for clarity
				Params->ModuleArray[i].coord[1] += (( -ModXCoordinate * sinmodangle )  + ( ModYCoordinate * cosmodangle ));
			}
			Params->ModuleArray[i].index = i;
			i++;
		}
	}while( found != NULL );

	if( i != Params->Modules )
		RenumError( "\nError: PCB modules does not match declaration", MODULEMISSMATCH );

	printf("There are %d modules on layer: %s\n", FrontModuleCount, FrontLayerString );
	if(BackModuleCount != 0 )
		printf("There are %d modules on layer: %s\n", BackModuleCount, BackLayerString );

}

//
//  Routines to make nice with tabs
//
void    FloatNumberAlign( struct RenumParams *Params, float afloat, int tab  )
{
char    tmpstr[10];
        sprintf( tmpstr, "%.3f", afloat );
        LineItUp( Params, tmpstr, 10  );
}

void    NewLineNumber( struct RenumParams *Params, int linenum, int tab  )
{
char    tmpstr[20];
        sprintf( tmpstr, "\n#%d", linenum );
        LineItUp( Params, tmpstr, 10  );
}
//
//  Make nice with tabs
//
void    LineItUp( struct RenumParams *Params, char *text, int tab  )
{
char    outstr[ strlen( text) + tab + 2 ];
int     i = strlen( text );              //Start here
        strcpy( outstr, text );         //start with the text

        while( 0 != i%tab) outstr[i++] = ' ';
        outstr[i] = '\0';
        fprintf( Params->loghandle, "%s", outstr );
}

//
//
//	Round to a sort grid (can make more sense sorting slightly off position modules.
//
void	RoundToGrid( struct RenumParams *Params )
{
int		i;
float	rounder, old_x, old_y;

char    tmpstr[5]; //scratch

struct KiCadModule *ModuleArray = Params->ModuleArray;

        if( 0 != (Params->log & LOGCOORD))
            fprintf(Params->loghandle, "*********** Sort on %s Coordinates *******************",
			        ( Params->sortonmodules == 0 ? "Ref Des" : "Module" ));
//
// Round the Module Array to the grid (only do this once)
//
		for( i = 0; i < Params->Modules; i++ )
		{
			old_x = ModuleArray[i].coord[0];
			rounder = fmod( old_x, Params->grid);
			ModuleArray[i].coord[0] -= rounder;							//X coordinate down to grid
			if( rounder > ( Params->grid/2 )) ModuleArray[i].coord[0] += Params->grid  ;		//Round X coordinate up to grid

			old_y = ModuleArray[i].coord[1];
			rounder = fmod( old_y, Params->grid );
			ModuleArray[i].coord[1] -= rounder;							//Y coordinate down to grid
			if( rounder >= ( Params->grid /2 )) ModuleArray[i].coord[1] += Params->grid;		//Round Y coordinate up to grid

	        if( 0 != (Params->log & LOGCOORD))
	        {
	            NewLineNumber(Params, i, 5 );
	            tmpstr[0] = ModuleArray[i].layer;           //Layer
	            tmpstr[1] = '\0';
                LineItUp( Params, tmpstr, 4  );
                LineItUp( Params, ModuleArray[i].RefDesString, 16  );
                FloatNumberAlign( Params, old_x, 10 );
                FloatNumberAlign( Params, old_y, 20 );
                FloatNumberAlign( Params, ModuleArray[i].coord[0], 10 );
                FloatNumberAlign( Params, ModuleArray[i].coord[1], 10 );
	        }
		}
//LineItUp( Params, char *text, 10  )

} //RoundToGrid

/*
 * Find the instr in the buffer only if less than limit (ignore limit if NULL)
 * copy the string after instr into the dest buffer up to MAXREFDES characters or until a ')' and 0 temrinate it
 * return pointer to buffer where instr was found after the copied string
 *
 * return NULL if not found
 */

char	*FindAndSkip( char *dest, char *buffer, char *instr, char *limit )
{
char	*found;
int		i;
		*dest = '\0';									//Assume not found

		found = strstr( buffer, instr );				//Use this to find where the next module starts

		if(( found > limit ) && ( limit != NULL ))		//In case not found in this module
			return( NULL );								//Stop here

		found += strlen( instr );						//Skip the input string

		for( i = 0; (( i < MAXREFDES) && (*found != ')')); i++ )	//End with ')'
			*dest++ = *found++;						//Copy the string

		*dest++ = '\0';									//Make sure zero terminate
		return( found );
} //FindAndSkip


/*
 * Copy only the text part of a string return pointer to non-alpha, non-space, printable
 */
char	*CopyText( char *dest, char *source, int size )		//Copy the text part
{
	while((( 0 != isalpha( (int) *source )                      //As long as a letter
	        || (( 0 != ispunct((int) *source ) && ( ' ' != *source ))   //Or a non-space punctuation.
	              && (size-- > 0 )))))			//Until the end
		*dest++ = *source++;					//Copy
	*dest = '\0';								//Zero terminate the destination string
	return( source );
}//CopyTest()

//
// Sort the sort array on X or Y.
// Sort array structure is
// int	index;
// float coord[2]	//Element 0 is X by convention, element y is 1 by convention
//
// If XY = 0 I sort on X, if XY = 1 I sort on Y
//
// The sort array is terminated with index = -1;
//
// Sort ascending if 1 else descending
//
void	SortOnXY( struct KiCadModule*sortarray, int XY, int sortdirection, float grid  )
{
float	minmax, bigsmallnum;					//Assume board less than 10M wide
int		i, j = 0, tmpindex, arraysize = 0;

	while( sortarray[arraysize].index != -1 ) arraysize ++;		//Size the array
	if( arraysize == 0 ) return;				//Nothing to sort

struct KiCadModule	tmparray[ arraysize + 1 ];		//Make a temporary array

	CopyKiCadModuleArray( tmparray, sortarray, arraysize, grid );	//Copy the input array and adjust coordinates to grid because sort destroys the data

	if( sortdirection == ASCENDING ) 					//If a low to high sort`
		bigsmallnum = 10000.;					//Big coordinate
	else
		bigsmallnum = -10000.;					//Small coordinate

	do {
		minmax = bigsmallnum;						//Assume the worse.
		tmpindex = -1;								//If nothing found -1

		for( i = 0; i < arraysize; i++ ) {
			if( sortdirection == ASCENDING ) {
				if( tmparray[i].coord[XY]  < minmax ) {
					tmpindex = i;
					minmax = tmparray[i].coord[XY];		//Found the lowest
				}
			}
			//Done for clarity rather than a complex conditional
			else if( tmparray[i].coord[XY]  > minmax ) {
					tmpindex = i;
					minmax = tmparray[i].coord[XY];		//Found the highest or lowest
			}
		}

		if( tmpindex != -1 ) {											//If I found something
			tmparray[tmpindex].index = tmpindex;						//This says the location of the largest/smallest
			CopyKiCadModuleArrayElement( &sortarray[j++], &tmparray[tmpindex], grid );	//Copy over to the sort array
			tmparray[tmpindex].coord[XY] = bigsmallnum;					//Only once for this element
		}
	} while( j < arraysize );						//Until the end
}	// SortOnXY

/*
 *
 * Copy a KicadModuleArray
 *
 */

void	CopyKiCadModuleArray( struct KiCadModule *dest, struct KiCadModule *source, int modules, float grid )
{
int	i;
	for( i = 0; i < modules; i++ )
	{
		dest->layer = source->layer ;						//What layer the module is on (usually F or B )
		dest->coord[0] = source->coord[0];					//Copy X coordinate
		dest->coord[1] = source->coord[1];					//Copy Y  coordinate
		SafeStringCopy( dest->RefDesString, source->RefDesString, MAXREFDES );		//What the RefDes Preamble is (i.e. U, R VR, etc.)
		SafeStringCopy( dest->RefDesType, source->RefDesType, MAXREFDES );		//What the RefDes Preamble is (i.e. U, R VR, etc.)
		dest->RefDesNum = source->RefDesNum;						//What the RefDes is (i.e. 23)
		dest->index = source->index;						//And the index (used for sort)
		source++;
		dest++;
	}
}

/*
 *
 * Copy a KicadModuleArray Element
 *
 */

void	CopyKiCadModuleArrayElement( struct KiCadModule *dest, struct KiCadModule *source, float grid )
{
		dest->layer = source->layer ;						//What layer the module is on (usually F or B )
		dest->coord[0] = source->coord[0];					//Copy X coordinate
		dest->coord[1] = source->coord[1];					//Copy Y  coordinate
		SafeStringCopy( dest->RefDesString, source->RefDesString, MAXREFDES );		//What the RefDes Preamble is (i.e. U, R VR, etc.)
		SafeStringCopy( dest->RefDesType, source->RefDesType, MAXREFDES );		//What the RefDes Preamble is (i.e. U, R VR, etc.)
		dest->RefDesNum = source->RefDesNum; 				//What the RefDes is (i.e. 23)
		dest->index = source->index;						//And the index (used for sort)
}

//
// Front and back side sorts and changes require different parameters.
// Do front if Side == 'F' else do back
//
void	SortSide( struct RenumParams *Params, char Side )
{
			SetRefDesTypeStart( Params, Side );						//Set to the starting top refdes
			SortKiCadModules( Params, Side);						//Sort this side
			MakeRefDesChangeArray( Params, Side);		//Make the change array
}



/*
 * This allow arbitrary starting refdes
 * It is mostly useful if you have a prepend (i.e B_R1)
 *
 */
void	SetRefDesTypeStart( struct RenumParams *Params, char Side )					//Set to the starting top refdes
{
int		i = 0;
int		startrefdes = ( 'F' == Side ? Params->topstartrefdes : Params->bottomstartrefdes );

struct RefDes *typesarray = Params->RefDesTypeArray;

	if( startrefdes == 0 ) return;							//Nothing to do

	while(typesarray[i].RefDesType[0] != '\0')			  	//Until the end
		typesarray[i++].NewRefDesNum = startrefdes;				//Set the starting point
}

/*
 * 	Figure out what types of reference designators there are
 *	Make an array with those and starting with refdes 1
 *	Returns the number of reference designator types
 */


void	MakeRefDesTypesArray( struct RenumParams *Params, char CountOrFill )
{
int	i, j, found;

struct RefDes *outarray = (( 'C' == CountOrFill ? Params->RefDesChangeArray : Params->RefDesTypeArray ));	//Use change as scratch to count;

	outarray[0].RefDesType[0] = '\0';			//Zap the refdes string
	Params->NumRefDesTypes = 0;

	for( i = 0; i < Params->Modules; i++)
	{
		found = 0;								//Assume not found
		for( j = 0; j < Params->NumRefDesTypes; j++ )
			if( strcmp( Params->ModuleArray[i].RefDesType, outarray[j].RefDesType ) == 0 )
				found = 1;

		if( found == 0 )				//Not found in the table so insert it
		{
			SafeStringCopy( outarray[j].RefDesType, Params->ModuleArray[i].RefDesType, MAXREFDES );				//Copy it in
			outarray[j].NewRefDesNum = 1;														//Start with a 1
			Params->NumRefDesTypes++;																//One more type of refdes
			outarray[Params->NumRefDesTypes].RefDesType[0] = '\0';									//Zap the string
		}
	}
	return;

} //MakeRefDesTypesArray()

/*
 * Now create an array which has the refdes type, the old refdes and the new refdes
 * There is an optional prepend string which allows refdeses to be renamed (eg) T_ or  B_ so you get T_R21 and B_U2
 */

void	MakeRefDesChangeArray( struct RenumParams *Params, char Side )
{
int		i = 0, j;
int 	errcnt = 0, blankcount = 0;

struct 	KiCadModule *modulearray;
struct 	RefDes *refdeschangearray;
int 	modules;
char 	*prepend;

char    NewRefDes[MAXREFDES * 4 ];          //Save the new ref des

int 	numrefdestypes = Params->NumRefDesTypes;

	if( 'F' == Side )
	{
		modulearray = Params->TopModuleArray;
		refdeschangearray = &Params->RefDesChangeArray[0];
		modules = Params->TopModules;
		prepend = Params->TopPrepend;
	}
	else
	{
		modulearray = Params->BottomModuleArray;
		refdeschangearray = &Params->RefDesChangeArray[Params->TopModules];
		modules = Params->BottomModules;
		prepend = Params->BotPrepend;
	}

	if( 0 != (Params->log & LOGPLAN))
	    fprintf( Params->loghandle, "\n\n%s Side Changes", (Side == 'F'? "Front" : "Back"));

	for( i = 0; i < modules; i++)
	{
		if(( '\0' == modulearray[i].RefDesType[0] ) && ( 0 == blankcount ))
		{
			printf("\nWarning : zero length refdes on PCB %s !", ( 'F' == Side ? "Front" : "Back" ));
			++blankcount;
		}

		for( j = 0; j < numrefdestypes; j++ )
		{
			if( strcmp( modulearray[i].RefDesType, Params->RefDesTypeArray[j].RefDesType ) == 0 )	//I found the type (U, R, VR, etc )
			{
				SafeStringCopy( refdeschangearray[i].RefDesType, Params->RefDesTypeArray[j].RefDesType, MAXREFDES ); //Copy the type (U, R, VR, etc)
				SafeStringCopy( refdeschangearray[i].OldRefDesString, modulearray[i].RefDesString, MAXREFDES ); //Copy the type (U, R, VR, etc)
				refdeschangearray[i].prepend = prepend;										//Know what to prepend to update
				refdeschangearray[i].OldRefDesNum = modulearray[i].RefDesNum;						//Get the old refdes
				refdeschangearray[i].NewRefDesNum = Params->RefDesTypeArray[j].NewRefDesNum++;	//Show the new one and bump it
				refdeschangearray[i].Found = 0;												//Assume not found in the schematic

				if( 0 != (Params->log & LOGPLAN))
				{
	                NewLineNumber(Params, i, 5 );
	                LineItUp( Params, modulearray[i].RefDesString, 16  );
                    LineItUp( Params, "->", 8  );
				}

				if(refdeschangearray[i].OldRefDesNum == SKIPREFDES )			//If I ignore this one
				    strcpy( NewRefDes, refdeschangearray[i].OldRefDesString );
				else
				    MakeNewRefDes( NewRefDes, &refdeschangearray[i] );

				if( 0 != (Params->log & LOGPLAN))
				{
                    LineItUp( Params, NewRefDes, 16  );
	                if(refdeschangearray[i].OldRefDesNum == SKIPREFDES )           //If I ignore this one
	                    LineItUp( Params, "*** will be ignored ***", 2  );
				}
				break;
			}
		}

		if(( j >= numrefdestypes ) && ( errcnt ++ < 10 ))	//Show max 10 error per sheetS
		{
			printf("\nError %i Refdes %s not found! ", errcnt, modulearray[i].RefDesType );
			if( 0 != (Params->log & LOGPLAN))
			    fprintf(Params->loghandle, "\nError %i Refdes %s not found! ", errcnt, modulearray[i].RefDesType );
		}
	}

	Params->errcount += errcnt;

} //MakeRefDesChangeArray

/*
 * Count the number of top modules (also tells you bottom modules
 */

int	CountModules( struct KiCadModule *ModuleArray,  int Modules, char layer )
{
int	i, TopModules = 0;							//How many modules are on top and bottom side

	for( i = 0; i < Modules; i++ )				//Scan through the ModuleArray and
		if( ModuleArray[i].layer == layer)		//Count the number on top
			TopModules++;

	return( TopModules );
}

/*
 * split the module array into top and bottom sides
 * return with the number of modules on top (because bottommodules = modules - topmodules )
 */

//int	SplitTopBottom( int Modules, int TopModules, struct KiCadModule *ModuleArray, struct KiCadModule *TopModuleArray, struct KiCadModule *BottomModuleArray  )
void	SplitTopBottom( struct RenumParams *Params )
{
int	i, j = 0, k = 0;

	for( i = 0; i < Params->Modules; i++ )								//Scan through the ModuleArray and
		if( Params->ModuleArray[i].layer == 'F') 						//Count the number on top
			CopyKiCadModuleArrayElement( &Params->TopModuleArray[ j++], &Params->ModuleArray[i], (float) 0.0 );
		else if( Params->ModuleArray[i].layer == 'B' )
			CopyKiCadModuleArrayElement( &Params->BottomModuleArray[ k++], &Params->ModuleArray[i], ( float) 0.0 );

	if(( j + k ) != Params->Modules )
		printf("\nWarning: %d modules neither top nor bottom ",  Params->Modules - (j + k ));

	Params->TopModules = j;

}

/*
 * This creates a KiCadModulearray terminated with index -1 from the ModuleArray
 * It sorts it by X ascending, then copies and sorts each X (row) by Y ascending
 */
//void	SortKiCadModules( struct KiCadModule *modulearray, struct RenumParams *Params, int modules, int sortcode )


void	SortKiCadModules( struct RenumParams *Params, char Side )
{
int		i,	j, scanindex = 0, tmpindex;
float	min, srcval;					//Assume board less than 10M wide

struct KiCadModule *modulearray;
int     modules;
int     sortcode;

		if( 'F' == Side )
		{
			modulearray = Params->TopModuleArray;
			modules = Params->TopModules;
			sortcode = Params->topsortcode;
		}
		else
		{
			modulearray = Params->BottomModuleArray;
			modules = Params->BottomModules;
			sortcode = Params->bottomsortcode;
		}


int		sortxy = ((( sortcode & SORTYFIRST ) == 0 ) ? SORTX : SORTY );
int 	firstdirection = ((( sortcode & DESCENDINGFIRST ) == 0 ) ? ASCENDING : DESCENDING );
int 	secondirection = ((( sortcode & DESCENDINGSECOND ) == 0 ) ? ASCENDING : DESCENDING );

struct 	KiCadModule sortarray0[ modules + 1 ];		//Make a temporary array for x sort
struct 	KiCadModule sortarray1[ modules + 1 ];		//Make a temporary array for y sort

		CopyKiCadModuleArray( sortarray0, modulearray, modules, Params->grid );
		sortarray0[modules].index = -1;							//End the array
		SortOnXY( sortarray0, sortxy, firstdirection, Params->grid );	//Sort on the first order (x or y)
//
// Now make another sort array copy, this time with the next order
//
		for( i = 0; i < modules; i++ )
		{
			j = sortarray0[i].index;							//The index gives the sequence for the destination
			CopyKiCadModuleArrayElement( &sortarray1[i], &modulearray[j], Params->grid  );
			sortarray1[i].index = j;
		}

		sortarray1[i].index = -1;										//End the array

		while( scanindex <= modules )							//Do until the end
		{
			min = sortarray1[scanindex].coord[sortxy];			//Look for this X or Y
			for( i = scanindex;  i <=  modules; i++)
			{
				srcval = sortarray1[i].coord[sortxy];

				if((( firstdirection == ASCENDING ) && ( srcval > min ))  		//+/- the grid value
						|| (( firstdirection == DESCENDING ) && ( srcval < min ))) 	//If descending the first go
				{
					tmpindex = sortarray1[i].index;				//Save the place
					sortarray1[i].index = -1;					//Put in a fake end

					SortOnXY( &sortarray1[scanindex], ( sortxy ^ 1), secondirection, Params->grid );
					sortarray1[i].index = tmpindex;				//Reinstate the index
					scanindex = i;								//Look from here next time
					break;										//Get out of the for loop
				}
			else if( sortarray1[i].index == -1 )			//the last row
			{
				SortOnXY( &sortarray1[scanindex], ( sortxy ^ 1), secondirection, Params->grid );			//Sort on Y coordinates from here
				scanindex = modules + 1;					//Break the while
			}
		}
	}
	CopyKiCadModuleArray( modulearray, sortarray1, modules, Params->grid );			//Copy the sorted array to the module array
} //SortKiCadModules


void	ClearKiCadFileInMemoryStruct( struct KiCadFileInMemory *FileList, int NumberOfEntries )
{
int	i;

	for( i = 0; i < NumberOfEntries; i++ )			//Initialize the array
	{
		memset( &FileList[i].filebuffer, 0, MAXPATH );				//Make sure all names are null
		FileList[i].filebuffer = NULL;
		FileList[i].maxfilesize = 0;
	}
}

//
// If the extension is in the passed filename, remove it
//

void    TrimExtension( char *filename, char *extension )
{
char    *pntr = strrchr( filename, '.');         //Look for extension from the right
        if( NULL != pntr )                      //There is an extension
            if( 0 == strcasecmp( pntr, extension ))
                *pntr = '\0'; //and it is extension
}

/*
 * This is a recursive function!
 * It loads a sheet into memory and scans it for $Sheet(s) and
 * 	1) counts the number of sheets (storage allocated = MAXPATH * number of sheets
 * 	2) if the SheetNamePointer != NULL it copies the schematic.sch to the memory pointed to by
 * 		SheetNamePointer, increments the pointer ( next pointer in array) and saves the address
 * 		after the '\0' in the next pointer location and puts a '\0' there
 *
 *      List of files will be replaced by eeSchema built in function sch_sheet_path.cpp and others
 */
void	CrawlSheets( char *path, char *SheetName, struct KiCadFileInMemory *FileList, int *NumberOfSheets )
{

int		i, sheets;						//Sheets and Bytes in all the sheets
char	*scanner; 						//Load this file and extension

char    filename[ MAXPATH * 2 ];               //room for the complete filename

//
// Now look through all the sheet names to see if this is unique.
// If so add to the end of the list
//
        TrimExtension( SheetName , ".sch");               //If it ends in sch or pro, trim
        TrimExtension( SheetName , ".pro");

        MakeFileName( filename, path, SheetName, ".sch");       //Default sheet has .sch so trim then add

        i = 0;
	    if( FileList != NULL )          //If I am not just counting
	    {
		    while( '\0' != FileList[i].filename[0] )							//Until the no strings to look at
		        if( strcmp( filename, FileList[i].filename ) == 0 ) break;
				    else i++;													//Look next

		    if( '\0' == FileList[i].filename[0] )								//Got to the end
		        strcpy( FileList[i].filename, filename );                       //Copy the name into the last spot

	    }

char    *buffer = LoadFile( filename, (FileList == NULL ? NULL : &(FileList[i].maxfilesize)) );		//Load this file and extension, get file size

int	    numrefdes = CountStrings( buffer, "$Comp" );						//How many components in this schematic
        sheets = CountStrings( buffer, "$Sheet" );							//How many sheets are there in this schematic

        if( FileList != NULL )
            FileList[i].maxfilesize += (long) (( numrefdes * 2) * MAXREFDES );			//The file can't be any bigger than this (i.e. components * 128 )

        if( 0 == sheets )				//No more sheets so finis
        {
            free( buffer );
            return;
        }

        *NumberOfSheets += sheets;							//Accumulate the number of sheets

        struct	KiCadFileInMemory CrawlFileList[ sheets + 1 ];
        ClearKiCadFileInMemoryStruct( CrawlFileList, sheets );

        scanner = buffer;							//Start over
        for( i = 0; i < sheets; i++ )				//This makes an array of names of files
            scanner = ScanForSheets( scanner, &CrawlFileList[i] );

        free( buffer );				//I am now done with that file for name scanning

        for( i = 0; i < sheets; i++ )	//Now crawl through each sheet
            CrawlSheets( path, CrawlFileList[i].filename, FileList, NumberOfSheets );
} //NuCrawlSheets()

//
// Returns the number of times the string is found in the buffer
//
int	CountStrings( char *buffer, char *string )
{
int	count = 0;

	while( NULL != buffer )
	{
		buffer = strstr( buffer, string );
		if( NULL != buffer )
		{ count++;
		  buffer += sizeof( string );
		}
	}

	return( count );

}

/*
 * Scan for each sheet by name. If $Sheet is found, pull in the name
 * If sheetnamedest != Null copy it else just count (storage of sheetname assumed to be MAXPATH
 * Return pointer past $EndSheet if found else NULL
 */

char	*ScanForSheets( char *scanner,  struct KiCadFileInMemory *SheetStruct )
{
char	*endsearch;
char	tmpc;

	scanner = strstr( scanner, "$Sheet" );				//Look for a sheet
	if( scanner != NULL )								//I found something
	{
		endsearch = strstr( scanner, "$EndSheet");		//Look no further
		if( endsearch == NULL )							//Bad file format
			return( NULL );

		tmpc = *endsearch;								//Save the character
		*endsearch = '\0';								//End the search here
		PullFieldString( SheetStruct->filename, scanner, "F1" );
		*endsearch = tmpc;								//Restore the byte
		scanner = endsearch + sizeof( "$EndSheet" );	//Skip to the end
	}
	return( scanner );									//Look from here
}



/*
 * Add if this is a new sheet name otherwise ignore
 * SheetNameList is a buffer with sheet names MAXPATH long
 */

void	AddSheetName( char *SheetName, char *SheetNameList )
{
int	found = 0;

	if( SheetNameList == NULL ) return;

		while(( '\0' != *SheetNameList ) && ( 0 == found ))		//Until the no strings to look at or found
			if( strcmp( SheetName, SheetNameList ) == 0 )
				found = 1; 	//Look for the sheet name in the array of names
			else SheetNameList += MAXPATH;										//Look next

		if( found == 0 )		//This is a new string i points past
			strcpy( SheetNameList, SheetName );			//Copy the name into the SheetNameBuffer
} // AddSheetName()


/*
 * Locate a field with text SearchFor and find the starting '"'
 * Append to dest up and including the quote
 * Copy the quoted test into fieldptr
 * Return pointer to the last '"'
 */

char	*CopyCompField( char *linestart, char *lineptr, char *dest, char *fieldptr )
{
char	tmpc;

		while(( *lineptr != '"' ) && ( *lineptr != '\0')) lineptr++;		//Skip to "
		++lineptr;					//Skip the quote (doesn't matter if it is '\0'
		tmpc = *lineptr;
		*lineptr = '\0';				//Terminate for the strcat
		strcat( dest, linestart );		//Copy up to here to destination
		*lineptr = tmpc;

		while(( *lineptr != '"' )  && ( *lineptr != '\0')) *fieldptr++ = *lineptr++;	 //copy the field
		*fieldptr = '\0';
		return( lineptr );			//Return where you ended up
}

/*
 * Find the schematic component in the RefDesCHangeArray and append it to the destination
 *
 */

int		FindSchematicComponent( struct RenumParams *Params , char *deststr, char *field, int *err_report )
{
int	i;
char	NewRefDes[MAXREFDES * 3];		//Store the component here

	for( i = 0; i < Params->Modules; i++) 							//Search through the change array
	{
		if( strcmp( field, Params->RefDesChangeArray[i].OldRefDesString	) == 0 )		//If the string is found
		{	//This is what I want to substitute
			if( Params->RefDesChangeArray[i].OldRefDesNum == SKIPREFDES )						//Do not substitute with a new refdes
				strcpy( NewRefDes, Params->RefDesChangeArray[i].OldRefDesString );		//Basically do nothing
			else
			{ //The component has been found
				MakeNewRefDes( NewRefDes, &Params->RefDesChangeArray[i] );
				Params->RefDesChangeArray[i].Found++;							//Found it at least once.
			}
			break;			//Out of the search loop
		}
	}
	if(( 0 == *err_report) && ( i >= Params->Modules ))	//Not found in RefDesChangeArray (only do this one per $comp
	{
			*err_report += 1;
			printf("\nSchematic refdes %s not found in change array!", field );		//In case of PWR, etc..
			++Params->errcount;
			strcpy( NewRefDes, field );
			strcat( NewRefDes, "-NOTFOUND" );
	}

	NewRefDes[MAXREFDES] = '\0';			//As long as it gets
	strcat( deststr, NewRefDes );			//And put it here
	return(( i >- Params->Modules )? 0: -1 );
} //FindSchematicComponent()


/*
 * Process the input component to the output component with the RefDesChangeArray
 *
 * source points to $comp .... $endcomp zero terminated
 * dest points to the output buffer
 *
 * Process one line at a time, make substitutions if necessary, and copy to the output buffer
 *
 */

void	ProcessComponent( struct RenumParams *Params, char *dest, char *source )
{
int		buflen = strlen( source );		//How long is it?

char	aline[buflen + 1];			//Where each line is kept (can't be longer than the source)
char	field[buflen + 1];			//Where the field ( ) is kept
char	*lineptr, *fieldptr;
char	tmpc;					//Used to zero terminate

int		i = 0, j = 0, err_report = 0;

		do
				if(( '\n' == source[i] ) || ( '\r' == source[i] )) j++; //Count #lines (no more than one refdes per line
		while( '\0' != source[i++] );

char	tmpdest[ buflen + ( MAXREFDES * j ) + 1 ];		//Room for substitution

	while( *source != 0 )					//Until the end of the $comp ... $endcomp
	{
		fieldptr = field;
		lineptr = aline;					//Point to start of line
		*lineptr = '\0';					//Zero terminate
		*fieldptr = '\0';
		tmpdest[0] = '\0';

		while(( *source != '\n' ) && ( *source != '\r') && ( *source != '\0')) 		//Copy until newline or line feed
				*lineptr++ = *source++;
		while((( *source == '\n' ) || ( *source == '\r')) && ( *source != '\0'))	//Copy the newline and line feed
				*lineptr++ = *source++;

		*lineptr = '\0';							//Terminate the end of the line
		field[0] = '\0';							//No field found
		lineptr = aline;							//Point to start of line

		if( *lineptr == 'L')						//Is this the first line? (ie. L LED D201 )
		{
			lineptr += 2;
			while(( *lineptr != ' ' ) && ( *lineptr != '\0')) lineptr++;		//Skip to non space
			tmpc = *++lineptr;				//Skip the space and get the next character
			*lineptr = '\0';
			strcat( tmpdest, aline );		//Copy up to here to destination
			*lineptr = tmpc;
			while(( *lineptr > ' ' )  && ( *lineptr != '\0')) *fieldptr++ = *lineptr++;	 //copy the field
			*fieldptr = '\0';		//Zero terminate
		}
		else if ( strstr ( lineptr, "F 0") == lineptr )		//"F 0" at start of line (i.e. F 0 "D501" H 5600 4025 50  0000 C CNN )
		{
			lineptr += ( sizeof( "F 0") - 1 );
			lineptr = CopyCompField( aline, lineptr, tmpdest, field ); //Get the field starting at "
		}
		else if ( strstr ( lineptr, "AR") == lineptr )		//"AR" at start of line (i.e. AR Path="/579A9AFE/579AA2E5" Ref="D201"  Part="1" )
		{
			lineptr = strstr( lineptr, "Ref=" ) + sizeof( "Ref=") - 1;			//Find the Ref= tag
			lineptr = CopyCompField(  aline, lineptr, tmpdest, field ); 			//Get the field starting at "
		}
		else strcat( tmpdest, aline );					//No fields (L, F0, AR) found just copy it

		if( field[0] != '\0' )						//I found something
		{
			if( field[0] == '#')						//Fake refdes (#pwr, etc
			{	strcat( tmpdest, field );					//Copy the fake refdes
				strcat( tmpdest, lineptr );					//Append after field
			}
			else
			{	//Find the component and copy it to tmprefdes
				if( 0 == FindSchematicComponent( Params, tmpdest, field, &err_report )) //Change to new refes
					strcat( tmpdest, lineptr );					//Append after the second "
				else
					printf("\nComponent not found");
			}
		}
		strcat( dest, tmpdest );
	}
} //ProcessComponent


/*
* This is a consolidated and simplified search and replace (rather than two passes for modules and nets )
*/
//void	UpdatePCBFileRefDes( struct RenumParams *Params, struct ProjectFileStruct *ProjectFile , int modules,
//			struct RefDes *RefDesChangeArray, struct	KiCadFileInMemory *FileList, char *buffer )

void	UpdatePCBFileRefDes( struct RenumParams *Params, struct	KiCadFileInMemory *FileList )
{
char	*head = Params->pcbbuffer;				//Where to start or end writing.
char	*fp_text_ptr, *Net_ptr;
char	*changestr, *srcnptr, delimiter;

char 	OldRefDes[MAXREFDES+1];		//What the OldRefDes is (i.e. U, R VR, etc.)
char	NewRefDes[ 3 * MAXREFDES];		//Where the new refdes goes

int 	i, no_fp = 0, no_net = 0;	//Clear the no more flags
int		nummodules = 0;				//Number of refdes found
int		numnets = 0;				//Number of nets found

		MakeFileName( FileList->filename, Params->projectpath, Params->projectfilename, ".kicad_pcb" );			//Make the full file name - even though it is in memory
		FileList->filebuffer = calloc( FileList->maxfilesize, sizeof( char ) ); //Allocate room for the output

char	*outbuffer = FileList->filebuffer;

        if( 0 != (Params->log & LOGCHANGE))
		{
            fprintf(Params->loghandle, "\n\n\n*********** Change List ***********");
            fprintf(Params->loghandle, "\n#          Was     Is        Type");
		}

		while( *head != 0 )	//Until the end of the file
		{
			if( no_fp == 0 ) 										//If it isn't there once, it'll never be there
				fp_text_ptr = strstr( head, "fp_text reference");	//Find this field
			if( no_net == 0 )
				Net_ptr = strstr( head, "\"Net-(");					//Find that field

			if( fp_text_ptr == NULL ) no_fp = 1;					//No more fp_text references
			if( Net_ptr ==  NULL ) no_net = 1;						//No more Net-(

			if(( no_fp * no_net) == 1 ) break;						//Neither found so I am finished - just flush
			if(( no_fp + no_net ) == 0 )							//If neither are NULL
			{
				if( fp_text_ptr < Net_ptr )							//fp_text_ptr is first
					Net_ptr = NULL;									//Ignore the latter
				else
					fp_text_ptr = NULL;
			}

			if( fp_text_ptr == NULL )
			{ //I found the "Net-(" field
				srcnptr = Net_ptr + sizeof( "\"Net-(" ) - 1;				//Head is now after the string
				changestr = "Net ";
				delimiter = '-';//Delimiter for old text reference
			}
			else	//I found the "fp_text reference" field
			{
				srcnptr = fp_text_ptr + sizeof( "fp_text reference " ) - 1;	//Head is now after the string
				changestr = "Ref ";
				delimiter = '\0';								//Delimiter for old text reference
			}

			OldRefDes[0] = *srcnptr;						//Save the character in the old ref des array
			*srcnptr = '\0';								//Zero terminate
			outbuffer = CopyBuffer( outbuffer, head );							//Write out what you got to here
//
// Save the old refdes just in case there is no change
//
			i = 1;												//Get the rest of the old reference designator
			while(( i < MAXREFDES) && ( srcnptr[i] > ' ')
				&& ( srcnptr[i] != delimiter ) && srcnptr[i] != '\0' )		//Until the dash usually
			{
				OldRefDes[i] = srcnptr[i];						//Copy the Old Ref Des and skip it
				i++;											//
			}
			OldRefDes[i] = '\0';								//End the string
			head = srcnptr + i;									//Remember where to pick this up

			for( i = 0; i < Params->Modules; i++)					//Search the change array
			{
				if( strcmp( OldRefDes, Params->RefDesChangeArray[i].OldRefDesString	) == 0 )
				{	//This is what I want to substitute
					if( delimiter == '-' )numnets++;		//One more net
					else nummodules++ ;						//or module

					if( Params->RefDesChangeArray[i].OldRefDesNum == SKIPREFDES )		//Do not substitute with a new refdes
						outbuffer = CopyBuffer( outbuffer, OldRefDes );							//So write out the old one
					else	//Replace the refdes string
					{	//Found the new refdes so write that out
						MakeNewRefDes( NewRefDes, &Params->RefDesChangeArray[i] );
						outbuffer = CopyBuffer( outbuffer, NewRefDes );							//Add to the output file
					}

					if( 0 != (Params->log & LOGCHANGE))
					{
			                NewLineNumber(Params, i, 5 );
			                LineItUp( Params, OldRefDes, 16  );
			                LineItUp( Params, NewRefDes, 16  );
			                LineItUp( Params, changestr, 8  );
					}

					if( Params->RefDesChangeArray[i].OldRefDesNum == SKIPREFDES )
					{    if( 0 != (Params->log & LOGCHANGE))
	                        fprintf( Params->loghandle, "*** will be ignored ***");
					}

					break;			//Out of the for search loop
				}
			}	//for
			if( i >= Params->Modules )					//If not found just write the old ref des
			{
				outbuffer = CopyBuffer( outbuffer, OldRefDes );		//Write out what was the old refdes
				if( delimiter == '\0' )			//If a module
				{	if( Params->errcount++ < 20 )
						printf("\n  PCB Update warning module refdes %s not found in change array", OldRefDes );

					if( Params->errcount== 20 ) printf("\n Too many errors - stop reporting them ");
				}
			}
		}//While
		outbuffer = CopyBuffer( outbuffer, head );								//Flush the rest of the file
		FileList->maxfilesize = ( outbuffer - FileList->filebuffer );			//Actual Bytes transfered

}//long int	UpdatePCBFileRefDes( struct ProjectFileStruct *ProjectFile , int modules, struct RefDes *RefDesChangeArray, char *outbuffer, char *buffer )



/*
 *  * Scan through the Schematic file and substitute RefDes Old for RefDes New
 * 	The input file name has already been determined by the hierarchy scan so no need to add .sch
 *	The new file is buffered in FileList.buffer
*/

void	UpdateSchematicFileRefDes( struct RenumParams *Params, struct KiCadFileInMemory *FileList )
{

char	*found, *nextcomp;							//Where stuff is
char	tmpc;
char	*compend;

char    schfilename[MAXPATH];
char    tmpschname[MAXPATH];

        strcpy( tmpschname, FileList->filename );
        TrimExtension( tmpschname, ".sch" );            //remove implied extension if present

		MakeFileName( schfilename, Params->projectpath, tmpschname, ".sch" );
		FileList->filebuffer = calloc( FileList->maxfilesize, sizeof( char ) ); //Allocate room for the output
char	*outbuffer = FileList->filebuffer;						//Where stuff goes

char	*buffer = LoadFile( schfilename, NULL );			//Load the file but do not adjust the size
char	*head = buffer;											//Where to start or end writing.

		nextcomp = strstr( buffer, "$Comp");			//Use this to find where the next module starts
		while( nextcomp != NULL )	//Scan through the buffer (PCBFIle) and replace old refdes with new ones.
		{
			found = nextcomp; 								//Find the "$Comp" token
			{
				outbuffer = CopyHereToThere( outbuffer, head, found );
				compend = strstr( found, "$EndComp") + sizeof( "$EndComp" ) - 1;			//Find the end of the component
				head = compend;											//Pick up from after $EndComp

				tmpc = *compend; *compend = '\0';						//Zero terminate the string rather than copy it
				ProcessComponent( Params, outbuffer, found  );			//Processes from $Comp ... $Endcomp
				*compend = tmpc;										//Restore the string
				while( '\0' != *outbuffer ) ++outbuffer;				//Find the end of what was just added
				nextcomp = strstr( found + sizeof("$Comp"), "$Comp");			//Use this to find where the next module starts
			}
		}// while

		outbuffer = CopyBuffer( outbuffer, head );	 					//Add to the output file
		FileList->maxfilesize = outbuffer - FileList->filebuffer;		//Number of bytes

		if( NULL != buffer ) free( buffer );
	printf("\nSCH Updated %s size %ld", schfilename, FileList->maxfilesize );

} //UpdateSchematicFileRefDes()


/*
 *  Scan through the Netlist file and substitute RefDes Old for RefDes New
 *  Netlist update is simple Nets are all like this (ref C18)
 *  So if the new ref des is C123 I replace C18 with C123
 *
 * (components
 *   (comp (ref C12)
 *
 * and
 *
 *    (net (code 1) (name FPGA_87)
 *     (node (ref U8) (pin 4))
 *     (node (ref U6) (pin 87)))
 *
 */

void		UpdateNetlistRefDes( struct RenumParams *Params, struct KiCadFileInMemory *FileList )
{
char	*found, *nextnet;			//Where stuff is

char	OldRefDes[3*MAXREFDES];			//Where to store the old ref des for the net (i.e. D2)
char	NewRefDes[3*MAXREFDES];			//Where to store the old ref des for the net (i.e. D2)
int		i;

		MakeFileName( FileList->filename, Params->projectpath, Params->projectfilename, ".net" );
		if( 0 != access(FileList->filename, F_OK)) //File not found
		{
			printf("\n*** Netlist file %s not found or empty", FileList->filename );
			FileList->filebuffer = NULL;
			FileList->filename[0] = '\0';
			FileList->maxfilesize = 0;
			return;
		}

char	*buffer = LoadFile( FileList->filename, &(FileList->maxfilesize) );		//Load this file and extension, get file size

		FileList->maxfilesize += (2 * MAXREFDES * CountStrings( buffer, "(ref " )); //How many text reference fields
		FileList->filebuffer = calloc( FileList->maxfilesize, sizeof( char ) ); 	//Allocate room for the output

char	*head = buffer;										//Where to start or end writing.
char	*outbuffer = FileList->filebuffer;

		nextnet = strstr( buffer, "(ref ");				//Use this to find where the ref name
//
// Now work through the netlist file until no more "(ref "
//
		while( nextnet != NULL )							//Scan through the buffer and replace old refdes with new ones.
		{
			if( nextnet != NULL )								//I did find a net ...
			{
				found = nextnet + sizeof("(ref ") - 1;  								//Find the "(ref " token at found
				outbuffer = CopyHereToThere( outbuffer, head, found );							//Write out what you got to here

				i = 0;
				while(( *found != ')') && ( i < MAXREFDES ))
						OldRefDes[i++] = *found++;			//Copy the text after "(ref " until the ")")

				OldRefDes[i] = '\0';							//End the string. nextnet points to the closing );

				if( i >= MAXREFDES )
					RenumError( "Invalid Netlist format!", BADNETLIST );	//Write out to this point

//
// OldRefDes now has the name of the old reference designation for the net. Find it in the change array
//
			for( i = 0; i < Params->Modules; i++)					//Search the change array
			{
				if( strcmp( OldRefDes, Params->RefDesChangeArray[i].OldRefDesString	) == 0 ) //Look up in the change array
				{	//This is what I want to substitute
					if( Params->RefDesChangeArray[i].OldRefDesNum == SKIPREFDES )		//Do not substitute with a new refdes
							outbuffer = CopyBuffer( outbuffer, OldRefDes );						//So write out the old one

					else	//Replace the refdes string
					{		//Found the new refdes so write that out
						MakeNewRefDes( NewRefDes, &Params->RefDesChangeArray[i] );
						outbuffer = CopyBuffer( outbuffer, NewRefDes );						//So write out the new one
					}
						break;			//Out of the for search loop
				}	//New ref des has been substituted
			}	//for

			if( i >= Params->Modules ) printf("\n****** Warning net %s not found in update table ", OldRefDes );
//
//	nextnet points to the input buffer where the ")" of the (net C6) is
//
			head = found;										//Start looking from here (the ')' from (ref C26)
			nextnet = strstr( found, "(ref ");				//Use this to find where the ref name
			}
		}// while
/*
* All the modules have been found and replaced. Now write out the end of the file
*/
		outbuffer = CopyBuffer( outbuffer, head);						//So write out the rest
		FileList->maxfilesize = ( outbuffer - FileList->filebuffer );		//How many bytes transfered
} //UpdateNetlistRefDes()


/*
 * Enter a $Sheet structure which is zero terminated.
 * Find the field, then a '"'. Copy from the '"' to the next quote
 *
 * Bug fig (Bug reported by Sean T Happel who supplied a corrected PullFieldString Function)
 *
 * The field has to be preceded by a new line !
 *
 *
 * return NULL if not found
 */
void    PullFieldString( char *dest, char *buffer, char *instr )
{
char    *found_start, *found_end;

    	*dest = '\0';    //Assume not found
int    	instr_len = strlen(instr);

    	for(;buffer != NULL; buffer = strchr(buffer, '\n')){ // Start looking at the beginning of the buffer, then at each subsequent newline (windows uses \r\n, so \n will still start beyond newline)
        while(isspace(*buffer)) buffer++;					// Skip any leading whitespace - including the previous \n found

        if(strncmp(buffer, instr, instr_len) == 0)			// If appropriate field is found
        {
        	if(NULL == (found_start = strchr(buffer, '\"'))) return;

        	found_start++;

            if(NULL == (found_end = strchr(found_start, '\"'))) return;

            memcpy(dest, found_start, found_end - found_start);
            dest[found_end - found_start] = '\0';

            return;
        }
    }
    return;
} //PullFieldString


//
// Copy zero terminated input to outbuffer
// Return adjusted outbuffer pointer
char *CopyHereToThere( char *dest, char *from, char *to )
{
char tmpc = *to;
	*to = '\0';				//Zero terminate
	while( *from != '\0') *dest++ = *from++;
	*to = tmpc;
	return( dest );
}

//
// Make a ref dest string consisting of [prepend][refdestype][refdes]
//
// If the prepend starts with -, remove the prepend from the refdestype
//
// fnork
void	MakeNewRefDes( char *NewRefDes, struct RefDes *RefDesChangeArray )
{
char *prepnt = RefDesChangeArray->prepend;
    strcpy( NewRefDes, prepnt );                            //Make a copy of the prepend string
    strcat( NewRefDes, RefDesChangeArray->RefDesType );     //And the type (eg C )

    if( '-' == *prepnt )                //If stripping the prepend string
	{
	   ++prepnt;    //After the '-'
	   if( 0 == strncmp( prepnt, RefDesChangeArray->RefDesType, strlen( prepnt )))   //If the prepend was there
	      strcpy( NewRefDes, &RefDesChangeArray->RefDesType[ strlen( prepnt)]);	//Remove the prepend.
	}
	RefDesToString( NewRefDes, RefDesChangeArray->NewRefDesNum );	//Now the new refdes (1, 2, ...)
	NewRefDes[MAXREFDES] = '\0';								//Maxlength
}

//
// Append int to ascii string
//
void	RefDesToString( char *outstr, int refdes )
{
char	tmpstr[6];		//Max 100,000
int		i;

	for( i = 0; i < 6; i++ ) tmpstr[i] = '\0';	//Zap the string

	for( i = 0; i < 6; i++)
	{
		tmpstr[i] = '0' + refdes % 10;		//Get the lsd, convert to ascii
		refdes /= 10;						//Divide by 10
		if( 0 == refdes ) break;			//All done
	}
	i++;									//Always have at least one digit
//
//tmpstr has a palindron of the string, i has the number of digits
//
	outstr += strlen( outstr );				//Find the end
	do *outstr++ = tmpstr[--i]; while( 0 != i );	//Copy to the output;
	*outstr = '\0';							//End the string
}



//
// Copy zero terminated input to outbuffer
// Return adjusted outbuffer pointer

char	*CopyBuffer( char *outbuffer, char *input )							//Write out what you got to here
{
	while( *input != '\0') *outbuffer++ = *input++;
	return( outbuffer );
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	UI Functions
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* Parse the string to determine the sort direction
*
* Format is [X/Y][A/D][A/D] (case doesn't matter)
*
*/
struct	MenuChoice {
	char	*string;
	char	*abbreviation;
	int		code;
};

struct	MenuChoice G_DirectionsArray[] =
{
		{"Top to bottom, left to right", "TBLR", SORTYFIRST + ASCENDINGFIRST + ASCENDINGSECOND },
		{"Top to bottom, right to left", "TBRL", SORTYFIRST + ASCENDINGFIRST + DESCENDINGSECOND },
		{"Bottom to top, left to right", "BTLR", SORTYFIRST + DESCENDINGFIRST + ASCENDINGSECOND },
		{"Bottom to top, right to left", "BTRL", SORTYFIRST + DESCENDINGFIRST + DESCENDINGSECOND },
		{"Left to right, top to bottom", "LRTB", SORTXFIRST + ASCENDINGFIRST + ASCENDINGSECOND },
		{"Left to right, bottom to top", "LRBT", SORTXFIRST + ASCENDINGFIRST + DESCENDINGSECOND },
		{"Right to left, top to bottom", "RLTB", SORTXFIRST + DESCENDINGFIRST + ASCENDINGSECOND },
		{"Right to left, bottom to top", "RLBT", SORTXFIRST + DESCENDINGFIRST + DESCENDINGSECOND },
		{ NULL, NULL, -1 }
};

struct	MenuChoice G_ModulesRefDesArray[] =
{
		{"Reference Designation coordinates", "", 0 },
		{"Module coordinates", "", 1 },
		{ NULL, NULL, -1 }
};


char	*NewParameterList[] =
{
	"PCB file name",							//0
	"Root schematic file name",					//1
	"Netlist file name",						//2
	"Top Sort Direction",						//3
	"Bottom Sort Direction", 					//4
	"Top Start Reference Designation", 			//5
	"Bottom Start Reference Designation",		//6
	"Sort on Modules/Reference Designators",	//7
	"Sort Grid",								//8
    "Log",                                      //9         This is read only (a hack, I know ...)
	""
};




void	SetSortDirection( char* argv, int *SortCode )
{
struct	MenuChoice *DirArray = &G_DirectionsArray[0];
int		i = 0;

	do
	{
		if( strcmp( DirArray[i].abbreviation, argv ) == 0 )
			*SortCode = DirArray[i].code;
	}while( DirArray[i++].abbreviation != NULL );
}

/*
 * Return a string which is in the same location as the code
 */
char	*GetMenuString( struct	MenuChoice *choices, int code )
{
int		i = 0;
		do
		{
			if( choices[i].code == code ) return( choices[i].string );
		} while( choices[i++].code != -1 );

		return( "Invalid menu code ");
} //GetSortString

/*
 * Delete to the right of the cursor
 */
int	DeleteRight( char *tmpstr, int location )
{
int	i, j;

	i = location;					//Save the location
	j = strlen( tmpstr );			//Length of original string

	if(( i < j ) && ( tmpstr[i] != '\0'))	//Nothing is a blank strung
	{
		for( ; i < j; i++ ) tmpstr[i] = tmpstr[i+1];

		for( i = 0; i < location; i++ ) putchar( 0x08);		//Back space all the way to the start
		printf("%s", tmpstr );								//And show the new string

		if( j > strlen( tmpstr ) )							//If it hangs over
			for( i = 0; i < ( j - strlen( tmpstr )); i++ ) putchar( ' ');				//Clear the line
		for( i = 1; i <= ( j - location ); i++ ) putchar(0x08);		//Back space to where you where
	}
	return( location );
}


/*
 * Delete to the left of the cursor
 */
int	DeleteLeft( char *tmpstr, int location )
{
int	i, j;

	if( location > 0 )						//Not if at first character
	{
		i = location;						//Save the location
		j = strlen( tmpstr );				//Length of original string

		if( i == j ) tmpstr[--i] = '\0';
		else
			for( ; i <= j; i++ ) tmpstr[i-1] = tmpstr[i];		//Copy left

		for( i = 0; i < location; i++ ) putchar( 0x08);		//Back space all the way to the start
		printf("%s", tmpstr );								//And show the new string

		if( j > strlen( tmpstr ) )							//If it hangs over
				for( i = 0; i < ( j - strlen( tmpstr )); i++ ) putchar( ' ');				//Clear the line
		for( i = 0; i <= ( j - location ); i++ ) putchar(0x08);		//Back space to where you where
		--location;
	}
	return( location );
}

#ifdef		Windows					//Windows

int	getchEASCII( void )
{
	return( getch() );

}

#else

/*
* Deal with wonky Linux getch()
* reads from keypress, doesn't wait for a CR
*/

int getch(void)
{
struct termios oldattr, newattr;
int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return( ch );
}


/*
* Deal with Linux escap sequences
*/

int	getchEASCII( void )
{
	if( getch() != '[') return( 0 );			//If not a proper escape sequence

int	i = getch();							//Get the second part
	if( i == 'D' ) return( LEFTARROW );
	if( i == 'C' ) return( RIGHTARROW );
	if(( i == '3' ) && (getch() == '~' )) return( DEL );
	return( 0 );
}

#endif

/*
 * My getch handles extended ascii
 */

int	mygetch( void )
{
int	i = getch();
	if( i == EASCII )
		i = ( getchEASCII() | KEYESCAPE );
	return( i );
}
/*
 * Get a string of maximum size (terminal does most of the editing )
 */

void	StringGet( char	*prompt, char *dest, char *original, int maxsize )
{
int		tmp;
char	tmpstr[maxsize+1];

int	i;
	for( i = 0; i < maxsize; i++ ) tmpstr[i] = '\0';		//Ensure an end
	SafeStringCopy( tmpstr, original, maxsize );

	i = strlen( tmpstr );
	printf("\n%s %s", prompt, tmpstr );
	do	//Get with some editing
	{
		tmp = mygetch();				//Get a character including extended ascii
		if(( tmp >= ' ' ) && ( tmp < 0x7f )) 		//If a printable
		{	putchar( tmp );
			tmpstr[i++] = (char) tmp;				//Save it
		}

		else //Maybe some editing to do
		{
			switch ( tmp )
			{
				case BS :			//Destructive backspace
					i = DeleteLeft( tmpstr, i );
				break;

				case LEFTARROW :	//Move left
					if( i > 0 )
					{
						printf("\x08");
						--i;
					}
				break;

				case RIGHTARROW :	//Move right
					if( tmpstr[i] != '\0' )
						printf("%c", tmpstr[i++] );
				break;

				case DEL :			//Delete
					DeleteRight( tmpstr, i );		//Deleted to the right of the cursor
					break;

			}
		}
	} while(( i < maxsize ) && ( tmp != ABORT ) && ( tmp != CR ));

	if( tmp != ABORT )
		SafeStringCopy( dest, tmpstr, maxsize );
} //StringGet()


/*
 * Select a choice from the menu
 */

void	MenuSelect( char *prompt, struct MenuChoice *choices, int *result )
{
int	i, j, tmpresult = *result;
int	tmpc, menuentry;

	printf("\nHit <space> to change, <cr> to select ");
	printf("\n%s ", prompt );

	do
	{
		menuentry = 0;
		do
		{
			if( choices[menuentry].code == -1 )
					{ menuentry = 0; break; }						//End of the menu so wrap
			if( choices[menuentry].code == *result ) break;			//found the spot
			menuentry++;
		}while( 1 );

		j = strlen ( choices[menuentry].string );	//How big is the related string?
		printf("%s", choices[menuentry].string );	//Show the string
		*result = choices[menuentry].code;					//Get this code

		tmpc = toupper( mygetch( ) );				//What to do?
		for( i = 0; i < j; i++ ) putchar('\b');		//First back up to the beginning of the string
		for( i = 0; i < j; i++ ) putchar( ' ');		//Clear the line
		for( i = 0; i < j; i++ ) putchar( '\b' );		//And back up to the beginning again

		if( tmpc == ' ')							//If a space go to next menu item
			*result = choices[menuentry+1].code;		//Get this code next time
		else if(( tmpc == CR ) || ( tmpc == ABORT )) 	//Abort or enter
			break;
	} while( 1 );



	if( tmpc == ABORT ) *result = tmpresult;			//Restore if it was abort.
}



//
// Chop up the file name into path, name, extension.
// Replace the extension with the argument, reconstruct the file name
// Check if the file name exists. Return 0 if it does not
//

int		CheckInputFileName( struct RenumParams *Params, char *extension )
{
char    filename[MAXPATH];
char    slashstr[2];
char    *lastslash= strrchr( Params->projectfilename, SLASHCHR );              //Path ends with the first slash from the right
char    tmpc;

Params->projectpath[0] = '\0';                                          //You got nothing!

        slashstr[0] = SLASHCHR;
        slashstr[1] = '\0';

        if( lastslash != NULL)                                      //I found a slash
        {
            tmpc = *lastslash;          //Save this
            *lastslash = '\0';          //Terminate
            strcpy( Params->projectpath, Params->projectfilename );
            *lastslash = tmpc;          //Restore

            if( Params->projectpath[strlen(Params->projectpath) - 1 ] != SLASHCHR )
                strcat( Params->projectpath, slashstr );                                 //Add a trailing slash
        }

		MakeFileName( filename, Params->projectpath, Params->projectfilename, extension );		//Make the full file name
		if( 0 != access(filename, F_OK))
		{
			printf("\n\nFile not found: %s\n\n", filename );
			return( 0 );				//Nada
		}
		return( 1 );
}

//
//  WriteParameterFile and LoadParameter are both made quick and dirty as they will be stripped out
//	Once I integrate with pcbnew
//
/*
 * char	*NewParameterList[] =
{
	"PCB file name",						//0
	"Root schematic file name",				//1
	"Netlist file name",					//2
	"Top Sort Direction",					//3
	"Bottom Sort Direction", 				//4
	"Top Start Reference Designation", 		//5
	"Bottom Start Reference Designation",	//6
	"Sort on Modules/Reference Designators",	//7
	"Sort Grid ",							//8
    "Log",                                      //9         This is read only (a hack, I know ...)
	"\0"
};

 *
 */
//
// This writes the parameter file into the local directory
//
void	WriteParameterFile( struct RenumParams *Params )
{
FILE	*WriteFile = fopen ( Params->paramfilename,"wb+");			//Open the file

		if (WriteFile  == NULL) RenumError( "Can't create parameter file!", PARAMWRITECREATEERROR );

		fprintf( WriteFile, "%s=%s\n", NewParameterList[0], Params->projectfilename);
		fprintf( WriteFile, "%s=%d\n", NewParameterList[3], Params->topsortcode);
		fprintf( WriteFile, "%s=%d\n", NewParameterList[4], Params->bottomsortcode);
		fprintf( WriteFile, "%s=%d\n", NewParameterList[5], Params->topstartrefdes);
		fprintf( WriteFile, "%s=%d\n", NewParameterList[6], Params->bottomstartrefdes);
		fprintf( WriteFile, "%s=%d\n", NewParameterList[7], Params->sortonmodules);
        fprintf( WriteFile, "%s=%f\n", NewParameterList[8], Params->grid);
        fprintf( WriteFile, "%s=%d\n", NewParameterList[9], Params->log);

		fclose( WriteFile );
	}	//WriteParameterFile()

//
// This loads the parameter file from the local directory and sets the values
//
void	LoadParameterFile( struct RenumParams *Params )
{
long	sizeparams;

		if( 0 != access(Params->paramfilename, F_OK)) return;		//File not found

char	*buffer = LoadFile( Params->paramfilename, &sizeparams );
char	dest[ sizeparams + 2];		 	 		// allocate memory to contain the whole file plus a '\0'
char	*cpnt;
char	*found;
int		i;
//
// Now i have the parameter file in buffer
//
char	*bufpnt = buffer;
		do			// Process a line at a time
		{														//Copy a string to a work buffer
			cpnt = strchr( bufpnt, '\n');						//Look for EOL
			if( NULL == cpnt )
				cpnt = strchr( bufpnt, '\0' );					//Not found: look for EOF

			if( NULL == cpnt )									//Not found: something wrong
			{
				printf("\n\nInvalid Parameter File!" );
				if( NULL != buffer ) free( buffer );
				return;
			}
			else
				*cpnt++ = '\0';									//End it here (cpnt points past \n)
			strcpy( dest, bufpnt );								//Copy the line into dest

			bufpnt = cpnt;										//Next time start from here
			while(( *bufpnt <= ' ') && (*bufpnt != '\0')) bufpnt++; //Look for a printable character or EOF

			if( '\0' != dest [ 0 ] )							//File not finished
			{
				cpnt = strchr( dest, '=');								//Look for =
				if( NULL == cpnt )				//Not found: something wrong
				{	printf("\n\nInvalid Parameter File no = in %s ", dest );
					if( NULL != buffer ) free( buffer );
					return;
				}
				else
				{
					found = NULL;
					*cpnt++ = '\0';									//Split line at equals sign
					for( i = 0; (( NULL == found ) && ( i < ( sizeof( NewParameterList) / sizeof( char *)))); i++)
					{
						found = strstr( NewParameterList[i], dest );	//If the text is found
						if( NULL != found )					//If the text is found
						{
							switch ( i )
							{
							case 0 : strcpy( Params->projectfilename, cpnt ); break;
							case 3 : Params->topsortcode = atoi( cpnt ); break;
							case 4 : Params->bottomsortcode = atoi( cpnt ); break;
							case 5 : Params->topstartrefdes = atoi( cpnt ); break;
							case 6 : Params->bottomstartrefdes = atoi( cpnt ); break;
							case 7 : Params->sortonmodules= atoi( cpnt ); break;
                            case 8 : Params->grid = fabs( atof( cpnt )); break;
                            case 9 : Params->log = atoi( cpnt ); break;                   //For logging -
							default : printf("\nParameter %s Invalid! ", dest ); break;
							}
						}

					}
				}
			}
		} while( '\0' != *bufpnt );	//Until the end of the file
		if( NULL != buffer ) free( buffer );
}




void	ShowMenu( struct RenumParams *Params )
{
	printf("\n\n*****************************************************************");
	printf("\n[P] KiCad PCB file name : %s", Params->projectfilename );
	printf("\n[2] Front sort: %s ", GetMenuString( G_DirectionsArray, Params->topsortcode ) );
	printf("\n[3] Front reference designators start at %d", Params->topstartrefdes );

	printf("\n[4] Back sort: %s", GetMenuString( G_DirectionsArray, Params->bottomsortcode ) );
	printf("\n[5] Back reference designators start " );

	if( Params->bottomstartrefdes == 0 )
		printf("where the front ends ");
	else
		printf("at %d ", Params->bottomstartrefdes );

	printf("\n[6] Sorting on: %s ", GetMenuString( G_ModulesRefDesArray, Params->sortonmodules ));
	printf("\n[7] There is a grid setting of: %.3f", Params->grid );

	printf("\n[8] Front references designators prepend string %s", Params->TopPrepend );
	printf("\n[9] Back references designators prepend string: %s", Params->BotPrepend );

	if(( strlen( Params->TopPrepend ) != 0 ) || ( strlen( Params->BotPrepend ) != 0 ))
	{
			printf("\n\n\t\t ******* Warning only use prepends once per side: they add up! *******");
            printf("\n\t\t ******* Remove prepend with a -, i.e. -F_ *******\n");
	}
} //ShowMenu()

//
//	If there is a file extension, remove it and add it to the filename
//	Return 0 if the file exists, else return 1
//

//
// Check the project filename
//
int	CheckProjectFileName( struct RenumParams *Params )
{
    if( 0 == strlen( Params->projectfilename )) return( -1 );   //Can't do much without the
    TrimExtension( Params->projectfilename, ".pro");            //Trim the extension if it is there
    if( 0 == CheckInputFileName( Params, ".pro" ))
    {
        printf("\n*** Kicad project file not found! *** " );
        return( 1 );
    } else return( 0 );
}

/*
 * Show what you are going to do and unless -Y parameter, allow changes/updates
char	G_TopPrependString[ MAXPREPEND ];
char	G_BottomPrependString[ MAXPREPEND ];".kicad_pcb"
 *
 */
void	ShowAndGetParameters( struct RenumParams *Params )
{
int	i;

    printf("\n\n");
	printf("%s\n", G_HELLO );
    printf("Compiled %s %s", __DATE__, __TIME__);

	LoadParameterFile( Params  );				//Try and load the parameter file
	CheckProjectFileName( Params );

	ShowMenu( Params );						//Say what is going on
	do
	{
		CheckProjectFileName( Params );
		printf("\n\nEnter number or letter in brackets 'R' to Run (ctl-C aborts): ");
		i = toupper( mygetch());			//Get the reply
		if( i == ABORT )
		{
			gettimeofday( &Params->StartTime, NULL );				//Get the start time
			FreeMemoryAndExit( 0 );							//Control C
		}
		printf("%c\n\n", i );

		switch ( i )
		{
			case 'P' :
			    StringGet( "\n\nFile name: ", Params->projectfilename, Params->projectfilename, MAXPATH );
				CheckProjectFileName( Params );
			break;

			case '2' :
				MenuSelect( "Front sort direction: ", G_DirectionsArray, &Params->topsortcode );
			break;

			case '3' :
				sprintf( G_ERRSTR, "%d", Params->topstartrefdes );
				StringGet( "Front reference designators start: ", G_ERRSTR, G_ERRSTR, 6 );
				Params->topstartrefdes = abs( atoi( G_ERRSTR ) );
				if( Params->topstartrefdes == 0 ) Params->topstartrefdes = 1;
			break;

			case '4' :
				MenuSelect( "Back sort direction: ", G_DirectionsArray, &Params->bottomsortcode);
			break;

			case '5' :
				sprintf( G_ERRSTR, "%d", Params->bottomsortcode );
				StringGet( "Back reference designators start (0 means where Front ends): ", G_ERRSTR, G_ERRSTR, 6 );
				Params->bottomsortcode = abs( atoi( G_ERRSTR ) );
			break;

			case '6' :
				MenuSelect("Sort on: ", G_ModulesRefDesArray, &Params->sortonmodules );
			break;

			case '7' :
				sprintf( G_ERRSTR, "%.3f", Params->grid );
				StringGet( "Grid Setting: ",  G_ERRSTR, G_ERRSTR, 10 );
				Params->grid = fabs( atof( G_ERRSTR ));

			break;

			case '8' :
				StringGet( "Front reference designator prepend string", Params->TopPrepend, Params->TopPrepend, MAXREFDES );
			break;

			case '9' :
				StringGet( "Back reference designator prepend string", Params->BotPrepend, Params->BotPrepend, MAXREFDES );
			break;

			case 'R' : break;

			default : printf("\nInvalid selection" ); break;
			}
			if( i != 'R') ShowMenu( Params );						//Say what is going on
		} while( i != 'R');
	gettimeofday( &Params->StartTime, NULL );				//Get the start time
}



/*
 *  the end
 */
