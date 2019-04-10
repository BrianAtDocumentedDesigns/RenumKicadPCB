/*
 * RenumStructures.h
 *
 *  Created on: Mar. 30, 2019
 *      Author: BrianP
 */

#ifndef RENUMSTRUCTURES_H_
#define RENUMSTRUCTURES_H_

#include    <string>

struct  KiCadModule {
        char            layer;                  //What layer the module is on (usually 1 or 2)
        double          coord[2];               //X ad Y coordinate
        std::string     RefDesType;             //What the RefDes Preamble is (i.e. U, R VR, etc.)
        std::string     RefDesString;           //What the RefDes Preamble is (i.e. U, R VR, etc.)
        unsigned int    RefDesNum;              //And what the RefDes is (i.e. 23)
//        int             index;                  //Used for sorting
};

struct  RefDesChange
{
        std::string  NewRefDes;             //The new reference designation (F_U21)
        std::string  OldRefDesString;       //What the old refdes preamble + number was
        bool         Ignore;                //Used to skip (if #, etc)
        bool         Found;                 //Found the ref des in the schematic
};

struct  KiCadFile
{
        std::string  filename;
        std::string  buffer;
        bool         scanned;
};

struct  PCBModule {
        char    layer;                      //What layer the module is on (usually 1 or 2)
        double  x,y;                        //X and Y coordinate
        std::string  RefDesString;          //What the RefDes Preamble is (i.e. U, R VR, etc.)
        std::string  RefDesType;            //Keep the type for renumbering.
        unsigned long RefDesNumber;         //Used for prefixes and checking starting bottom refdes
};

struct  RefDesTypeStruct {
        std::string  RefDesType;
        unsigned int RefDesCount;
};

struct      ModuleTextField {
        std::string  name;           //Name of the module text field
        std::string  layer;          //Layer it is on
};

#endif /* RENUMSTRUCTURES_H_ */
