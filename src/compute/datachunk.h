#ifndef DATACHUNK_H_INCLUDED
#define DATACHUNK_H_INCLUDED

#include <vector>
#include <string>
#include "defines/calcdefines.h"

class dataChunk
{
private:
    int setOperator(std::string str, int pos);

public:
    bool isAnotherChunk=false;
    int positionInPrevious;

    int topCalc;
    double topNums[2];
    std::vector<double> numbers;
    std::vector<int> operators;
    std::vector<dataChunk> chunks;

    double calculate();
    void assignString(std::string str);

    dataChunk();
    ~dataChunk();
};

#endif // DATACHUNK_H_INCLUDED
