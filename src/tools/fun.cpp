#include "tools/fun.h"
#include <ctime>


void Fun::reverseDouble(double & num)
{
    std::string numstr = toString((int)num);

    double res=0, cnt=1, frac;
    frac= num - (int)num;

    for(int i = 0; i < numstr.size(); i++)
    {
        res= res + ((double)(numstr[i] - 48)) * cnt;
        cnt*=10;
    }

    num = res + frac;
}

double Fun::stringToDouble(std::string str, int where)
{
    int wot=where;
    return stringToDoubleRem(str,wot);
}

double Fun::stringToDoubleRemA(std::string str, int where, int & remain)
{
    remain=where;
    return stringToDoubleRem(str,remain);
}

double Fun::stringToDoubleRem(std::string str, int & where)
{
    if(where > str.size()-1) return 0;

    double res=0, num=1, numfrac=10;
    bool frac=false, quit=false, numfound=false;
    int digs=0;

    for(int i=where; i<str.size(); i++)
    {
        if(str[i]>=48 && str[i]<58)
        {
            numfound=true;
            digs++;
        }
        else
        {
            if(numfound) break;
        }
    }

    num=pow(10, digs-1);

    numfound=false;

    int i;

    for(i=where; i<str.size(); i++)
    {
        if(str[i]>=48 && str[i]<58)
        {
            numfound=true;

            if(!frac)
            {
                res = res + ((double)(str[i]-48)) * num;
                num /= 10;
            }
            else
            {
                res = res + ((double)(str[i]-48)) / numfrac;
                numfrac *= 10;
            }
        }

        else if(str[i]==44 || str[i]==46)
        {
            if(!frac)
            {
                frac=true;
            }
            else
            {
                if(numfound) quit=true;
            }
        }

        else
        {
            if(numfound) quit=true;
        }

        if(quit)
        {
            break; //Stop the loop.
        }
    }

    where=i;

    return res;
}

bool Fun::findTextComplex(std::string mainStr, std::string text, int where)
{
    if(where >= mainStr.size()) return false;
    if(mainStr.size() < text.size()) return false;

    for(int i = 0; i<text.size(); i++)
    {
        if(mainStr[where] == text[i])
        {
            bool cool = true;

            for(int j = where-i; j<where-i+text.size(); j++)
            {
                if((j>=0 && mainStr[j] != text[j-(where-i)]) || j<0 || j>=mainStr.size() || j>=text.size()) cool=false;
            }

            if(cool) return true;
        }
    }

    return false;
}

bool Fun::findText(std::string mainStr, std::string textToSearch, int where)
{
    if(where >= mainStr.size() || where<0) return false;
    if(mainStr.size() < textToSearch.size()) return false;

    bool cool=true;

    for(int i=where; i < where+textToSearch.size(); i++)
    {
        if(i>=mainStr.size() || mainStr[i]!=textToSearch[i-where])
        {
            cool=false;
            break;
        }
    }

    return cool;
}

std::string Fun::getTimeString()
{
    time_t tim;
    struct tm * taim;
    char stime[80];

    time(&tim);
    taim = localtime(&tim);

    strftime(stime,80,"%x %X", taim);

    return (std::string)stime;
}

//end;
