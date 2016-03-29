#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include "defines/calcdefines.h"
#include <vector>

struct OperParam
{
    bool isConst=false;
    double val;
};

class CalcOperator
{
public:
    CalcOperator(){ param = std::vector<OperParam>( CALC_MAX_MULTIPARAMS ); }
    CalcOperator(int _oper, int _paramCount=0, OperParam* _params=NULL)
    {
        CalcOperator();
        operation = _oper;
        paramCount = _paramCount;

        if(_paramCount>0 && _paramCount<param.size() && _params!=NULL)
        {
            for(int i=0; i<param.size(); i++)
                param[i] = _params[i];
        }
    }
    ~CalcOperator(){ }

    int operation=CALC_NONE;
    int paramCount=0;
    std::vector<OperParam> param;

    void clear()
    {
        operation=CALC_NONE;
        paramCount=0; //this was a big bug!

        for(int i=0; i<param.size(); i++)
        {
            param[i].isConst=false;
            param[i].val=0;
        }
    }
};

class CalcElement
{
public:
    CalcElement(){ }
    CalcElement(int _type, double _number = 0,
                CalcOperator _oper=CalcOperator() , std::string _strName=std::string())
    {
        type = _type;
        number = _number;
        strName = _strName;
        oper = _oper;
    }
    ~CalcElement(){ clear(); }

    int type;
    double number;
    std::string strName;
    CalcOperator oper;

    void clear()
    {
        type=CALC_NONE;
        number=0;
        oper.clear();
    }
};

//Enum of possible errors
enum CalcErrorCode {
    No_Error            = 0,
    No_Numbers          = 1,
    Bad_Pars            = 2,
    Unknown_Strings     = 3,
    Result_is_NaN       = 4,
    Result_is_INF       = 5,
    Bad_Multiparams     = 6,
    Incomplete_Calculation = 7,
    Same_Elem_Repeat    = 8,
    Invalid_Arrangement = 9
};

#endif // STRUCTS_H_INCLUDED
