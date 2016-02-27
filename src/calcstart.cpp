#include "calcstart.h"
#include "compute/calc.h"
#include <iostream>

#include "compute/transform.h"
#include "compute/structs.h"
#include "tools/debug.h"
#include "defines/globaldefines.h"
#include <vector>
#include <windows.h>

using namespace std;


int startCalculazione()
{
    deb<<"startCalculazione():\n";

    string sal;
    //sal = "1+sqrt_2(2+4+3)*ln(10+sqrt_3(27+sin(pi/2)*2)+55)+1456*pow_pi(e)";
    //sal="1";

    cout<<"\nIvesk salyga:\n";
    getline(cin, sal);

    cout<<"\nSalyga: "<<sal<<"\n\n------------------------------\n\n";

    deb<<"Salyga: "<<sal<<"\n";
    int timenow=GetTickCount();

    Calculator myCalc;
    myCalc.sendString(sal);

    double rezult = myCalc.getResultAsDouble();
    string resultstr= myCalc.getResultAsString();

    deb<<"\nRemind of salyga: "<<sal<<"\n\n";
    deb<<"------------------------------\n\n";
    deb<<"Result (asDouble): "<<rezult<<" ,Result (asString): "<<resultstr<<",  time of calculation: "<<GetTickCount()-timenow<<" ms.\n";

    cout.precision(FINAL_PRECISION);

    cout<<"Rezultatas: "<<resultstr<<" ,  skaic. laikas: "<<GetTickCount()-timenow<<" ms.\n";

    return 0;
}

//end;
