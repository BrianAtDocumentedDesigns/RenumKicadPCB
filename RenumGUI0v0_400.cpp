//#define     _ECLIPSEDEBUG 1
//#define   _NOCHANGES   1
/*
 *      Author: Brian Piccioni DocumentedDesigns.com
 *      (c) Brian Piccioni
 *
 *      This is free software made available under the GNU General Public License(GPL) version 3 or greater.
 *      see https://www.gnu.org/licenses/gpl-3.0.en.html
 *
 *      It can be modified, copied, etc. provided:
 *      1) Original authorship of Brian Piccioni and DocumentDesigns.com is prominently noted
 *      2) Any derivative work or copy also be released under GPL V3 or later with the attribution mentioned in 1)
 *
 *      This software is release "as is" and with no warranty.
 *
 *      Please contact brian@documenteddesigns.com with any feature requests/bug notifications
 *      If you want me to fix a problem or add a feature I will expect you to provide sample files showing
 *      the problem or feature need.
 *
 *  Note:   set project properties g++ -static -static-libgcc -static-libstdc++ -g -static-libgcc -static-libstdc++
 *          use external console debug configuration
 *
 * Changes:
 *  V0.400      Full re-write c++, GUI with wxWidgets
 *
 *
 * See  https://github.com/wxWidgets/wxWidgets/releases/v3.1.1 for wxWidgets installation
 * Note: things like libraries and compiler flags are *** really *** important. See manual.txt for details.
 */

#include "RenumLiterals.h"
#include "RenumDefines.h"
#include "RenumPrototypes.h"
#include "RenumStructures.h"

#include <wx/wx.h>
#include <string>

#include "RenumKicadPCB_Base.h"

#include <wx/filedlg.h>
#include "RenumKicadPCBClasses.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    G_RenumMenu = new RenumKicadPCB( NULL, wxID_ANY, wxT("RenumKiCadPCB V0.401"), wxDefaultPosition, wxSize(920,1000));
    LoadParameterFile(  );
    G_RenumMenu->SetParameters();
    G_PCBInputFile.buffer.clear();              //Make sure nothing is there
    G_TmpPCBBuffer.clear();
    LoadPCBFile( G_PCBInputFile );
    G_RenumMenu->Show(true);
    return true;
}

//
// This writes the parameter file into the local directory
//
void    WriteParameterFile( void )
{
std::string parameterstring;

std::ofstream    WriteFile;
        WriteFile.open ( PARAMETERFILENAME, std::ios::binary | std::ios::out | std::ios::trunc );          //Open the file

        if( !WriteFile.is_open() ) {
            ShowMessage("Can't create parameter file!" );
            return;
        }

        WriteFile << ParameterList[0] + '=' + G_ProjectFileName + '\n';
        WriteFile << ParameterList[1] + '=' + G_TopPrefix + '\n';
        WriteFile << ParameterList[2] + '=' + std::to_string( G_RemoveTopPrefix ) + '\n';
        WriteFile << ParameterList[3] + '=' + G_BotPrefix + '\n';
        WriteFile << ParameterList[4] + '=' + std::to_string( G_RemoveBottomPrefix ) + '\n';
        WriteFile << ParameterList[5] + '=' + std::to_string(G_TopSortCode ) + '\n';
        WriteFile << ParameterList[6] + '=' + std::to_string(G_BottomSortCode ) + '\n';
        WriteFile << ParameterList[7] + '=' + std::to_string(G_TopStartRefDes ) + '\n';
        WriteFile << ParameterList[8] + '=' + std::to_string(G_BottomStartRefDes ) + '\n';
        WriteFile << ParameterList[9] + '=' + std::to_string(G_SortOnModules ) + '\n';
        WriteFile << ParameterList[10] + '=' + std::to_string(G_SortGrid ) + '\n';
        WriteFile << ParameterList[12] + '=' + std::to_string( G_WriteChangeFile ) + '\n';
        WriteFile << ParameterList[11] + '=' + std::to_string( G_WriteLogFile ) + '\n';
        WriteFile.close();
        return;
}   //WriteParameterFile()

//
// This loads the parameter file from the local directory and sets the values
//
void    LoadParameterFile(  )
{
std::string  paramline, buffer, parameter;
int     i, paramvalue;
size_t  newline = 0, delim;
std::string  paramfilename = PARAMETERFILENAME;

        G_ProjectFileName.clear();            //Set all defaults
        G_ProjectPath.clear();
        G_TopPrefix.clear();
        G_BotPrefix.clear();
        G_RemoveTopPrefix = false;
        G_RemoveBottomPrefix = false;
        G_TopSortCode = 0;
        G_BottomSortCode = 0;
        G_TopStartRefDes = 1;
        G_BottomStartRefDes = 0;
        G_SortOnModules = true;
        G_SortGrid = DEFAULT_GRID;
        G_WriteLogFile = false;
        G_WriteChangeFile = false;

        G_SetTextAngle = false;
        G_OverlayReference = false;
        G_CenterOnModule = false;

        G_TextFieldChoice = 0;
        G_OverlayReferenceChoice = 0;
        G_TextAngle = 0;

struct stat buf;
        if( 0 != stat( paramfilename.c_str(), &buf)) return;       //File not found
        LoadFileIntoString( buffer, paramfilename );

//    G_LOGCODE = LOGPLAN + LOGCHANGE + LOGCOORD;

//
// Now i have the parameter file in buffer
//
        do          // Process a line at a time
        {
            PullLineIntoString( paramline, newline, buffer );   //Copy a string to a work buffer
            if (paramline.empty()) break;                  //Nothing more to do - break out of do
            TrimString( paramline );

            delim = paramline.find( '=' );                  //Find the =
            if( NPOS == delim )
            {
                ShowMessage( "Invalid Parameter File no = in %s ", paramline );
                break;
            }

            parameter = paramline.substr( delim + 1 );
            paramvalue = atoi( parameter.c_str());

            for( i = 0; !ParameterList[i].empty(); i++)  //Until the end of the list
            {
                if( 0 == paramline.find( ParameterList[i] ))      //found
                {    switch ( i )
                    {
                        case 0 : G_ProjectFileName = parameter; break;
                        case 1 : G_TopPrefix = parameter;break;
                        case 2 : G_RemoveTopPrefix = paramvalue; break;
                        case 3 : G_BotPrefix = parameter; break;
                        case 4 : G_RemoveBottomPrefix = paramvalue; break;
                        case 5 : G_TopSortCode = paramvalue; break;
                        case 6 : G_BottomSortCode = paramvalue; break;
                        case 7 : G_TopStartRefDes = paramvalue; break;
                        case 8 : G_BottomStartRefDes = paramvalue; break;
                        case 9 : G_SortOnModules = paramvalue; break;
                        case 10 : G_SortGrid = fabs( atof( parameter.c_str())); break; //c++ stof blows up if not a valid float
                        case 11 : G_WriteChangeFile = paramvalue; break;
                        case 12 : G_WriteLogFile = paramvalue; break;

                        default :
                            ShowMessage( "Parameter %s invalid!\n ", parameter );
                            break;
                     }
                    break;  //Out of the for loop
                }
            }
        } while( true );

}

int     CheckProjectFileName( void )
{
size_t  lastslash = G_ProjectFileName.rfind( SLASHCHR );     //Path ends with the first slash from the right
std::string filename;
struct stat buf;

        if( NPOS == lastslash ) G_ProjectPath.clear();              //If no path
        else G_ProjectPath = G_ProjectFileName.substr( 0, lastslash + 1 ); //Get the path

        MakeFileName( filename, G_ProjectFileName, ".pro" );      //Make the full file name
        return( stat( filename.c_str(), &buf));
}

//
//Write the string to filename
//
void    WriteStringtoFile( std::string& filename, std::string& buffer )
{
    {
std::ofstream tmphandle( filename, std::ios::trunc );
        if( tmphandle.is_open()) tmphandle << buffer;
        if( !tmphandle.is_open() || tmphandle.bad() )
            FatalError( "Can't write %s\n", filename );
        ShowMessage( "%s updated \n", filename );
    }
}

//
// Save the buffer to the filename
//
void    SaveKiCadFile( struct KiCadFile &kicadfile )
{
        if( kicadfile.filename.empty() ) return;            //Nothing to do

std::string  newfilename = kicadfile.filename +"_RenumBack";
        remove( newfilename.c_str() );

        if( rename( kicadfile.filename.c_str(), newfilename.c_str()) != 0 )
            FatalError( "Can't create backup file for %s\n", kicadfile.filename );
        WriteStringtoFile( kicadfile.filename, kicadfile.buffer );
}  //Savekicadfile()


//
//  Write out the ChangeArray to KiCadChangeArray.txt
//
void    WriteChangeArray( char *ChangeArrayName )
{
std::string  outstring, outfilename;

for( size_t i = 0; i < G_ChangeArray.size(); i++ )
            outstring = outstring + G_ChangeArray[i].OldRefDesString + ',' + G_ChangeArray[i].NewRefDes + '\n';

        MakeFileName( outfilename, ChangeArrayName, ".txt" );  //Make the file name
        WriteStringtoFile( outfilename, outstring );
}

//
// This is where we usually leave: ensures the memory is freed up
//
void    WriteLogFile( void )
{
        if( !G_WriteLogFile ) return;

std::string      outfilename;
        MakeFileName( outfilename, "renum_logfile", ".txt" );  //Make the debug file name
        WriteStringtoFile( outfilename, G_LogFile );
}

void    NotFoundError( const char *errtype, std::string &refdes )
{
std::string  serrtype = errtype;
        ShowWarning( "***** New Reference for %s not found in %s *****\n", serrtype, refdes );
        if( G_Errcount++ > 10 )
            FatalError("\n\nToo many errors - exiting ..." );     //I am out of here
}

//
//  Make a file name with the path, filename, extension
//  This trims any path from the filename before copying it
//
void    MakeFileName( std::string& fullname, const std::string& filename, const std::string& extension )
{
size_t  lastslash = filename.rfind( SLASHCHR );
        lastslash = ( lastslash == NPOS ? 0 : lastslash + 1 );
size_t  dot = filename.rfind( '.' );                            //Find the "." for extension
        dot = ( dot == NPOS ? 0 : dot );

        fullname = G_ProjectPath + filename.substr( lastslash,
                                           (( 0 == dot ) || ( lastslash > dot ) ? NPOS : dot ) - lastslash ) + extension;
}

//
//  Load a file into a std::string. This avoids malloc issues.
//
int    LoadFileIntoString( std::string& outbuffer, std::string& filename )
{
std::stringstream   buffer;
            ShowMessage( "Loading %s \n", filename );

std::ifstream    readhandle(filename);

            if( !readhandle.is_open())
                FatalError("LoadFile: File not found!" );

            buffer << readhandle.rdbuf();       //Read the file into the buffer
            outbuffer = buffer.str();           //Copy buffer into string
            return( 0 );
}   //LoadFileIntoString()


//
// Find OldRefDes in the sorted change array using a binary search
//
int     FindNewRefDes( std::vector <RefDesChange> &ChangeArray, std::string& NewRefDes, const std::string& OldRefDes, bool& unannotated )    // Find it in the change array
{
size_t  start = 0, end = ChangeArray.size(), mid;

        if( 0 == end ) return( -1 );
        NewRefDes = OldRefDes;
        if(( '#' == OldRefDes[0] ) ||                 //If ignore (i.e. #)
                ( "\"\"" == OldRefDes ) ||            //Blank "" ref des ignore
                    ( '?' == OldRefDes.back()))
                        return( 0 );                //Unannotated

        if( !isdigit( OldRefDes.back()))        //If not a number last (i.e. Screw) do not change it
        {
            unannotated = false;
            return(0);
        }

        mid = ( start + end )/2;        //Start in the middle
        do
        {
            if( OldRefDes < ChangeArray[ mid ].OldRefDesString ) //In the lower half
                end = mid;
            else
                start = mid;
            mid = ( start + end )/2;
        } while(( end - start ) > 1 );                      //Until only 1 place left

        if( ChangeArray[ mid ].OldRefDesString == OldRefDes )   //Is this it?
        {
            unannotated = false;                            //Found it
            ChangeArray[ mid ].Found = true;                //Assume this is the new refdes
            if( ChangeArray[ mid ].Ignore ) return( 0 );    //Oops supposed to ignore
#ifdef _NOCHANGES
//Use for debug: replaces old with old so you can use file compare to verify file isn't trashed
#else
            NewRefDes = ChangeArray[ mid ].NewRefDes;       //This is the new refdes

#endif
            return( 0 );                                    //All good
        }
        else
            return( - 1 );
}


void    AddFileToSchematic( std::vector  <KiCadFile> &AllSchematics, const std::string &filename )
{
KiCadFile   NewEntry;
size_t      i;

        NewEntry.filename = filename;
        NewEntry.scanned = false;
        MakeFileName( NewEntry.filename, filename, ".sch");         //Get the name of the schematic

        for( i = 0; i < AllSchematics.size(); i++ )
            if( AllSchematics[i].filename == NewEntry.filename ) return;      //Already have the file in the array so do nothing
//
//  Not found! Push to the AllSchematics std::vector, read file
//
        AllSchematics.push_back( NewEntry );                        //Add the file name to the list
        i = AllSchematics.size() - 1;                               //Where it goes
        LoadFileIntoString( AllSchematics[i].buffer,             //Get the schematic file
                                    AllSchematics[i].filename );
}

//
// Build up the list of schematic file names and load into buffers as strings
//
//$Sheet
//F1 "NAND Flash , eMMC, T-Card and Audio.sch" 60
//$EndSheet
//

void    FlattenSchematic( void )
{
struct  KiCadFile   OneSheet;

std::string  fieldline;
size_t  firstquote, secondquote, wheresheet, whereendsheet, i;
bool    foundfilename;

        G_AllSchematics.clear();
        AddFileToSchematic( G_AllSchematics, G_ProjectFileName );            //Start the process
//
// Note: AllSchematics.size() increases as each new sheet is added
//
        for( i = 0; i < G_AllSchematics.size(); i++ )            //Go through the list of schematics
        {
            if( G_AllSchematics[i].scanned ) continue;                  //Already checked
            G_AllSchematics[i].scanned = true;                          //Now I did
            wheresheet = 0;
            whereendsheet = 0;

            do
            {   //Look for F1 at beginning of line (Filename)
                foundfilename = false;
                wheresheet = G_AllSchematics[i].buffer.find("$Sheet", whereendsheet );         //Find where it ends
                if( NPOS == wheresheet ) break;                                                 //No sheets left in this schematic
                whereendsheet = G_AllSchematics[i].buffer.find("$EndSheet", wheresheet );         //Find where it ends
                do
                {
                    PullLineIntoString( fieldline, wheresheet, G_AllSchematics[i].buffer );       //Copy just a line
                    if( 0 != fieldline.find("F1 ")) continue;                                      //If F1 not the beginning try next
                    firstquote = fieldline.find('\"' ) + 1;
                    secondquote = fieldline.find('\"', firstquote );
                    AddFileToSchematic( G_AllSchematics, fieldline.substr( firstquote, secondquote - firstquote ));  //Add this file to the list
                    foundfilename = true;
                } while( wheresheet < whereendsheet );

                if( !foundfilename )
                    ShowWarning( "Warning bad $Sheet in file %s\n)", G_AllSchematics[i].filename );
             } while( true );
        }
} //FlattenSchematic

//
// Pull a line into a std::string from a std::string, adjust pointer to next line
//
void    PullLineIntoString( std::string& dest, size_t& newline, const std::string& source )
{
        dest.clear();
        newline = source.find_first_not_of( "\n\r ", newline );  //Go to not CRLF or space
        if( NPOS == newline ) return;    //Not found
size_t  nextline = source.find_first_of( "\n\r", newline );            //Find newline
        nextline = source.find_first_not_of( "\n\r", nextline );       //Get end of line
        dest = source.substr( newline, NPOS == nextline ? NPOS : nextline - newline );
        newline = nextline;
}


//
//  Locate a field with text SearchFor and find the starting '"'
//  Append to dest up and including the quote
//  Copy the quoted test into fieldptr
//  Return pointer to the last '"'
//

void    CopyCompField( std::string& field, std::string& aline,
                       size_t& fieldstart, size_t& fieldend, const std::string& startdelim, const std::string& enddelim )
{
    fieldstart = aline.find( startdelim ) + startdelim.size();
    fieldend = aline.find( enddelim, fieldstart + 1 ) ;

    field = aline.substr( fieldstart, fieldend - fieldstart );
}
//
//  Trim leading and trailing spaces from a std::string
//

void    TrimString( std::string& input )
{
        while(( input[0] <= ' ') && (!input.empty())) input.erase(0);   //Deal with the leading non-printables
        while(( input[input.size() - 1] <= ' ' ) && (!input.empty())) input.resize(input.size() - 1);    //Trim trailing non printables
}


void    GetParenDelimitedField( std::string& outfield, const std::string& inbuf, const size_t& fieldstart )
{
    size_t  i = fieldstart, bracketcount = 0;
    do{
        if( '(' == inbuf[i] ) ++bracketcount;
        if( ')' == inbuf[i++] ) --bracketcount;
    }while ( 0 != bracketcount );

    outfield = inbuf.substr( fieldstart, i - fieldstart );
}

//
// aline points to a std::string buffer, field is the field to get the cordinate of, i.e. "(at "
// startcood is the start point for the search, endcoord
//
//  return false if no field present
//
bool    GetXY( const std::string &buffer, const char *field, size_t &startcoord, size_t &endcoord, double &xcoord, double &ycoord )
{
    startcoord = buffer.find( field, startcoord ) + strlen( field );  //Get Module X coordinate
    if( NPOS == startcoord ) return( false );       //No field

    endcoord = buffer.find( ' ', startcoord );      //Get X coordinate
    xcoord = atof( buffer.substr( startcoord, endcoord - startcoord ).c_str() );

    startcoord = buffer.find(' ', endcoord );       //Get Y coordinate
    endcoord = buffer.find_first_of( " )", startcoord + 1 );    //Space if an angle, otherwise )
    ycoord = atof( buffer.substr( startcoord + 1, endcoord - startcoord ).c_str() );

    return ( true );
}

bool    GetXY( const std::string &buffer, const char *field, size_t &startcoord, size_t &endcoord, std::string &xcoord, std::string &ycoord )
{
    startcoord = buffer.find( field, startcoord );
    if( NPOS == startcoord ) return( false );   //No field
    startcoord += strlen( field );              //Get X coordinate

    endcoord = buffer.find( ' ', startcoord );
    xcoord = buffer.substr( startcoord, endcoord - startcoord );

    startcoord = buffer.find(' ', endcoord );                   //Get Y coordinate
    endcoord = buffer.find_first_of( " )", startcoord + 1 );    //Space if an angle, otherwise )
    ycoord = buffer.substr( startcoord + 1, endcoord - startcoord - 1);

    return ( true );
}

bool    GetXYandAngle( const std::string &buffer, const char *field, size_t &startcoord, size_t &endcoord, double &xcoord, double &ycoord, double &angle )
{
    if( !GetXY( buffer, field, startcoord, endcoord, xcoord, ycoord ) ) return( false ); //field not found
    startcoord = endcoord;
    endcoord = buffer.find(')', startcoord );          //Get X,Y coordinate

    if( ')' == buffer[startcoord] )   //If no angle
        angle = 0.0;
    else
        angle = atof( buffer.substr( startcoord + 1, endcoord - startcoord ).c_str() );
    return( true );
}

bool    GetXYandAngle( const std::string &buffer, const char *field, size_t &startcoord, size_t &endcoord, std::string &xcoord, std::string &ycoord, std::string &angle )
{
    if( !GetXY( buffer, field, startcoord, endcoord, xcoord, ycoord ) ) return( false ); //field not found
    startcoord = endcoord;
    endcoord = buffer.find(')', startcoord );          //Get X,Y coordinate

    if( ')' == buffer[startcoord] )   //If no angle
        angle.clear();
    else
        angle = buffer.substr( startcoord + 1, endcoord - startcoord );
    return( true );
}

//
// Load the PCB file
//
int     LoadPCBFile( struct KiCadFile &PCBFile )
{
int     errcode;

        if( 0 != CheckProjectFileName( )) return( -1 );                                 //Invalid file name
        PCBFile.buffer.clear();

        MakeFileName( PCBFile.filename, G_ProjectFileName, ".kicad_pcb" );               //Make the full file name
        errcode = LoadFileIntoString( PCBFile.buffer, PCBFile.filename );                //Load this file and extension, get file size
        if( 0 != errcode ) return ( errcode );

size_t  modulecount = PCBFile.buffer.find("(modules ");                                  //Find out how many modules there are
        if( NPOS == modulecount )
            FatalError("No modules count in file!" );

        G_Modules = atoi(PCBFile.buffer.substr( modulecount + sizeof("(modules")).c_str());

size_t  starttxt = PCBFile.buffer.find( "(layers" );     //Get the top layer text name (always layer 0)
        if( NPOS == starttxt )
            FatalError("No layers field in PCB file!" );

        starttxt = PCBFile.buffer.find( "(0 ", starttxt + sizeof("(layers") );                //find next (
        starttxt += sizeof("(0");
size_t  endtxt = PCBFile.buffer.find(' ', starttxt );
        G_TopLayerName = PCBFile.buffer.substr( starttxt, endtxt - starttxt );             //Get the top layer name

        return( 0 );
}

void    GetField( std::string &dest, const char *fieldname, std::string &instr, size_t &pointer, char enddelim )
{
std::string  tmpfield;

        tmpfield.append( fieldname );
        tmpfield += ' ';
        pointer = instr.find( tmpfield, pointer );

        if( NPOS != pointer )           //Field found
        {
            pointer += tmpfield.size();  //Point to the text name
            dest = instr.substr( pointer, instr.find( enddelim, pointer ) - pointer ); //Get the text name
        }
}


void    UpdateProjectRefDes( bool messages )
{
        UpdatePCBRefDes( G_PCBInputFile, messages );               //Update the PCB ref des
        for( int i = 0; i < (int) G_AllSchematics.size(); i++)
            UpdateSchematicFileRefDes( G_AllSchematics[i], messages );
        UpdateNetlistRefDes( G_Netlist, messages  );                            //Now update the netlist names
}

//
//  Update all the nets and modules in the PCB
//
void    UpdatePCBRefDes( struct KiCadFile &PCBFile, bool messages )
{
bool    unannotated;        //Dummy
size_t  netptr = 0, inbufptr = 0, modptr, oldrefptr ;
std::string  TMPbuffer, NewRefDes, OldRefDes, Module;

        TMPbuffer.clear();
        if( messages ) ShowMessage( "Updating %s\n", PCBFile.filename );               //Put this here to separate from log files

        while( NPOS != netptr ) //Until no more
        {
            netptr = PCBFile.buffer.find("\"Net-(", inbufptr ); //look for a net
            if( NPOS == netptr ) break;                     //No more nets just write out the remainder
            netptr += sizeof( "\"Net-(") - 1;
            TMPbuffer += PCBFile.buffer.substr( inbufptr, netptr  - inbufptr );      //Write out to here

            OldRefDes = PCBFile.buffer.substr( netptr, PCBFile.buffer.find( '-', netptr ) - netptr );
            if( 0 != FindNewRefDes( G_ChangeArray, NewRefDes, OldRefDes, unannotated ))    // Find it in the change array
                NotFoundError( "net", NewRefDes );

            TMPbuffer += NewRefDes;     //Write out the new refdes (if not found is old ref des
            inbufptr = netptr + OldRefDes.size() + sizeof('-') - 1;             //Skip over the old refdes, rince and repeat
        }
        TMPbuffer += PCBFile.buffer.substr( inbufptr );         //Write out the rest of it
//
//  Now do modules. Do this differently (a module at a time ) to handle text changes later
//
        PCBFile.buffer.clear();                                  //Write out from scratch to PCB buffer
        inbufptr = 0;
        modptr = 0;

        while( NPOS != modptr ) //Until no more
        {
            modptr = TMPbuffer.find("(module ", inbufptr );     //look for a module
            if( NPOS == modptr ) break;                         //No more modules just write out the remainder
            PCBFile.buffer += TMPbuffer.substr( inbufptr, modptr  - inbufptr );      //Write out to "(module "
            GetParenDelimitedField( Module, TMPbuffer , modptr );          //Module now has the isolated module (do this for text changes)
            inbufptr = modptr + Module.size();                            //Pick up after here for next module

            oldrefptr = Module.find( "fp_text reference " ) + sizeof( "fp_text reference ") - 1;  //Point to the old ref des
            OldRefDes = Module.substr( oldrefptr, Module.find( ' ', oldrefptr ) - oldrefptr );

            if( 0 != FindNewRefDes( G_ChangeArray, NewRefDes, OldRefDes, unannotated ))
                NotFoundError( "module", NewRefDes );

            else
                Module.replace( oldrefptr, OldRefDes.size(), NewRefDes );         //Replace the old with the new

            PCBFile.buffer += Module;                        //Write out the new module
        } //While
        PCBFile.buffer += TMPbuffer.substr( inbufptr );         //Write out the rest of it
        return;
}

//
//  Scan through the Schematic file and substitute RefDes Old for RefDes New
//  The input file name has already been determined by the hierarchy scan so no need to add .sch
//  The new file is buffered in FileList.buffer
//

// Update this to treat $comp ... $endcomp as a

void    UpdateSchematicFileRefDes( struct KiCadFile &Schematic, bool messages )
{

bool    unannotated = true, err_report = true;
int     foundcmp;

size_t  nextcomp = 0, compend = 0;            //Use this to find where the next module starts
size_t  newline = 0, OldRefDesStart = 0, OldRefDesEnd = 0;

std::string  compfield;
std::string  aline, OldRefDes, NewRefDes, errfield;

std::string  outbuffer;

        outbuffer.clear();
        if( messages ) ShowMessage( "Updating %s\n", Schematic.filename ); //Put this here to separate from log files

        do    //Scan through the buffer (PCBFIle) and replace old refdes with new ones.
        {
            nextcomp = Schematic.buffer.find("$Comp", compend );                          //Find the next component
            if( NPOS == nextcomp ) break;
            outbuffer  += Schematic.buffer.substr( compend, nextcomp - compend );          //Write out to after $EndComp
            compend = Schematic.buffer.find("$EndComp", nextcomp ) + sizeof("$EndComp" );  //Look for the $EndComp
            compfield = Schematic.buffer.substr( nextcomp, compend - nextcomp );           //Get the $Comp ... $EndComp
            newline = 0;
            PullLineIntoString( aline, newline, compfield );                        //Do a line at a time

            while( !aline.empty())
            {
                OldRefDes.clear();
                if( 0 == aline.find("L ")) //Is this the first line? (ie. L LED D201 )
                {
                    OldRefDesStart = aline.rfind(' ') + 1;       //Go to second space
                    OldRefDesEnd = aline.find_first_of( "\r\n", OldRefDesStart );
                    OldRefDes = aline.substr( OldRefDesStart );       //Get the comp field
                    TrimString( OldRefDes );
                }

                else if ( 0 == aline.find("AR") )
                    CopyCompField( OldRefDes, aline, OldRefDesStart, OldRefDesEnd, "Ref=\"", "\"" ); //Field from Ref=" to "

                else if ( 0 == aline.find("F 0") )
                    CopyCompField( OldRefDes, aline, OldRefDesStart, OldRefDesEnd, "\"", "\"" );    //Field from first " to "

                if( OldRefDes.empty())
                    outbuffer += aline;                      //Just write out the line
                else
                {   //Find the component and copy it to NewRefDes
                    foundcmp = 0;               //Assume found
                    if( '#' == OldRefDes[0]) unannotated = false;                       //Do not annotate
                    if( '?' == OldRefDes.back()) foundcmp = 1;                          //Un annotated component or AR field mismatch

                    foundcmp = FindNewRefDes( G_ChangeArray, NewRefDes, OldRefDes, unannotated );

                    if( err_report && ( -1 == foundcmp ))                             //Not found in change array
                    {
                        err_report = false;     //Once per component
                        NotFoundError( "component", NewRefDes );
                    }
                    errfield = OldRefDes;
                    outbuffer += aline.substr( 0, OldRefDesStart ) + NewRefDes + aline.substr( OldRefDesEnd );
              }
                PullLineIntoString( aline, newline, compfield );
                if( NPOS == newline ) { outbuffer += aline; break;}
            }

            if( true == unannotated )
                ShowWarning( "**** Warning unannotated component %s! ****\n", errfield );

        }while( true );// while
        outbuffer += Schematic.buffer.substr( compend );                    //Write out to after $EndComp
        Schematic.buffer = outbuffer;                                //Update it
        return;
} //UpdateSchematicFileRefDes()


//
//  Scan through the Netlist file and substitute RefDes Old for RefDes New
//  Netlist update is simple Nets are all like this (ref C18)
//  So if the new ref des is C123 I replace C18 with C123
//

void    UpdateNetlistRefDes( struct KiCadFile &NetList, bool messages )
{
bool    unannotated;

size_t  nextnet, endparen;

std::string  outnetlist, oldrefdes, NewRefDes, tmp;
struct stat buf;

        MakeFileName( NetList.filename, G_ProjectFileName, ".net" );
        if( 0 != stat( NetList.filename.c_str(), &buf)) //File not found
        {
            NetList.filename.clear();       //Don't try again
            return;
        }

        if( messages ) ShowMessage( "Updating %s\n", NetList.filename );               //Put this here to separate from log files

        LoadFileIntoString( NetList.buffer, NetList.filename );
        nextnet = NetList.buffer.find("(ref ");             //Use this to find where the ref name
        endparen = 0;

        while( nextnet != NPOS )              //Scan through the buffer and replace old refdes with new ones.
        {
            nextnet += + sizeof("(ref");               //copy to after the ref
            outnetlist += NetList.buffer.substr( endparen, nextnet - endparen );   //Copy from ")" to after "(ref "
            endparen = NetList.buffer.find( ')', nextnet );

            if( NPOS == endparen )                       //Not found
                FatalError("Invalid net list format!" );

            oldrefdes = NetList.buffer.substr( nextnet, endparen - nextnet );

            if( -1 == FindNewRefDes( G_ChangeArray, NewRefDes, oldrefdes, unannotated )) // Find it in the change array
                ShowWarning( "\n****** Warning net %s not found in net list change list\n", oldrefdes );

            outnetlist += NewRefDes;
            nextnet = NetList.buffer.find("(ref ", endparen );             //Use this to find where the ref name
            }
        outnetlist += NetList.buffer.substr( endparen );        //Copy from last ")" to the end
        NetList.buffer = outnetlist;
        return;
}
//UpdateNetlistRefDes()


//
// Routines to produce formatted output for logging
//
std::string  FloatFormat( double anum, int decimal )
{
std::string  outs = std::to_string( anum ); //Gives 6 decimal places
        outs.resize( outs.find('.') + 1 + ( decimal > 6 ? 6 : decimal ));
        return( outs );
}

void    LogNewLineNumber( int linenum, int tab  )
        {
        std::string  text = "\n" + std::to_string( linenum );
                text.resize( tab, ' ');
                G_LogFile += text;
        }
        //
        //  Make nice with tabs
        //
void    LogLineItUp(  std::string text, int tab  )
        {
                text.resize( tab, ' ');
                G_LogFile += text;
        }

//
//  This is where all the magic happens
//
void    RenumKiCadPCB(void)
{
int i, num;
std::vector     <RefDesChange> PrefixChangeArray;

    PrefixChangeArray.clear();
    G_LogFile.clear();          //Since I can do multiple runs, clear all the globals.
    G_Errcount = 0;
    if( G_SortGrid < MINGRID ) G_SortGrid = MINGRID;  //Make sure sort grid is > 0

    FlattenSchematic(  );
    ShowMessage( "The schematic has %d unique sheet(s).\n", ((int) G_AllSchematics.size()));
//
// If adding or removing prefixes, need to process those first
//
    if( G_RemoveTopPrefix || G_RemoveBottomPrefix || !G_TopPrefix.empty() || !G_BotPrefix.empty() )
    {
        LoadModuleArray( G_PCBInputFile.buffer );           //Load the module array, split into top and bottom
        MakePrefixChangeArray( G_TopModules, G_RemoveTopPrefix, G_TopPrefix  );             //Make a fake change array
        MakePrefixChangeArray( G_BottomModules, G_RemoveBottomPrefix, G_BotPrefix  );
        if( G_WriteLogFile ) LogChangeArray( (char *) "\nPrefix modification plan\n" );
        UpdateProjectRefDes( NOMESSAGES );                   //Update the PCB file, schematics, and netlist (if present)
        PrefixChangeArray = G_ChangeArray;
    }
//
//  Now do the sorting
//
    LoadModuleArray( G_PCBInputFile.buffer );       //reload the module array, split into top and bottom

unsigned int    maxrefdes = 0;
std::string     maxrefdestype;

    MakeChangeArray( maxrefdestype, maxrefdes );
    if( !maxrefdestype.empty() ) //I found something
    {
            ShowWarning( "\n**** Error: Top Layer highest ref des  is %s. ****\n", maxrefdestype );
            ShowWarning( "**** Starting Back layer ref des number must be greater than %d ****\n", maxrefdes );
            maxrefdestype = "Change Bottom Ref Des Start to 0 or > " + std::to_string( maxrefdes ) + " and try again!";
Abort       TmpAbort( NULL, wxID_ANY, maxrefdestype.c_str(), wxDefaultPosition, wxSize(450,100));      //Show Continue/Abort menu
            TmpAbort.ShowModal();                //I should stop until clicked: Contine sets G_Errcount to 0
            return;
    }   //Make the change array
    if( G_WriteLogFile ) LogChangeArray( (char *) "\nRenumbering Plan\n" );
    UpdateProjectRefDes( MESSAGES );                          //Update the PCB file, schematics, and netlist (if present)
//
// All the file processing is done. If there are errors, ask to commit.
//      If no errors or OK to commit, create backup files for all the file names and write out the new files
    //
    if( 0 != G_Errcount )     //If there were errors
    {
        ShowError( "\n**** Warning: %d errors in files ****\n", G_Errcount );

ContinueAbort  TmpContinueAbort( NULL, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(400,300));      //Show Continue/Abort menu
               TmpContinueAbort.ShowModal();                //I should stop until clicked: Contine sets G_Errcount to 0
    }

    if( 0 == G_Errcount)      //If no errors or decides to commit
    {
        ShowMessage( "Making backups ... \n");
        ShowMessage( "Writing files ... \n");
        SaveKiCadFile( G_PCBInputFile );

        for( i = 0; i < (int) G_AllSchematics.size(); i++)
            SaveKiCadFile( G_AllSchematics[i] );

        SaveKiCadFile( G_Netlist );

        num = PrefixChangeArray.size();
        if( 0 != num )       //If I did prefixes
        {
bool        dummy;
std::string NewRefDes;

            if( num != (int) G_ChangeArray.size() ) FatalError( "Miss-match on prefix, renumber change array ");
            for( i = 0; i < num; i++)       //Merge the two arrays
            {
                FindNewRefDes( G_ChangeArray, NewRefDes, PrefixChangeArray[i].NewRefDes, dummy );    // Find it in the change array
                PrefixChangeArray[i].NewRefDes = NewRefDes;
            }
            G_ChangeArray = PrefixChangeArray;
        }
        WriteChangeArray( (char *) "RenumKiCadPCBChanges" );
    }

    WriteLogFile( );
    return;
}//RenumKiCadPCB

void   RoundToGrid( std::vector <PCBModule> &PCBModules )
{
size_t  i, modules = PCBModules.size();
double   old_x, old_y, rounder;

        if( G_WriteLogFile )
            G_LogFile += "\n*********** Sort on " +
                        (std::string)( G_SortOnModules ? "Ref Des" : "Module" )
                                +  " Coordinates *******************";
        for( i = 0; i < modules; i++ )
        {
            old_x = PCBModules[i].x;
            old_y = PCBModules[i].y;

            rounder = fmod( PCBModules[i].x , G_SortGrid);
            PCBModules[i].x -= rounder;                                     //X coordinate down to grid
            if( rounder > ( G_SortGrid/2 )) PCBModules[i].x += G_SortGrid;    //Round X coordinate up to grid

            rounder = fmod( PCBModules[i].y , G_SortGrid);
            PCBModules[i].y -= rounder;                                     //Y coordinate down to grid
            if( rounder > ( G_SortGrid/2 )) PCBModules[i].y += G_SortGrid;    //Round Y coordinate up to grid

            if( G_WriteLogFile )
            {
                std::string layer;
                layer = PCBModules[i].layer;

                LogLineItUp( "\n" + std::to_string( i ), 10  );
                LogLineItUp( PCBModules[i].RefDesString, 20  );       //Ref des
                LogLineItUp( layer, 4  );               //The side
                LogLineItUp( FloatFormat( old_x, 3 ), 12 );                //Old X coordinate
                LogLineItUp( FloatFormat( old_y, 3 ), 12 );                //Old Y coordinate
                LogLineItUp( FloatFormat( PCBModules[i].x, 3 ), 12 );       //Old X coordinate
                LogLineItUp( FloatFormat( PCBModules[i].y, 3 ), 12 );       //Old Y coordinate
            }
        }
} //RoundToGrid

//
// Load the modules, split into Top and Bottom and create the RefDesTypeArray
//
int     LoadModuleArray( std::string &PCBBuffer )                     //Load up the module array
{
double  ModXCoordinate, ModYCoordinate;             //What the X and Y Coordinate of the module is
double  modangle, sinmodangle, cosmodangle;

size_t  starttxt = 0, endtxt;
int     i = 0, k;
int     topmodulecount = 0, bottommodulecount = 0;
int     blankcount = 0;

PCBModule   module;
RefDesTypeStruct  refdestype;

        G_RefDesTypes.clear();
        G_ChangeArray.clear();
        G_TopModules.clear();
        G_BottomModules.clear();                //Start fresh
        do
        {
            starttxt = PCBBuffer.find( "(module ", starttxt );  //Use this to find where the next module starts
            if( NPOS == starttxt ) break;

            starttxt = PCBBuffer.find("(layer ", starttxt ) + sizeof( "(layer");
            endtxt = PCBBuffer.find( ')', starttxt );

            module.layer = 'F';
            if( G_TopLayerName != PCBBuffer.substr( starttxt, endtxt - starttxt )) //If not the top layer it is the
                module.layer = 'B';           //Bottom layer

            GetXYandAngle( PCBBuffer, "(at ", starttxt, endtxt, module.x, module.y, modangle );
            starttxt = PCBBuffer.find("(fp_text reference ",starttxt ) + sizeof( "(fp_text reference");
            endtxt = PCBBuffer.find(' ', starttxt );

            module.RefDesString = PCBBuffer.substr( starttxt, endtxt - starttxt );   //Get the ref des;

            if(( "\"\"" == module.RefDesString ) || ( 0 == module.RefDesString.size() ))   //Ignore zero length ref des
            {   ++blankcount;
                continue;
            }
            GetXYandAngle( PCBBuffer, "(at ", starttxt, endtxt, ModXCoordinate, ModYCoordinate, modangle );
            if( !G_SortOnModules )                                    //Only if sorting by reference designator coordinate
            {
                sinmodangle = sin( modangle * (3.14159265 / 180.0));             //Find the sine of the module angle
                cosmodangle = cos( modangle * (3.14159265 / 180.0));             //Find the sine of the module angle
                module.x += (( ModXCoordinate * cosmodangle )  + ( ModYCoordinate * sinmodangle ));  //Save the coordinate (done for clarity
                module.y += (( -ModXCoordinate * sinmodangle )  + ( ModYCoordinate * cosmodangle ));
            }

            for( k = (int) module.RefDesString.size() - 1; k >= 0; k-- )    //Go from end to start looking for non-digit
                if( !isdigit( module.RefDesString[k] )) break;              //Allows for B1_nn ref des

            if( k >= 0 )
                refdestype.RefDesType = module.RefDesString.substr( 0, k + 1 );             //Get just the name (i.e. U)

            module.RefDesNumber = strtoul( module.RefDesString.substr( k ).c_str(), NULL, 0);
            module.RefDesType = refdestype.RefDesType;
            refdestype.RefDesCount = 0;

            for( k = 0; k < (int) G_RefDesTypes.size(); k++ )                           //Search for this ref des type
                if( refdestype.RefDesType == G_RefDesTypes[ k ].RefDesType ) break;     //Already have this one

            refdestype.RefDesCount = 0;
            if( k == (int) G_RefDesTypes.size() )                       //A new type!
                G_RefDesTypes.push_back( refdestype );                  //Add this to the list

             if( 'F' == module.layer )
            {
                topmodulecount++;
                G_TopModules.push_back( module );
            }
            else
            {
                bottommodulecount++;
                G_BottomModules.push_back( module );
            }

        }while( ++i < G_Modules );

        RoundToGrid( G_TopModules );               //Round to the sort grid
        RoundToGrid( G_BottomModules );

        if(( i ) != G_Modules )
            FatalError( "Error: PCB modules does not match declaration\n" );

        ShowMessage("There are %d modules on top ", topmodulecount );
        ShowMessage("%d modules on bottom ", bottommodulecount );
        ShowMessage("and %d modules with no ref des\n", blankcount );
        return( 0 );
}

//
// Use std::sort() to sort modules. Because it is a structure a compare function is needed
// Returns true if the first coordinate should be before the second coordinate
//
static bool ModuleCompare(const PCBModule &A, const PCBModule  &B )
{
double   X0 = A.x, X1 = B.x, Y0 = A.y, Y1 = B.y;
double   tmp;

        if( G_SortYFirst )  //If sorting by Y then X, swap X and Y
        {   tmp = X0; X0 = Y0; Y0 = tmp;
            tmp = X1; X1 = Y1; Y1 = tmp;
        }

        if( G_DescendingFirst ) { tmp = X0; X0 = X1; X1 = tmp; } //If descending, same compare just swap directions
        if( G_DescendingSecond  ) { tmp = Y0; Y0 = Y1; Y1 = tmp; }

        if( X0 < X1 ) return( true );               //yes, its smaller
        if( X0 > X1 ) return( false );              //No its not
        if( Y0 < Y1 ) return( true );               //same but equal
        return( false );

}

//
//Create a change array from a module list
//
void    PCBSortSide( std::vector <PCBModule > &ModuleArray, int SortCode )
{
// First sort the module array
size_t  i, j, errcnt = 0;
size_t  numtypes = G_RefDesTypes.size();                      //Get the number of types
size_t  modules = ModuleArray.size();

struct  RefDesChange    Change;

        G_SortYFirst = (( SortCode & SORTYFIRST ) != 0 );     //If sorting by Y then X, swap X and Y
        G_DescendingFirst = (( SortCode & DESCENDINGFIRST ) != 0 );    //If descending, same compare just swap directions
        G_DescendingSecond = (( SortCode & DESCENDINGSECOND ) != 0 );    //If descending, same compare just swap directions
        sort( ModuleArray.begin(), ModuleArray.end(), ModuleCompare );
//
// Now make the change array from the sorted modules
//
        for( i = 0; i < modules; i++)   //For each module/change array
        {
            for( j = 0; j < numtypes; j++ ) //Look in the types array
            {
             if( ModuleArray[i].RefDesType == G_RefDesTypes[j].RefDesType )         //I found the type (U, R, VR, etc )
             {
                 Change.Ignore = !isdigit( ModuleArray[i].RefDesString.back());     //Ignore if no digit at end
                 Change.NewRefDes.clear();                                          //Start with nothing
                 Change.NewRefDes = G_RefDesTypes[j].RefDesType + std::to_string( G_RefDesTypes[j].RefDesCount++);   //Make the new refdes
                 Change.OldRefDesString = ModuleArray[i].RefDesString;              //Remember the original for look up
                 Change.Found = 0;                                                  //Hasn't been found
                 if( Change.Ignore ) Change.NewRefDes = Change.OldRefDesString;     //Don't modify if ignore
                 G_ChangeArray.push_back( Change );                                 //Add this module to the change Array
                 break;
            }
        }

        if(( (int) j >= (int) G_RefDesTypes.size()) && ( errcnt ++ < 10 ))   //Show max 10 error per sheet
        {
            ShowWarning( "\nError %d RefDes ", errcnt );
            ShowWarning( "%s not found! ", ModuleArray[i].RefDesType );

            if( G_WriteLogFile )        //Show errors in log file
            {
                G_LogFile += Change.OldRefDesString + "->" + Change.NewRefDes + '\n';
                G_LogFile += "\nError " + std::to_string( errcnt );
                G_LogFile += ModuleArray[i].RefDesType + " not found!";
            }
        }
        }
    G_Errcount += errcnt;
}

void    LogChangeArray( char * message )
{
int     i, num = ( int ) G_ChangeArray.size();

        G_LogFile += message;
        for( i = 0; i < num; i++ )
        {
            G_LogFile += G_ChangeArray[i].OldRefDesString + "->" + G_ChangeArray[i].NewRefDes + '\n';
        }
}


//
// Used for sorting the change array
//
static  bool    ChangeCompare( const RefDesChange& A, const RefDesChange& B )
{
        return( A.OldRefDesString < B.OldRefDesString );
}

//
// Make a sorted ChangeArray
//
void    MakeChangeArray( std::string &maxrefdestype, unsigned int &maxrefdes )
{
size_t  i, numtypes = G_RefDesTypes.size();
        if( G_WriteLogFile )
        {
            G_LogFile += "\n\nReference Designator Change Plan\n";
            G_LogFile += "\n\nThere are " + std::to_string( numtypes ) + " types of reference designations";

            for( int k = 0; k < (int) G_RefDesTypes.size(); k++ )
            {
                if( 0 == k%8 ) G_LogFile += "\n" ;              //On a new line
                    LogLineItUp( G_RefDesTypes[k].RefDesType, 10 );            //Pad spaces
            }
        }

        //First set the starting reference designation for this side
        for( i = 0; i < numtypes; i++ ) G_RefDesTypes[i].RefDesCount = G_TopStartRefDes;
        PCBSortSide( G_TopModules, G_TopDirectionsArray[G_TopSortCode] );        //Sort the top reference designations, then the bottom ones

        maxrefdes = 0;
//
// If I am not continuing from the top and I have a Bottom prefix and I'm not removing it
// check if the last top prefix is greater less than or equal to the Bottom start ref des
//
        if(( 0 != G_BottomStartRefDes ) && ( ( G_BotPrefix.empty( ) || ( !G_BotPrefix.empty() && G_RemoveBottomPrefix ))))
        {
            for( i = 0; i < numtypes; i++ )     //Find the highest ref des on the top layer
                if( G_RefDesTypes[i].RefDesCount > maxrefdes )
                {
                    maxrefdes =  G_RefDesTypes[i].RefDesCount;
                    maxrefdestype = G_RefDesTypes[i].RefDesType + std::to_string( maxrefdes );
                }

            if( 0 != G_BottomStartRefDes )    //If not continuing from the front reset the bottom ref des counters
            {
                if( maxrefdes >= (unsigned int) G_BottomStartRefDes )
                    return;
                for( i = 0; i < numtypes; i++ ) G_RefDesTypes[i].RefDesCount = G_BottomStartRefDes;
            }
        }
        PCBSortSide( G_BottomModules, G_BottomDirectionsArray[G_BottomSortCode] );
        sort( G_ChangeArray.begin(), G_ChangeArray.end(), ChangeCompare );  //Now sort the Modules by old ref des to speed up look ups
} //int     MakeChangeArray( void )


void    MakePrefixChangeArray( std::vector <PCBModule > &ModuleArray, bool RemovePrefix, std::string& Prefix  )
{
bool    haveprefix;
int     i, modules = ModuleArray.size(), prefixsize = Prefix.size();

struct  RefDesChange    Change;

        for( i = 0; i < modules; i++)   //For each module/change array
        {
            Change.Ignore = !isdigit( ModuleArray[i].RefDesString.back());      //Ignore if no digit at end
            Change.OldRefDesString = ModuleArray[i].RefDesString;               //Remember the original for look up
            Change.NewRefDes = Change.OldRefDesString;                          //Assume no change
            Change.Found = false;
            if(( !Change.Ignore ) || ( 0 != prefixsize ))                       //If a change in prefix (going to add it or remove it
            {
                haveprefix = ( 0 == ModuleArray[i].RefDesString.find( Prefix ));
                if( haveprefix && RemovePrefix )
                    Change.NewRefDes = ModuleArray[i].RefDesType.substr( Prefix.size()) + std::to_string( ModuleArray[i].RefDesNumber + BIGGEST_REFDES );
                else if( !haveprefix && !RemovePrefix )
                    Change.NewRefDes = Prefix + ModuleArray[i].RefDesType + std::to_string( ModuleArray[i].RefDesNumber + BIGGEST_REFDES );
            }
            G_ChangeArray.push_back( Change );                                 //Add this module to the change Array
        }
        sort( G_ChangeArray.begin(), G_ChangeArray.end(), ChangeCompare );  //Now sort the Modules by old ref des to speed up look ups
}

//
// Module Text Field functions
//
//
// Used to find the limits of a layer drawing
//
void    MinMax( double &tmpx, double &minx, double &maxx, double &tmpy, double &miny, double &maxy )
{
    if( tmpx > maxx ) maxx = tmpx;
    else if ( tmpx < minx ) minx = tmpx;

    if( tmpy > maxy ) maxy = tmpy;
    else if ( tmpy < miny ) miny = tmpy;
}

//
// Try and locate the center of a module based on layer (i.e. silk, fab, etc.) outline
// Outlines may or may not be complete so I do this by either
//      taking the center of a circle if there is one (this works for transistors, radial caps, and most things with a circle
// or
//      taking the average of the min and max extents of the outline
//      Leaves x, y unchanged if no information so have to set x, y before calling
//          Calculate center of module (layer name (Xmax+Xmin)/2, (Ymin+Ymax)/2 )
//
// Lines are not in order!
//
//    (fp_circle (center -1.8 2.673) (end -1.6 2.873) (layer F.SilkS) (width 0.254))
//    (fp_line (start -1.778 -3.302) (end 22.86 -3.302) (layer F.SilkS) (width 0.254))
//    (fp_line (start 22.86 -3.302) (end 22.86 10.922) (layer F.SilkS) (width 0.254))

//  x   22.9 + -1.8 = 20.1/2 = 10.5
//  y   11 + - 3.3 = 7.7 /2  = 3.375

//  (fp_text reference U2 (at 11.811 3.683 180) (layer F.SilkS)
//  (fp_text user %R (at 0 -1.9) (layer F.Fab)
//  (fp_text value SMCJ5.0A (at 0 4.2) (layer F.Fab)
//
//

bool    LocateModuleCenter( const std::string &Module, std::string &layer, std::string &x, std::string &y )
{
bool    nocoord = true;
size_t  newline = 0, startcoord, endcoord;
double  minx = 10000.0, miny = 10000.0, maxx = -10000.0, maxy = -10000.0;
double  tmpx, tmpy;

std::string  aline, tmpfield, coordxstring, coordystring;

        do
        {
            startcoord = 0;
            PullLineIntoString( aline, newline, Module );               //Do a line at a time

            if( NPOS == newline ) break;                                 //Done
            if( NPOS == aline.find( "(layer " + layer  )) continue;      //Wrong layer

            if( NPOS != aline.find( "fp_circle"))
                return( GetXY( aline, "(center ", startcoord, endcoord, x, y ));   //(kludge) usually works

            if( NPOS != aline.find( "fp_arc"))
                return( GetXY( aline, "(start ", startcoord, endcoord, x, y ));   //(kludge) usually works

            if( NPOS != aline.find( "fp_line"))              //If there is a circle, assume it is the center
            {
                GetXY( aline, "(start ", startcoord, endcoord, tmpx, tmpy );        //Get the start x and y
                MinMax( tmpx, minx, maxx, tmpy, miny, maxy );
                GetXY( aline, "(end ", startcoord, endcoord, tmpx, tmpy );          //Get the end x and y
                MinMax( tmpx, minx, maxx, tmpy, miny, maxy );
                nocoord = false;                                                    //found at least one set of coordinates
            }
        }while( true );

        if( nocoord ) return( false );       //Return with nothing leave x, y alone
        x = std::to_string(( maxx + minx )/2);
        y = std::to_string(( maxy + miny )/2);
        return( true );
}

//
// Update the coordinates/angle in reference, value, and Module text
//
//    (fp_text value C (at 1.25 0) (layer B.Fab)
//        (effects (font (size 1 1) (thickness 0.15)) (justify mirror))
//
int FindTextNameAndLayer( std::string &Module, std::string &atfield, size_t &starttxtfield, const size_t whichtext  )
{
std::string debug;
size_t  startoffield;

    starttxtfield = Module.find( "(fp_text " + G_AllTextFields[ whichtext ].name, starttxtfield );   //Find the subject field
    startoffield = starttxtfield;               //Remember this
    if( NPOS == starttxtfield ) return( -1 );
    GetParenDelimitedField( atfield, Module, starttxtfield ); //Get from (fp_text to last );

    debug = "(layer " + G_AllTextFields[whichtext].layer + ')' ;
    printf("%s\n", debug.c_str() );
    printf("%s\n", atfield.c_str() );

    if( NPOS == atfield.find( "(layer " + G_AllTextFields[whichtext].layer + ')' )) return( 1 ); //Not this layer
    starttxtfield = startoffield;
    return( 0 );
}

//
//  Use std::sort() to sort test fields. Sort of layer and name so
//  Because it is a structure a compare function is needed
//  Returns true if the first should be before the second.
//
static bool TextFieldCompare(const ModuleTextField &A, const ModuleTextField  &B )
{
    if( A.layer > B.layer ) return( true );     //F.SilkS comes after B.Silks but it makes more sense to put it first.
    if (A.layer < B.layer) return(false);
    return(A.name < B.name);
}


//
//  Scan through all the modules in the PCB file and make a database of names and what layer they are on
//  Returns sorted with top layer fields first then sorted alphabetically
//
void    ScanPCBTextFields( void )
{
size_t  textptr = 0;
size_t  i, numfields = 0;

struct  ModuleTextField ThisTextField;

        do
        {
            numfields = G_AllTextFields.size();
            GetField( ThisTextField.name, "fp_text", G_TmpPCBBuffer, textptr, ' ' );
            if( NPOS == textptr )
                break;                                //No more text fields in this module

            GetField( ThisTextField.layer, "(layer", G_TmpPCBBuffer, textptr, ')' );
            for( i = 0; i < numfields; i++ )
            if(( ThisTextField.name == G_AllTextFields[i].name ) &&
                    ( ThisTextField.layer == G_AllTextFields[i].layer ))
                        break;       //if found

            if(( 0 == numfields ) || ( i >= numfields ))
            {
                G_AllTextFields.push_back( ThisTextField );
            }
        }while( true );     //Do until the end
        sort( G_AllTextFields.begin(), G_AllTextFields.end(), TextFieldCompare );
}

//
//  Update all the nets and modules in the PCB if UpdateRefDes is true
//  Update the text field angles and locations if UpdateRefDes is false
//

void    UpdatePCBText( void )
{
size_t  starttxtfield = 0, coordstart = 0, coordend;
size_t  modptr = 0, inbufptr = 0;
std::string  Module, TMPbuffer;
std::string  atfield, x, y, angle;
std::string  refatfield, refx, refy, refangle;

        modptr = 0;
        TMPbuffer.clear();

        while( NPOS != modptr ) //Until no more
        {
            modptr = G_TmpPCBBuffer.find("(module ", inbufptr );  //look for a module
            if( NPOS == modptr ) break;

            TMPbuffer += G_TmpPCBBuffer.substr( inbufptr, modptr  - inbufptr );      //Write out to "(module "
            GetParenDelimitedField( Module, G_TmpPCBBuffer, modptr );          //Module now has the isolated module (do this for text changes)
            inbufptr = modptr + Module.size();                                      //Pick up after here for next module

            starttxtfield = 0;
            while( true )   //Until no more text fields
            {
                if( 0 != FindTextNameAndLayer( Module, atfield, starttxtfield, G_TextFieldChoice ))
                    break;    //Not found in this module

                starttxtfield = Module.find("(at ", starttxtfield );            //Match to field name and layer do get coordindates
                GetParenDelimitedField( atfield, Module, starttxtfield );
//
// Get coordinates x, y, angle
// format is (at 96.373 85.705 270) or (at 96.373 85.705) no angle means angle is 0
//
                coordstart = 0;
                GetXYandAngle( atfield, "(at ", coordstart, coordend, x, y, angle );        //Get the start x and y

                if( G_CenterOnModule )
                    LocateModuleCenter( Module, G_AllTextFields[G_TextFieldChoice].layer, x, y );
                else if( G_SetTextAngle )
                    angle = FloatFormat( G_TextAngle, 2 );
                else if ( G_OverlayReference )
                {
                    coordstart = starttxtfield;
                    if( 0 != FindTextNameAndLayer( Module, refatfield, starttxtfield, G_OverlayReferenceChoice )) return; //Reference not found in this module
                    GetParenDelimitedField( refatfield, Module, starttxtfield );
                    starttxtfield = coordstart;
                    coordstart = 0;
                    GetXYandAngle( refatfield, "(at ", coordstart, coordend, refx, refy, refangle );        //Get the start x and y
                    x = refx;
                    y = refy;
                    angle = refangle;
                }

                if( 0 == atof( angle.c_str()) ) angle.clear();
                if( !angle.empty() ) angle.insert( 0, " " );
                Module.replace( starttxtfield, atfield.size(), "(at " + x + ' ' + y + angle + ')' );
            }   //While
            TMPbuffer += Module;                        //Write out the new module
        } //While                       //For the whole file

        TMPbuffer += G_TmpPCBBuffer.substr( inbufptr );         //Write out the rest of it
        G_TmpPCBBuffer = TMPbuffer;
}

//
// Routines to produce formatted output for logging
//
std::string  FloatFormat( float anum, int decimal )
{
std::string  outs = std::to_string( anum ); //Gives 6 decimal places
        outs.resize( outs.find('.') + 1 + ( decimal > 6 ? 6 : decimal ));
        return( outs );
}



/*
 *  the end
 */
