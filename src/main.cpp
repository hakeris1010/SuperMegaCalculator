/*
SuperMegaCalculator by h3nt4iBoY a.k.a. Hakeris1010

v0.5.0
- (1) Added Makefile, with a Static Library as an option:
    - Targets: normal_exec, static_lib, exec_wlib
    - The last one is using a static library, headers from /include, and cpp from /test.
- (2) Made some adjustments to Debug outputter to better suit Library mode.
- Fully working Library mode!

BUGS:

TODOS:
- CMakeLists support
- [Far Future] Graphic UI.
*/

#include <string>
#include "calcstart.h"
#include "tools/fun.h"
#include "tools/debug.h"
#include "defines/globaldefines.h"
#include "defines/formal.h"

//#define VERSION "v0.5.0" //more accurate in "globaldefines.h"

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
