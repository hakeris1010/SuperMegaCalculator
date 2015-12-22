#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

#include <vector>
#include <string>
#include "compute/structs.h"

class Transformer
{
public:
    Transformer();
    ~Transformer();

    static int setCalculationElements(std::string str, std::vector<CalcElement> &elems);
    static int setOperator(std::string str, int pos=0);
    static void showElements(std::vector<CalcElement> eloo, int mode=0);
    static std::string codeToOperator(int opcode);
    static double getValueFromElement(CalcElement elem, int param=-1);
    static double getValueFromConstant(int concode);
    static int codeCategory(int code);
    static void showCalcElementContents(CalcElement elem);
};

#endif // TRANSFORM_H_INCLUDED
