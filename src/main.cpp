/*
BUGS:
- None by now!!!!!!!!!!!

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

#define VERSION "v0.3.1"

Debug deb;

using namespace std;


int main()
{
    deb.setMode(DEBUG_TO_FILE);
    deb.setDoublePrecision(15);
    deb.closeFile(false);

    deb<<"\n\n==========================\n\n";
    deb<<"==========================\n\n";
    deb<<"["<<Fun::getTimeString()<<"]>>\nSkaiciafke "<<VERSION<<" by H3nt4iB0y (C) 2015\n\n";
    deb<<"\n------------||------------\n\n";

    int val=startCalculazione();
    if(val!=0) deb<<"\nError: "<<val<<"\n";

    return 0;
}

//end;
