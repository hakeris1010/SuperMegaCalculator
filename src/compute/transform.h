#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

#include <vector>
#include <string>
#include "compute/structs.h"

class Transformer
{
private:
    static bool isTempVariableGood(std::string str);
    static bool assignableChar(char ch);

public:
    Transformer();
    ~Transformer();

    static int setCalculationElements(std::string str, std::vector<CalcElement> &elems);
    static int postProcessCalcElements(std::vector<CalcElement> &elems);
    static int setOperator(std::string str, int pos=0);
    static void showElements(std::vector<CalcElement> eloo, int mode=0);
    static std::string codeToOperator(int opcode, double value=0);
    static double getValueFromElement(CalcElement elem, int param=-1);
    static double getValueFromConstant(int concode);
    static int codeCategory(int code);
    static void showCalcElementContents(CalcElement elem);
    static std::string getErrorString(CalcErrorCode errCode);
    static void insertElementToChunk(std::vector<CalcElement> &elems, int pos, CalcElement elemToInsert);
    static int getDefaultMultiparamCount(int opCode);
    static bool isMultiparamElementGood(CalcElement elem);
};

#endif // TRANSFORM_H_INCLUDED
