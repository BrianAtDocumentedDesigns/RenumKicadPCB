/*
 * RenumProtoTypes.h
 *
 *  Created on: Mar. 30, 2019
 *      Author: BrianP
 */

#ifndef RENUMPROTOTYPES_H_
#define RENUMPROTOTYPES_H_

#include    <iostream>
#include    <fstream>
#include    <sstream>
#include    <bitset>
#include    <string>

#include    <math.h>

#ifndef _MSC_VER
#include    <unistd.h>
#else
#include	<io.h>
#endif

#include    <stdbool.h>

#include    <vector>        //Vector
#include    <algorithm>     //Sort

#include    "RenumStructures.h"
//
// Function prototypes
//

void        MakeFileName( std::string& dest, const std::string& filename, const std::string& extension );
int         LoadFileIntoString( std::string& buffer, std::string& filename );
void        SaveKiCadFile( struct KiCadFile &kicadfile );

void        PullLineIntoString( std::string& dest, size_t& newline, const std::string& source );
void        FreeMemoryAndExit( int code );
void        RenumError( std::string message, int code );

void        FlattenSchematic( void );
void        UpdateProjectRefDes( bool messages );
void        UpdatePCBRefDes( struct KiCadFile &PCBFile, bool messages );
void        UpdateSchematicFileRefDes( struct KiCadFile &Schematic, bool messages  );
void        UpdateNetlistRefDes( struct KiCadFile &Schematic, bool messages );

int         FindNewRefDes( std::vector <RefDesChange> &ChangeArray, std::string& NewRefDes, const std::string& OldRefDes, bool& unannotated );    // Find it in the change array

int         CheckProjectFileName( void );
int         LoadPCBFile( struct KiCadFile &Schematic );
void        ScanPCBTextFields( std::string PCBFile );

void        GetTextCoord( std::string& inbuf, std::string& x, std::string& y, std::string& angle);
void        UpdateTextFieldInModule( std::string& module, const std::string& textfield, std::string NewTextAngle,
                                     bool AlignPCBText, const std::string& refx, const std::string& refy, const std::string& refangle );
void        GetParenDelimitedField( std::string& outfield, const std::string& inbuf, const size_t& fieldstart );

bool        GetXY( const std::string &buffer, const char *field, size_t &startcoord, size_t &endcoord, double &xcoord, double &ycoord );
bool        GetXY( const std::string &buffer, const char *field, size_t &startcoord, size_t &endcoord, std::string &xcoord, std::string &ycoord );
bool        GetXYandAngle( const std::string &buffer, const char *field, size_t &startcoord, size_t &endcoord, double &xcoord, double &ycoord, double &angle );
bool        GetXYandAngle( const std::string &buffer, const char *field, size_t &startcoord, size_t &endcoord, std::string &xcoord, std::string &ycoord, std::string &angle );

void        RenumKiCadPCB(void);
void        WriteLogFile( void );

void        ScanPCBTextFields( void );
void        UpdatePCBText( void );


std::string FloatFormat( double anum, int decimal );
void        LogLineItUp(  std::string text, int tab  );
void        LogNewLineNumber( int linenum, int tab  );
void        WriteChangeArray( char *ChangeArrayName );

void        LoadParameterFile( void);
void        WriteParameterFile( void);
void        TrimString( std::string& input );

void        RoundToGrid( std::vector <PCBModule> &PCBModules );
int         LoadModuleArray( std::string &PCBBuffer );
void        PCBSortSide( std::vector <PCBModule > &ModuleArray, int SortCode );
void        MakePrefixChangeArray( std::vector <PCBModule > &ModuleArray, bool RemovePrefix, std::string& Prefix  );
void        MakeChangeArray( std::string &maxrefdestype, unsigned int &maxrefdes );
void        LogChangeArray( char * message );


//
// Use these to make messages a bit easier
//

void    ShowError( const char *message, int arg1);

void    FatalError( const char *message );
void    FatalError( const char *message, std::string& arg1 );

void    ShowMessage( const char *message );
void    ShowMessage( const char *message, int arg1 );
void    ShowMessage( const char *message, std::string &arg1 );
void    ShowMessage( const char *message, std::string &arg1, std::string &arg2 );

void    ShowWarning( const char *message, int arg1 );
void    ShowWarning( const char *message, std::string& arg1  );
void    ShowWarning( const char *message, std::string& arg1, std::string& arg2 );

#endif /* RENUMPROTOTYPES_H_ */
