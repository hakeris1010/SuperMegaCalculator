#ifndef CALC_H_INCLUDED
#define CALC_H_INCLUDED

#include <vector>
#include <string>
#include "tools/fun.h"
#include "defines/calcdefines.h"
#include "compute/structs.h"

class Calculator
{
private:
    std::string salyga;
    double result=-13.37;
    int calcErrCode=0;
    double lastNum;

    bool advancedMath=false;
    bool isCalculated=false;

    int startCalculation();

    void correctString(std::string & str);
    void calculateDaugs(std::vector<CalcElement> &elems);
    void calculateSpecials(std::vector<CalcElement> &elems);

    double recursiveChunkyCalculation(std::vector<CalcElement> elemso, int level=0);
    double makeCalculationFromChunk(std::vector<CalcElement> chunk);

    double calculateSudDaug(std::vector<CalcElement> elem, int mode);
    double skaicSpecial(CalcElement numop);

    bool operatorNext(std::string str, int pos);
    bool noMoreParentheses(std::vector<CalcElement> vec, int startPos);

public:
    Calculator();
    Calculator(int mode);
    ~Calculator();

    void sendString(std::string str);
    double getResultAsDouble();
    std::string getResultAsString();

};

#endif // CALC_H_INCLUDED
