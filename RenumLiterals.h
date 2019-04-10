/*
 * RenumLiterals.h
 *
 *  Created on: Mar. 30, 2019
 *      Author: BrianP
 */

#ifndef RENUMLITERALS_H_
#define RENUMLITERALS_H_

#define NPOS std::string::npos

#ifdef  __linux__
    #define     SLASHCHR    '/'          //Linux/Unix/Apple file path slash
#else
    #define     SLASHCHR  '\\'          //Windows file path slash
#endif

#define SORTXFIRST  0b000               //Sort on X
#define SORTYFIRST  0b100               //Sort on Y

#define ASCENDINGFIRST      0b00        //Sort low to high
#define DESCENDINGFIRST     0b10        //Sort high to low
#define ASCENDINGSECOND     0b00        //Sort low to high
#define DESCENDINGSECOND    0b01        //Sort high to low

#define MINGRID             0.001
#define DEFAULT_GRID        1.000
#define BIGGEST_REFDES      4000000000      //The biggest possible ref des

#define NOMESSAGES      false
#define MESSAGES        true

#define CENTERTEXT      0       //Center the module text field
#define SETTEXTANGLE    1       //Set the module text angle
#define ALIGNPCBTEXT    2       //Align this first module text with that

#define PARAMETERFILENAME   "RenumParameters.txt"

#endif /* RENUMLITERALS_H_ */
