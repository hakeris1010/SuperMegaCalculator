#ifndef GLOBALDEFINES_H_INCLUDED
#define GLOBALDEFINES_H_INCLUDED

//GlobalDefines - all the data for debugging and stuff.

#include "tools/debug.h"

//VERSION
#define VERSION "v0.5.2" //Cool!

//precision
#define FINAL_PRECISION 15

//debug defines
#define DEBUGGER_DEBUGMODE   DEBUG_TO_SCREEN

#define DEBUG_CALCULATOR_SENDSTRING                 false
#define DEBUG_CALCULATOR_STARTCALCULATION           true
#define DEBUG_CALCULATOR_RECURSIVECHUNKYCALCULATION false
#define DEBUG_CALCULATOR_CALCULATEDAUGS             false
#define DEBUG_CALCULATOR_CALCULATESPECIALS          false
#define DEBUG_CALCULATOR_SKAICSPECIAL               true
#define DEBUG_CALCULATOR_IDENTIFYNEGATIVES          false

#define DEBUG_TRANSFORMER_SETCALCULATIONELEMENTS    false
#define DEBUG_TRANSFORMER_POSTPROCESSCALCELEMENTS   true
#define DEBUG_TRANSFORMER_SHOWELEMENTS              true
#define DEBUG_TRANSFORMER_SHOWCALCELEMENTCONTENTS   true

//debug define end

#endif // GLOBALDEFINES_H_INCLUDED
