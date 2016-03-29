#include "compute/transform.h"
#include "tools/fun.h"
#include "defines/calcdefines.h"
#include "defines/globaldefines.h"
#include "tools/debug.h"
#include <iostream>
#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>


int Transformer::setCalculationElements(std::string str, std::vector<CalcElement> &elems)
{
    bool dbg=DEBUG_TRANSFORMER_SETCALCULATIONELEMENTS;
    if(dbg) deb<<"\n----------------------\nTrans::setCalculationElements():\n\n";
    if(dbg) deb<<"str.size() = "<<str.size()<<"\n";
    if(dbg) deb<<"Declaring vars, push-backing space to str...\n";

    str.push_back(char(32));

    int numCount=0, opCount=0, par1Count=0, par2Count=0, chunkCount=0, i=0;
    bool areAdvancedOps=false, onChar=false, operatorLast=false, onVariable=false;
    std::string temp;

    CalcErrorCode errCode = No_Error;

    if(str.size()==0) return 1; //empty!!!

    if(dbg) deb<<"Starting loop...\n";
    for(i=0; i<str.size(); i++)
    {
        if(dbg) deb<<"\n* * * *\nLoop of i="<<i<<"\n";
        //if(dbg) deb<<"States: onChar: "<<onChar<<", numCount="<<numCount<<", opCount="<<opCount<<",\ntemp(opStr)= "<<temp<<"\n~~~\n";

        if(onChar)
        {
            if(dbg) deb<<"onChar: Trying to set op from temp= "<<temp<<"\n";
            int opnum = setOperator(temp);

            if(opnum==CALC_NONE)
            {
                if(dbg) deb<<"Failed (no op by now)\n";
                bool canAssign = false;

                if(str[i]>='0' && str[i]<='9') //number
                {
                    if(isTempVariableGood(temp)) //variable like x2 or x12
                        canAssign = true;
                    else
                    {
                        if(dbg) deb<<"Bad temp and number straight after! ["<<temp<<"] (-noOP)\n";
                        errCode = Unknown_Strings;
                    }
                    //onChar = false; // NOT NEEDED
                }
                else //not number
                {
                    int maybe = setOperator(std::string(1, str[i])); //try to set operator from current i (e.g. +)
                    if( maybe!=CALC_NONE || i==(str.size()-1) )
                    {
                        if(isTempVariableGood(temp)) //var found & is good.
                        {
                            if(dbg) deb<<"\nmaybe!=CALC_NONE || end: Looks like we found a Variable! val= "<<temp<<"\n";
                            opnum = CALC_SIM_VAR;
                        }
                        else
                            errCode = Unknown_Strings;
                    }
                    else //maybe == CALC_NONE: can assign str[i];
                        canAssign = true;
                }
                if(canAssign)
                {
                    if(dbg) deb<<"Assigning another char ("<<str[i]<<")\n";
                    temp.push_back(str[i]);
                }

                if(i==str.size()-1 && opnum==CALC_NONE) //the end, and bad string onboard
                {
                    if(dbg) deb<<"Still no operation at end - Error: BadString!\n";
                    errCode = Unknown_Strings;
                }
            }
            if(opnum!=CALC_NONE)
            {
                if(opnum >= CALC_ADVANCED_START) areAdvancedOps=true;
                if(dbg) deb<<"\nWow! assigned op! val= "<<codeToOperator(opnum)<<"\n";
                if(dbg) deb<<"Inserting new element to vector.\n";

                CalcElement el;

                if(codeCategory(opnum)==CALC_CONSTANT)
                {
                    el = CalcElement(CALC_NUMBER, getValueFromConstant(opnum));
                    numCount++;
                }
                else if(codeCategory(opnum)==CALC_OPERATOR)
                    el = CalcElement(CALC_OPERATOR, 0, CalcOperator(opnum));

                else if(codeCategory(opnum)==CALC_EQUATIONAL)
                    el = CalcElement(CALC_EQUATIONAL, 0, CalcOperator(opnum), temp);

                elems.push_back(el);

                if(el.type != CALC_NUMBER)
                {
                    opCount++;
                    operatorLast=true;
                }
                else operatorLast=false;

                if(opnum==CALC_PAR1) par1Count++;
                if(opnum==CALC_PAR2) par2Count++;

                if(dbg) deb<<"Clearing temp...\n";

                temp.clear();
                onChar=false;
            }
        }

        if(!onChar)
        {
            if(dbg) deb<<"not onChar:\n";

            if(str[i]>='0' && str[i]<='9') //number
            {
                if(dbg) deb<<"Found number. ("<<str[i]<<") - Creating new element...\n";
                double num = Fun::stringToDoubleRem(str, i);
                i--;

                elems.push_back(CalcElement(CALC_NUMBER, num));
                numCount++;
                operatorLast=false;
            }
            else if(assignableChar(str[i])) //not number, but assignable
            {
                if(dbg) deb<<"Found char. ("<<str[i]<<") - assigning to temp.\n";
                onChar=true;
                temp.clear();
                temp.push_back(str[i]);
            }
        }
    }

    if(dbg) deb<<"\n*-*-*\nFinished transforming calculation. Result:\n";
    showElements(elems);

    if(numCount==0)          errCode = No_Numbers; //no numbers
    if(par1Count!=par2Count) errCode = Bad_Pars;   //bad count of parents

    if(!errCode) //if still no error, check for more!
    {
        if(dbg) deb<<"Starting Post-Processing for More Error Detection in vector<CalcElements>...\n";
        CalcErrorCode postProc = (CalcErrorCode) postProcessCalcElements(elems);

        if(postProc) errCode = postProc;   //error at post processing
    }

    if(dbg) deb<<"----------------------\n";

    return errCode; // No_Error = 0 when good.
}

void Transformer::insertElementToChunk(std::vector<CalcElement> &elems, int pos, CalcElement elemToInsert)
{
    if(pos > elems.size() || pos<0) return;
    elems.insert(elems.begin()+pos, elemToInsert);
}

//Error identification in chunk function.
int Transformer::postProcessCalcElements(std::vector<CalcElement> &elems)
{
    //Ultimate Error Checker...
    //TODO: Use Class ErrorChecker { ... };

    deb.setWriteDebugs(DEBUG_TRANSFORMER_POSTPROCESSCALCELEMENTS);

    deb<<"\n...::: Transformer::postProcessCalcElements() :::...\n\nSetting Vars...\n";
    bool afterNumber=false, afterVariable=false, afterSimpleOperator=false, afterAdvancedOperator=false;
    CalcErrorCode errCode = No_Error;

    deb<<"Loop Start!\n";
    for(int i=0; i<elems.size(); i++)
    {
        //check Currents
        if(elems[i].type == CALC_NUMBER)
        {
            for(int j=i-1; j<=i+1; j+=2) //check before i and after i (2 iters here)
            {
                if(j<0 || j>=elems.size())
                    break;

                if(elems[j].type==CALC_NUMBER) //error!
                {
                    deb<<"OnNumber: Error! Same Elements!\n";
                    errCode = Same_Elem_Repeat;
                    break;
                }
            }
            //idk what more
        }

        else if(elems[i].type == CALC_OPERATOR)
        {
            deb<<"Operator found on i="<<i<<" : "<<codeToOperator(elems[i].oper.operation)<<"\n";
            for(int j=i-1; j<=i+1; j+=2) //check before i and after i (2 iters here)
            {
                if(j<0) //checkings if first or last
                    continue;
                else if(j>=elems.size())
                    break;

                //universal:
                //if first or after '(': bads: '+',...,'*' (ex. '-'), '^', '_'
                bool firstBad=( ( i==0 || (j<i ? elems[j].oper.operation==CALC_PAR1 : 0) ) ?
                                ( (elems[i].oper.operation<CALC_ADVANCED_START && elems[i].oper.operation!=CALC_MIN) ||
                                  (elems[i].oper.operation>=CALC_OTHERSPECIAL_START && elems[i].oper.operation<CALC_OTHERSPECIAL_END) ||
                                   elems[i].oper.operation==CALC_SEPAR ) : false );

                //if last or before ')': everything but ')'
                bool lastBad=((i==elems.size()-1 || (i<j ? elems[j].oper.operation==CALC_PAR2 : 0)) ? (elems[i].oper.operation!=CALC_PAR2) : 0);

                if(lastBad || firstBad) //if operator is bad last (2ln or 2+) or bad first (*2 or ^2)
                {
                    deb<<"OnOperator: Error! Invalid Arrangement (last or first bad (or before/after pars)\n";
                    errCode = Invalid_Arrangement;
                    break;
                }

                //operator j:
                if(elems[j].type==CALC_OPERATOR) //check!
                {
                    bool c1= elems[i].oper.operation<CALC_ADVANCED_START;  //c1: i is simple
                    bool c2= elems[j].oper.operation<CALC_ADVANCED_START; //c2: j is simple

                    bool cmi= elems[i].oper.operation==CALC_MIN;
                    bool cmj= elems[j].oper.operation==CALC_MIN;
                    bool cex1= (j<i ? (c2 && cmi) : (c1 && cmj) ); //exception 1 (*-)

                    bool c3= (elems[i].oper.operation>=CALC_ADVANCED_START && elems[i].oper.operation<CALC_NON_CALCULATIVE_START); //i is advanced.
                    bool c4= (elems[j].oper.operation>=CALC_ADVANCED_START && elems[j].oper.operation<CALC_NON_CALCULATIVE_START); //j is advanced.

                    /*bool cpi= elems[i].oper.operation>=CALC_OTHERSPECIAL_START && elems[i].oper.operation<CALC_OTHERSPECIAL_END;
                    bool cpj= elems[j].oper.operation>=CALC_OTHERSPECIAL_START && elems[j].oper.operation<CALC_OTHERSPECIAL_END;
                    bool cex2= (j<i ? (cpj && c3) : (cpi && c4) ); //exception 2 (^ln) //ERROOOOOOOOOOOORRRRRRR!!1 */

                    bool c5= (elems[i].oper.operation==CALC_SEPAR && elems[j].oper.operation==CALC_SEPAR); //both are separators

                    bool c6= (elems[i].oper.operation==CALC_PAR1 && elems[j].oper.operation==CALC_PAR2); // '(' and ')'
                    bool c7= (elems[i].oper.operation==CALC_PAR2 && elems[j].oper.operation==CALC_PAR1); // ')' and '(' - we will * them.

                    //if both are simple (but not *-), both advanced, or if both are separators.
                    if((c1 && c2 && !cex1) || (c3 && c4 /*&& !cex2*/) || c5 || c6)
                    {
                        deb<<"OnOperator: Error: Repeat!\n";
                        errCode = Same_Elem_Repeat;
                        break;
                    }
                    //if j=i+1 then if i is advanced but j is simple (ln+)
                    if((j>i ? (c3 && c2) : false) || (j>i ? c6 : c7))
                    {
                        deb<<"OnOperator: Error! Invalid Arrangement (middle ln+)\n";
                        errCode = Invalid_Arrangement;
                        break;
                    }

                    if(j>i ? c7 : c6) // if (2+2)(2+2)
                    {
                        deb<<"Found (..)(..). Inserting \'*\'...\n";
                        elems.insert(elems.begin()+(j<i ? i : j) , CalcElement(CALC_OPERATOR, 0, CalcOperator(CALC_DAU)));
                    }
                }
                //number j:
                else if(elems[j].type==CALC_NUMBER) //check! [NOT WORKING!!!] (FIXME)
                {
                    //situation 1: "8 (" or ") 8"
                    bool cn1= (j<i ? elems[i].oper.operation==CALC_PAR1 : elems[i].oper.operation==CALC_PAR2);
                    //situation 2: i:[adv_op]
                    bool cn2= (elems[i].oper.operation>=CALC_ADVANCED_START && elems[i].oper.operation<CALC_NON_CALCULATIVE_START);
                    //situation 3: i:[other_sp] e.g. '^'
                    bool cn3= (elems[i].oper.operation>=CALC_OTHERSPECIAL_START && elems[i].oper.operation<CALC_OTHERSPECIAL_END);

                    if(cn1)
                    {
                        deb<<"Found \"4 (\" or \") 4\". Inserting \'*\'...\n";
                        elems.insert(elems.begin()+(j<i ? i : j) , CalcElement(CALC_OPERATOR, 0, CalcOperator(CALC_DAU)));
                    }
                    if(cn2 && !cn3 && j<i) //4ln
                    {
                        deb<<"Found \"4 ln\". Error!\n";
                        errCode = Invalid_Arrangement;
                    }
                }
            }
        }

        else if(elems[i].type==CALC_EQUATIONAL && elems[i].oper.operation==CALC_SIM_VAR)
        {
            for(int j=i-1; j<=i+1; j+=2) //check before i and after i (2 iters here)
            {
                if(j<0 || j>=elems.size())
                    break;
                //if situation like 'x ln', but not 'x ^' (when j=i+1)
                bool xAdvanced = (j>i ? (elems[j].type==CALC_OPERATOR && elems[j].oper.operation>=CALC_ADVANCED_START &&
                   elems[j].oper.operation<CALC_NON_CALCULATIVE_START && elems[j].oper.operation!=CALC_POWS) : 0 );
                //if x 8 or 8 x
                bool onNumber = (elems[j].type==CALC_NUMBER);

                if(onNumber || xAdvanced) //insert '*' (edit!)
                {
                    deb<<"Found 8x or 8ln. Inserting \'*\'...\n";
                    elems.insert(elems.begin()+(j<i ? i : i+1) , CalcElement(CALC_OPERATOR, 0, CalcOperator(CALC_DAU)));
                }
            }
        }

        if(errCode != No_Error) //default handler
        {
            deb<<"Error found! ["<<getErrorString(errCode)<<"].\n";
            break;
        }
    }
    deb<<"\n...::: Post Processing Ended. :::...\n\n";
    deb.setWriteDebugs(true);

    return errCode;
}

bool Transformer::isTempVariableGood(std::string str) //accepts only like this: x, x1, x12
{
    if(str.size()==0)
        return false;

    if( !((str[0]>='a' && str[0]<='z') || (str[0]>='A' && str[0]<='Z')) ) //not letter first
        return false;

    for(int i=1; i<str.size(); i++)
    {
        if(str[i]<'0' || str[i]>'9') //not number
            return false;
    }
    return true;
}

void Transformer::showElements(std::vector<CalcElement> eloo, int mode)
{
    if(!DEBUG_TRANSFORMER_SHOWELEMENTS) return;

    if(mode==0) deb<<"\n~-~-~-~\nTrans::showElements():\n\n";

    std::stringstream sts;

    for(int i=0; i<eloo.size(); i++)
    {
        if(mode==0) deb<<"no. "<<i<<": type=";

        if(eloo[i].type == CALC_NUMBER)
        {
            if(mode==0) deb<<"number, val="<<eloo[i].number<<"\n";
            sts<<eloo[i].number<<" ";
        }
        else if(eloo[i].type == CALC_OPERATOR)
        {
            if(mode==0) deb<<"operator, code="<<eloo[i].oper.operation<<", op= "<<codeToOperator(eloo[i].oper.operation)<<"\n";
            sts<< codeToOperator(eloo[i].oper.operation) <<" ";
        }
        else if(eloo[i].type == CALC_CONSTANT)
        {
            if(mode==0) deb<<"constant, code="<<eloo[i].number<<", op= "<<codeToOperator(eloo[i].number)<<"\n";
            sts<< codeToOperator(eloo[i].number) <<" ";
        }
        else if(eloo[i].type == CALC_EQUATIONAL)
        {
            if(mode==0) deb<<"Equational, code="<<eloo[i].oper.operation<<", op= "<<eloo[i].strName<<"\n";
            sts<< eloo[i].strName <<" ";
        }

        else {if(mode==0) deb<<"unknown\n";}
    }
    deb<<"\nAs String: "<<sts.str()<<"\n";
    if(mode==0) deb<<"~-~-~-~\n";
}

void Transformer::showCalcElementContents(CalcElement elem)
{
    if(!DEBUG_TRANSFORMER_SHOWCALCELEMENTCONTENTS) return;

    deb<<"type: "<<codeToOperator(elem.type)<<"\nnumber: "<<elem.number<<"\noper.operation: "<<codeToOperator(elem.oper.operation)<<"\n";
    for(int i=0; i<elem.oper.paramCount; i++)
    {
        if(!elem.oper.param[i].isConst) deb<<"oper.param["<<i<<"]: "<<elem.oper.param[i].val<<"\n";
        else deb<<"oper.param["<<i<<"]: "<<codeToOperator(elem.oper.param[i].val)<<"\n";
    }
}

bool Transformer::assignableChar(char ch)
{
    if(ch==char(32)) return false;
    return true;
}

int Transformer::setOperator(std::string str, int pos)
{
     //SIMPLE ONES
    if(str[pos] == '+' ) return CALC_PLU;
    else if(str[pos] == '-' ) return CALC_MIN;
    else if(str[pos] == '*' ) return CALC_DAU;
    else if(str[pos] == '/' ) return CALC_DAL;
    else if(str[pos] == '(' ) return CALC_PAR1;
    else if(str[pos] == ')' ) return CALC_PAR2;
    else if(str[pos] == '_' ) return CALC_SEPAR;
    else if(str[pos] == '^' ) return CALC_POWS;

    //COMPLEX ONES
    else if(Fun::findText(str, "mod", pos) || Fun::findText(str, "MOD", pos)) return CALC_MOD;

    else if(Fun::findText(str, "pow", pos) || Fun::findText(str, "POW", pos)) return CALC_POWFUN;

    else if(Fun::findText(str, "sqrt", pos) || Fun::findText(str, "SQRT", pos)
            || Fun::findText(str, "rt", pos) || Fun::findText(str, "RT", pos)) return CALC_SAK;

    else if(Fun::findText(str, "sin", pos) || Fun::findText(str, "SIN", pos)) return CALC_SIN;

    else if(Fun::findText(str, "cos", pos) || Fun::findText(str, "COS", pos)) return CALC_COS;

    else if(Fun::findText(str, "tan", pos) || Fun::findText(str, "TAN", pos) ||
            Fun::findText(str, "tg", pos) || Fun::findText(str, "TG", pos)) return CALC_TAN;

    else if(Fun::findText(str, "ctan", pos) || Fun::findText(str, "CTAN", pos) ||
            Fun::findText(str, "ctg", pos) || Fun::findText(str, "CTG", pos)) return CALC_CTAN;

    else if(Fun::findText(str, "arcsin", pos) || Fun::findText(str, "ARCSIN", pos)) return CALC_ASIN;

    else if(Fun::findText(str, "arccos", pos) || Fun::findText(str, "ARCCOS", pos)) return CALC_ACOS;

    else if(Fun::findText(str, "arctan", pos) || Fun::findText(str, "ARCTAN", pos) ||
            Fun::findText(str, "arctg", pos) || Fun::findText(str, "ARCTG", pos)) return CALC_ATAN;

    else if(Fun::findText(str, "arcctan", pos) || Fun::findText(str, "ARCCTAN", pos) ||
            Fun::findText(str, "arcctg", pos) || Fun::findText(str, "ARCCTG", pos)) return CALC_ACTAN;

    else if(Fun::findText(str, "lg", pos) || Fun::findText(str, "LG", pos)) return CALC_LG;
    else if(Fun::findText(str, "ln", pos) || Fun::findText(str, "LN", pos)) return CALC_LN;
    else if(Fun::findText(str, "log", pos) || Fun::findText(str, "LOG", pos)) return CALC_LOG;

    else if(Fun::findText(str, "%e", pos) || Fun::findText(str, "%E", pos)) return CALC_CON_E;    //constants will be with %
    else if(Fun::findText(str, "%pi", pos) || Fun::findText(str, "%PI", pos)) return CALC_CON_PI;

    //else if(Fun::findText(str, "", pos) || Fun::findText(str, "", pos)) return CALC_;

    else return CALC_NONE; //None found
}

std::string Transformer::codeToOperator(int opcode, double value)
{
    switch(opcode)
    {
    case CALC_PLU: return "+";
    case CALC_MIN: return "-";
    case CALC_DAU: return "*";
    case CALC_DAL: return "/";
    case CALC_PAR1: return "(";
    case CALC_PAR2: return ")";
    case CALC_POWFUN: return "pow";
    case CALC_SEPAR: return "_";
    case CALC_SAK: return "sqrt";
    case CALC_SIN: return "sin";
    case CALC_COS: return "cos";
    case CALC_TAN: return "tan";
    case CALC_CTAN: return "ctan";
    case CALC_ASIN: return "arcsin";
    case CALC_ACOS: return "arccos";
    case CALC_ATAN: return "arctan";
    case CALC_ACTAN: return "arcctan";
    case CALC_LG: return "lg";
    case CALC_LN: return "ln";
    case CALC_LOG: return "log";
    case CALC_POWS: return "^";
    case CALC_CON_PI: return "const: pi";
    case CALC_CON_E: return "const: e";

    case CALC_SIM_VAR: return "x";

    case CALC_NUMBER: return "NUMBER";
    case CALC_OPERATOR: return "OPERATOR";
    case CALC_CONSTANT: return "CONSTANT";

    case CALC_NONE: return "NONE";
    }

    return "Undefined";
}

double Transformer::getValueFromElement(CalcElement elem, int parm)
{
    double val=0;

    if(parm>=0)
    {
        if(parm<CALC_MAX_MULTIPARAMS && elem.oper.operation>=CALC_MULTIPARAM_START && elem.oper.operation<CALC_MULTIPARAM_END)
        {
            if(!elem.oper.param[parm].isConst) val= elem.oper.param[parm].val;
            else val= getValueFromConstant(elem.oper.param[parm].val);
        }
        //else return NAN;
    }
    else
    {
        if(elem.type==CALC_NUMBER) val= elem.number;
        else if(elem.type==CALC_CONSTANT) val= getValueFromConstant(elem.number);
        //return NAN;
    }

    return val;
}

double Transformer::getValueFromConstant(int concode)
{
    double val=0;

    if(concode == CALC_CON_PI) val = 2*asin(1);
    if(concode == CALC_CON_E) val = exp(1);

    return val;
}

int Transformer::codeCategory(int code)
{
    if(code>=CALC_CONSTANT_START          && code<CALC_CONSTANT_END)          return CALC_CONSTANT;
    if(code>=CALC_STANDART_OPERATOR_START && code<CALC_STANDART_OPERATOR_END) return CALC_OPERATOR;
    if(code>=CALC_TYPE_START              && code<CALC_TYPE_END)              return CALC_TYPE;
    if(code>=CALC_EQUATIONAL_START        && code<CALC_EQUATIONAL_END)        return CALC_EQUATIONAL;

    return CALC_UNKNOWN;
}

std::string Transformer::getErrorString(CalcErrorCode errCode)
{
    switch(errCode)
    {
        case CalcErrorCode::No_Error: return Fun::toString(errCode)+": No_Error";
        case CalcErrorCode::Bad_Pars: return Fun::toString(errCode)+": Bad_Pars";
        case CalcErrorCode::No_Numbers : return Fun::toString(errCode)+": No_Numbers";
        case CalcErrorCode::Result_is_INF : return Fun::toString(errCode)+": Result_is_INF";
        case CalcErrorCode::Result_is_NaN : return Fun::toString(errCode)+": Result_is_NaN";
        case CalcErrorCode::Unknown_Strings : return Fun::toString(errCode)+": Unknown_Strings";
        case CalcErrorCode::Bad_Multiparams : return Fun::toString(errCode)+": Bad_Multiparams";
        case CalcErrorCode::Invalid_Arrangement : return Fun::toString(errCode)+": Invalid_Arrangement";
        case CalcErrorCode::Same_Elem_Repeat : return Fun::toString(errCode)+": Same_Elem_Repeat";
        case CalcErrorCode::Incomplete_Calculation : return Fun::toString(errCode)+": Incomplete_Calculation";

        //case CalcErrorCode : return Fun::toString(errCode)+": ";
    }
    return Fun::toString(errCode);
}

int Transformer::getDefaultMultiparamCount(int opCode)
{
    switch(opCode)
    {
        case CALC_POWS      : return 2; //a^b

        case CALC_LOG       : return 1; //log_a_(...)
        case CALC_POWFUN    : return 1; //pow_a_(...)
        case CALC_SAK       : return 1; //rt_a_(...)

        case CALC_LONG_SUM  : return 3; //x, start, end
        case CALC_LONG_SAND : return 3; //x, start, end

        case CALC_DEF_INTEG : return 3; //x, a, b
        case CALC_DIFF_N    : return 2; //x, n (f(n)(x))
        case CALC_LIMIT     : return 2; //x, a (x->a)

        //case CALC : return
    }
    return 0;
}

bool Transformer::isMultiparamElementGood(CalcElement elem)
{
    if(elem.oper.paramCount == getDefaultMultiparamCount(elem.oper.operation))
        return true;

    return false;
}

//end;
