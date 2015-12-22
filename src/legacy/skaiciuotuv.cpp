#include "legacy/skaiciuotuv.h"
#include <iostream>

using namespace std;

//publics
Skaiciuotuvas::Skaiciuotuvas(int mode)
{
    mod=mode;
}

void Skaiciuotuvas::run()
{
    if(mod==1) papr();
    else sud();
}

void Skaiciuotuvas::set_mode(int mode)
{
    mod=mode;
}

//privates

void Skaiciuotuvas::papr()
{
    cout<<"\n****************************************\n";
    cout<<"           Iveskite veiksma: \n\n";

    skaic_veiksm();

    cout<<"\n\n****************************************\n\n";
}

void Skaiciuotuvas::sud()
{

}

double Skaiciuotuvas::skaic_veiksm()
{
    double sk[100];
    char zn[100];
}

double Skaiciuotuvas::spec_veiksm()
{

}

// skaiciuotuvo pabaiga

void menu(string st1=" ", string st2=" ")
{
    int i;
    cout<<"---------------------------------\n\n";
    cout<<"1. "<<st1<<endl;
    if(st2!=" ") cout<<"2. "<<st2<<endl;
    cout<<"\n---------------------------------\n\n";
}

void paskaiciuok() //starteris
{
    Skaiciuotuvas calc(1);

    bool quit=false;
    int mode=1;
    char sel;

    cout<<"\n SKAICIUOTUVAS  "<<"\n";

    while(quit==false)
    {
        menu("Skaiciuoti!","Nustatymai");
        cin>>sel;
        if(sel=='2')
        {
            menu("Pasirinkite skaiciavimo rezima (1- paprastas, 2- sudetingas)");
            cin>>mode;
            if(mode>2) mode=2;
            calc.set_mode(mode);
        }
        else
        {
            calc.run();
        }

        menu("Ar norite testi? [t/n]");
        cin>>sel;
        if(sel=='n') quit=true;
    }
}
