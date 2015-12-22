//Coded by H3nt4iB{}y (Gryllotalpa)


#ifndef FUN_H_INCLUDED
#define FUN_H_INCLUDED

#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>


class Fun
{
public:
    Fun(){ }
    ~Fun(){ }

    template <typename T>
    static std::string toString(T const& a);

    static double stringToDouble(std::string str, int where);
    static double stringToDoubleRem(std::string str, int & where);
    static double stringToDoubleRemA(std::string str, int where, int & remain);
    static int stringToInt(std::string str, int where=0);
    static void reverseDouble(double & num);
    static bool findTextComplex(std::string mainStr, std::string text, int where);
    static bool findText(std::string mainStr, std::string textToSearch, int where);
    static std::string getTimeString();
};

template <typename T> std::string Fun::toString(T const& a)
{
    std::stringstream strem;
    strem << std::setprecision( 15 ) << a;
    return strem.str();
}

#endif // FUN_H_INCLUDED
