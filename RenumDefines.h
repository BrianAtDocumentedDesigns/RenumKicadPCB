/*
 * RenumDefines.h
 */

#ifndef RENUMDEFINES_H_
#define RENUMDEFINES_H_

#include        "RenumLiterals.h"
#include        "RenumKicadPCBClasses.h"
#include        "RenumPrototypes.h"
#include        "RenumStructures.h"
#include		<sys/stat.h>		

bool            G_SortYFirst, G_DescendingFirst, G_DescendingSecond;
bool            G_SortOnModules;      //Sort on modules/ref des
bool            G_RemoveTopPrefix;
bool            G_RemoveBottomPrefix;
bool            G_WriteLogFile;
bool            G_WriteChangeFile;
bool            G_SetTextAngle;
bool            G_OverlayReference;
bool            G_CenterOnModule;

int             G_Errcount;           //The error count
int             G_TopSortCode;        //The Top sort code (left to right, etc.)
int             G_BottomSortCode;     //The Bottom sort code (left to right, etc.)
int             G_TextFieldChoice;
int             G_OverlayReferenceChoice;
unsigned int    G_TopStartRefDes;     //The starting top ref des;;
unsigned int    G_BottomStartRefDes;  //The starting bottom ref des
int             G_Modules;

double          G_SortGrid;           //The sort grid
double          G_TextAngle;

struct          KiCadFile   G_Netlist;
struct          KiCadFile   G_PCBInputFile;

std::string     G_TopLayerName;
std::string     G_ProjectPath;
std::string     G_ProjectFileName;
std::string     G_OldProjectFileName;
std::string     G_TopPrefix;             //The Top Prefix std::string
std::string     G_BotPrefix;             //The Bottom Prefix std::string
std::string     G_LogFile;
std::string     G_TmpPCBBuffer;          //Scratch for text manipulation

std::vector     <RefDesChange> G_ChangeArray;
std::vector     <KiCadFile> G_AllSchematics;
std::vector     <PCBModule> G_TopModules;
std::vector     <PCBModule> G_BottomModules;
std::vector     <RefDesTypeStruct> G_RefDesTypes;
std::vector     <ModuleTextField> G_AllTextFields;

RenumKicadPCB   *G_RenumMenu;
AboutDialog     *G_AboutDialog;
TextFunctions   *G_TextDialog;
ReadMeDialog    *G_ReadMeDialog;

//
// This converts the index into a sort code. Note that Bottom sort code will have left and right swapped.
//
int     G_TopDirectionsArray[] =
{
        SORTYFIRST + ASCENDINGFIRST + ASCENDINGSECOND,   //"Top to bottom, left to right",
        SORTYFIRST + ASCENDINGFIRST + DESCENDINGSECOND,  //"Top to bottom, right to left",
        SORTYFIRST + DESCENDINGFIRST + ASCENDINGSECOND,  //"Bottom to top, left to right",
        SORTYFIRST + DESCENDINGFIRST + DESCENDINGSECOND, //"Bottom to top, right to left",
        SORTXFIRST + ASCENDINGFIRST + ASCENDINGSECOND,   //"Left to right, top to bottom",
        SORTXFIRST + ASCENDINGFIRST + DESCENDINGSECOND,  //"Left to right, bottom to top",
        SORTXFIRST + DESCENDINGFIRST + ASCENDINGSECOND,  //"Right to left, top to bottom",
        SORTXFIRST + DESCENDINGFIRST + DESCENDINGSECOND  //"Right to left, bottom to top",
};
//
// Back Left/Right is opposite because it is a mirror image (coordinates are from the top)
//
int     G_BottomDirectionsArray[] =
{
        SORTYFIRST + ASCENDINGFIRST + DESCENDINGSECOND,  //"Top to bottom, left to right",
        SORTYFIRST + ASCENDINGFIRST + ASCENDINGSECOND,   //"Top to bottom, right to left",
        SORTYFIRST + DESCENDINGFIRST + DESCENDINGSECOND, //"Bottom to top, left to right",
        SORTYFIRST + DESCENDINGFIRST + ASCENDINGSECOND,  //"Bottom to top, right to left",
        SORTXFIRST + DESCENDINGFIRST + ASCENDINGSECOND,  //"Left to right, top to bottom",
        SORTXFIRST + DESCENDINGFIRST + DESCENDINGSECOND, //"Left to right, bottom to top",
        SORTXFIRST + ASCENDINGFIRST + ASCENDINGSECOND,   //"Right to left, top to bottom",
        SORTXFIRST + ASCENDINGFIRST + DESCENDINGSECOND   //"Right to left, bottom to top",
};

const std::string ParameterList[] =
{
    "PCB file name",                            //0
    "Top Prefix",                               //1
    "Remove Top Prefix",                        //2
    "Bottom Prefix",                            //3
    "Remove Bottom Prefix",                     //4
    "Top Sort Direction",                       //5
    "Bottom Sort Direction",                    //6
    "Top Start Reference Designation",          //7
    "Bottom Start Reference Designation",       //8
    "Sort on Modules",                          //9
    "Sort Grid",                                //10
    "Write Change File",                        //12
    "Write Log File",                           //11
    ""
};


#endif /* RENUMDEFINES_H_ */
