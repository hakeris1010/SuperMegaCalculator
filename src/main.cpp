/*
SuperMegaCalculator by H3nt4iBoY a.k.a. Hakeris1010

v0.5.2
- Added Error Checking system!
  - Errors are identified in several functions by levels:
    1. Transformer::setCalculationElements(...) - when a total nonsense is inputted (non-existing operators)
    2. Transformer::postProcessCalcElements(...) - called after 1, fixes bad occurences like (..)(..) or 2x
       by inserting '*', and finds errors like bad operator arrangement, e.g. ln+9( .
    3. Calculator::recursiveChunkyCalculation(...) - finds bad parenthesis.
    4. Calculator::skaicSpecial(...) - finds bad multiparam counts (like log_2_3_4)

- Variables implemented (x, y, x1, ...), to form equations.
- Removed a bug with Calculator::identifyNegatives() and not calculated parenthesis.

BUGS:
- SQRT have wrongly interpreted multiparams

TODOS:
v0.5.3
- Optimize every function to use new Debug::setWriteDebugs() method, and not use 'if(dbg)'.
- Write the Calculator::calculateSpecials_v2() function, because the one used now is
  gigantic and inefficient.
- Identify negatives at Transformer::postProcessCalcElements() - for more efficiency
- (Optional) A new class called ErrorHandler, and inside would be postProcessCalcElements(),
  and some code from the other functions that deals with error handling.

v0.6
- A class called EquationalCalculator will be extended and used in Calculator.
- Operations with equations (solve, differentiate, integrate, etc), performed in that class.
- Possible methods:
  - simplify(std::vector<CalcElement>&) - simplify a polynomial with variables.
  - and all of the calculative ones.

Future:
- CMakeLists support
- [Far Future] Graphic UI.
*/

#include <string>
#include "calcstart.h"
#include "tools/fun.h"
#include "tools/debug.h"
#include "defines/globaldefines.h"
#include "defines/formal.h"

//#define VERSION "v0.5.2" //more accurate in "globaldefines.h"

using namespace std;


int main()
{
    //deb.setLastModifTag("main");

    //startDebugger_printDebugInfo(); //set debugger

    int val=startCalculazione();
    if(val!=0) deb<<"\nError: "<<val<<"\n";

    return 0;
}

//end;
