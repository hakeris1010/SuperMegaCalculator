/*
SuperMegaCalculator by h3nt4iBoY a.k.a. Hakeris1010

- Lib Test.
*/

#include <iostream>
#include <string>
#include "Calcs/calc.h"

#define VERSION "v0.4.1"

using namespace std;


int main()
{
    Calculator myCalc;

    while(1)
    {
        cout<<"\nEnter salyga:\n";
        string sal;
        getline(cin, sal);
        if(sal=="exit") break;

        myCalc.clear();
        myCalc.sendString(sal);

        cout<<"\n-------------------------------\n\nSalyga: "<<sal<<"\n\n";
        cout<<"Result (asString): "<<myCalc.getResultAsString()<<"\n";
        cout<<"\n-------------------------------\n";
    }

    return 0;
}

//end;
