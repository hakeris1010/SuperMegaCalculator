#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include "defines/calcdefines.h"

struct OperParam
{
    bool isConst=false;
    double val;
};

struct CalcOperator
{
    int operation;
    int paramCount=0;
    OperParam param[MAX_MULTIPARAMS];
};

struct CalcElement
{
    int type;
    double number;
    CalcOperator oper;

    void clear()
    {
        type=NONE;
        number=0;
        oper.operation=NONE;
        for(int i=0; i<MAX_MULTIPARAMS; i++)
        {
            oper.param[i].isConst=false;
            oper.param[i].val=0;
        }
    }
};

#endif // STRUCTS_H_INCLUDED
