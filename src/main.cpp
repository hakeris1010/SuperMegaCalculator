/*
SuperMegaCalculator by h3nt4iBoY a.k.a. Hakeris1010

v0.4
- Added ^ operation
- added specialEnd for better calculation

BUGS:

TODOS:
- UI, afkors!
- Some Jokes.
*/

#include "calcstart.h"
#include "tools/fun.h"
#include <string>
#include <iostream>
#include <tools/debug.h>
#include <tools/fun.h>

#define VERSION "v0.4"

Debug deb;

using namespace std;


int main()
{
    deb.setMode(DEBUG_TO_SCREEN);
    deb.setDoublePrecision(15);
    deb.closeFile(false);

    deb<<"\n\n==========================\n\n";
    deb<<"==========================\n\n";
    deb<<"["<<Fun::getTimeString()<<"]>>\nSkaiciafke "<<VERSION<<"\nCreated by H3nt4iB0y (C) 2015\n\n";
    deb<<"\n------------||------------\n\n";

    int val=startCalculazione();
    if(val!=0) deb<<"\nError: "<<val<<"\n";

    return 0;
}

//end;
