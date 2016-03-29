#include "calcstart.h"
#include "compute/calc.h"
#include "tools/debug.h"
#include "defines/globaldefines.h"
#include <iostream>
#include <windows.h>

int startCalculazione()
{
    deb<<"startCalculazione():\n";

    Calculator myCalc;

    std::string sal;
    //sal = "1+sqrt_2(2+4+3)*ln(10+sqrt_3(27+sin(pi/2)*2)+55)+1456*pow_pi(e)";
    //sal="1";

    while(1)
    {
        std::cout<<"\nEnter expression: (exit to close)\n";
        getline(std::cin, sal);

        if(sal=="exit") //end loop
            break;

        std::cout<<"\nSalyga: "<<sal<<"\n";

        if(sal=="Nyaa :3") //just some otaku stuff.
            std::cout<<"\n   Kawaii desu~ (^_^)\n";

        std::cout<<"\n------------------------------\n\n";

        deb<<"Expression: "<<sal<<"\n";
        int timenow=GetTickCount();

        myCalc.clear();
        myCalc.sendString(sal);

        double rezult = myCalc.getResultAsDouble();
        std::string resultstr= myCalc.getResultAsString();

        deb<<"\nRemind of salyga: "<<sal<<"\n\n";
        deb<<"------------------------------\n\n";
        deb<<" Result (asDouble): "<<rezult<<"\n Result (asString): "<<resultstr<<"\n time of calculation: "<<GetTickCount()-timenow<<" ms.\n";

        std::cout.precision(FINAL_PRECISION);

        std::cout<<"\nResult: "<<resultstr<<"\ncalc. time: "<<GetTickCount()-timenow<<" ms.\n\n";
    }

    return 0;
}

//end;
