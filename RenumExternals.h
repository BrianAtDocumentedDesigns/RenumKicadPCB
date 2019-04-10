/*
 * RenumExternals.h
 *
 *  Created on: Apr. 3, 2019
 *      Author: BrianP
 */

#ifndef RENUMEXTERNALS_H_
#define RENUMEXTERNALS_H_

#include                "RenumStructures.h"
#include                "RenumKicadPCBClasses.h"

extern  bool            G_SortYFirst, G_DescendingFirst, G_DescendingSecond;
extern  bool            G_SortOnModules;      //Sort on modules/ref des
extern  bool            G_RemoveTopPrefix;
extern  bool            G_RemoveBottomPrefix;
extern  bool            G_WriteLogFile;
extern  bool            G_WriteChangeFile;
extern  bool            G_SetTextAngle;
extern  bool            G_OverlayReference;
extern  bool            G_CenterOnModule;


extern  int             G_FIRSTTEXTENTRY, G_SECONDTEXTENTRY;

extern  int             G_Errcount;           //The error count
extern  int             G_TopSortCode;        //The Top sort code (left to right, etc.)
extern  int             G_BottomSortCode;     //The Bottom sort code (left to right, etc.)
extern  int             G_TextFieldChoice;
extern  int             G_OverlayReferenceChoice;

extern  unsigned int    G_TopStartRefDes;     //The starting top ref des;;
extern  unsigned int    G_BottomStartRefDes;  //The starting bottom ref des
extern  int             G_Modules;
extern  int             G_TopDirectionsArray[];
extern  int             G_BottomDirectionsArray[];

extern  double          G_SortGrid;           //The sort grid
extern  double          G_TextAngle;

extern  struct          KiCadFile   G_netlist;
extern  struct          KiCadFile   G_PCBInputFile;

extern  std::string     G_TopLayerName;
extern  std::string     G_ProjectPath;
extern  std::string     G_ProjectFileName;
extern  std::string     G_OldProjectFileName;
extern  std::string     G_TopPrefix;        //The Top Prefix std::string
extern  std::string     G_BotPrefix;        //The Bottom Prefix std::string
extern  std::string     G_LogFile;
extern  std::string     G_TmpPCBBuffer;       //Scratch for text manipulation

extern  const std::string ParameterList[];

extern  std::vector     <RefDesChange> G_ChangeArray;
extern  std::vector     <KiCadFile> G_AllSchematics;
extern  std::vector     <PCBModule> G_TopModules;
extern  std::vector     <PCBModule> G_BottomModules;
extern  std::vector     <RefDesTypeStruct> G_RefDesTypes;
extern  std::vector     <ModuleTextField> G_AllTextFields;

extern    RenumKicadPCB *G_RenumMenu;
extern    AboutDialog   *G_AboutDialog;
extern    TextFunctions *G_TextDialog;
extern    ReadMeDialog* G_ReadMeDialog;


#endif /* RENUMEXTERNALS_H_ */
