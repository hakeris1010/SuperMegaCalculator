#include "formal.h"
#include "globaldefines.h"
#include "tools/debug.h"
#include "tools/fun.h"

Debug deb; //debug declared here..

void startDebugger_printDebugInfo(const char *data, int mode) //0 -only start debugger, 1 -start and print.
{
    deb.setMode(DEBUGGER_DEBUGMODE); //set debugger
    deb.setDoublePrecision(FINAL_PRECISION);
    deb.closeFile(false);

    if(mode==1)
    {
        deb<<"\n\n==========================\n\n";
        deb<<"==========================\n\n";
        deb<<"["<<Fun::getTimeString()<<"]>>\nSkaiciafke "<<VERSION<<"\nCreated by H3nt4iB0y (C) 2015\n\n";
        deb<<"\n------------||------------\n\n";
        deb<<data<<"\n";
    }
}

//end.
