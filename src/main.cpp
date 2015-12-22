#include "calcstart.h"
#include "tools/fun.h"
#include <string>
#include <iostream>
#include <tools/debug.h>
#include <tools/fun.h>

#define VERSION "v0.2"

Debug deb;

using namespace std;


int main()
{
    deb.setMode(DEBUG_NO_DEBUG);
    deb.setDoublePrecision(15);
    deb.closeFile(false);

    deb<<"\n["<<Fun::getTimeString()<<"]>>\nSkaiciafke "<<VERSION<<" by H3nt4iB0y (C) 2015\n\n";

    int val=startCalculazione();
    if(val!=0) deb<<"\nError: "<<val<<"\n";

    return 0;
}

//end;
