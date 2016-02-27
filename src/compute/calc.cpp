#include <iostream>
#include <cmath>
#include "compute/calc.h"
#include "compute/transform.h"
#include "defines/globaldefines.h"
#include "tools/debug.h"

Calculator::Calculator(){ }

Calculator::Calculator(int mode)
{
    if(mode==1) advancedMath = true;
}

Calculator::~Calculator()
{
    salyga.clear();
}

void Calculator::sendString(std::string str)
{
    isCalculated=false;
    deb<<"\nNew operation assigned: "<<str<<"\n";

    correctString(str);
    deb<<"\nAfter correction: "<<str<<"\n";

    salyga=str;
    int ero = startCalculation();

    if(ero!=0) calcErrCode=ero;
    if(std::isnan(result)) calcErrCode=6;
    if(std::isinf(result)) calcErrCode=7;

    isCalculated=true;
}

double Calculator::getResultAsDouble()
{
    if(isCalculated)
    {
        if(calcErrCode==0) return result;
        else return calcErrCode;
    }
    else return 0;
}

std::string Calculator::getResultAsString()
{
    if(isCalculated)
    {
        if(calcErrCode==0) return Fun::toString(result);
        else return "Error (not nyanyanya). errCode: "+Fun::toString(calcErrCode);
    }
    return "Still Calculating!";
}

int Calculator::startCalculation()
{
    std::vector<CalcElement> elemen;

    int erval = Transformer::setCalculationElements(salyga, elemen);

    if(erval!=0)
    {
        deb<<"Something's wrong with the equation... Correct it, madafaka! (errcode: "<<erval<<")\n";
        return erval;
    }

    result = recursiveChunkyCalculation(elemen, 1);

    return 0;
}

void Calculator::correctString(std::string & str)
{
    //something in the future.
}

bool Calculator::operatorNext(std::string str, int pos)
{
    if(pos >= str.size()) return false;

    if(str[pos]>='0' && str[pos]<='9') return false;
    if(str[pos]=='(' || str[pos]==')') return false;

    return true;
}

double Calculator::recursiveChunkyCalculation(std::vector<CalcElement> elemso, int level) //works great!
{
    bool dbg=DEBUG_CALCULATOR_RECURSIVECHUNKYCALCULATION;

    if(dbg) deb<<"\n-------------------------\nCalc::recursiveChunkyCalculation(), level="<<level<<"\n\n";
    if(dbg) deb<<"Got salyga:\n";
    Transformer::showElements(elemso,1);
    if(dbg) deb<<"\n";

    bool areChunksInside=false, nowWritingIntoChunk=false, areNumsInInnerChunk=false, afterMultiparam=false;
    int par1skInChun=0, par2skInChun=0, yolofakka=-1337, i=0, innerChunkStartPos=0, innerChunkLenght=0;

    std::vector<CalcElement> currentChunk;
    std::vector<CalcElement> insideChunk;

    if(dbg) deb<<"Declared vars. Starting loop...\n";

    for(i=0; i<elemso.size(); i++)
    {
        if(dbg) deb<<"\n*****\nLoop no. "<<i<<" \n";
        if(dbg) deb<<"States: areChunksInside: "<<areChunksInside<<", nowWritingIntoChunk: "<<nowWritingIntoChunk<<", areNumsInInnerChunk: "<<areNumsInInnerChunk<<"\n";
        if(dbg) deb<<"par1skInChun="<<par1skInChun<<", par2skInChun="<<par2skInChun<<", innerChunkStartPos="<<innerChunkStartPos<<", innerChunkLenght="<<innerChunkLenght<<"\n~~~\n";

        if(elemso[i].type==OPERATOR)
        {
            if(dbg) deb<<"Found: operator, val= "<<Transformer::codeToOperator(elemso[i].oper.operation)<<" \n";

            if(!nowWritingIntoChunk) //on main chunk
            {
                if(elemso[i].oper.operation == PAR1)
                {
                    if(dbg) deb<<"Par1 found! Starting inner chunk writing. \n";

                    nowWritingIntoChunk=true;
                    areChunksInside=true;
                    innerChunkStartPos=i;
                    insideChunk.clear();

                    if(dbg) deb<<"innerChunkStartPos="<<innerChunkStartPos<<" \n";
                }

                //assign op to a current vec.
                if(!nowWritingIntoChunk)
                {
                    if(dbg) deb<<"Assigning op to currentChunk... \n";
                    currentChunk.push_back(elemso[i]);

                    if(afterMultiparam &&  elemso[i].oper.operation != SEPAR) afterMultiparam=false;

                    if(elemso[i].oper.operation >= MULTIPARAM_START && elemso[i+1].oper.operation != SEPAR) afterMultiparam=true;
                }
            }
            else //writing into insider chunk.
            {
                if(elemso[i].oper.operation == PAR1) par1skInChun++;
                if(elemso[i].oper.operation == PAR2) par2skInChun++;

                if(par2skInChun<=par1skInChun)
                {
                    if(dbg) deb<<"Assigning op to insideChunk... \n";
                    insideChunk.push_back(elemso[i]); //assign op to insideChunk
                    innerChunkLenght++;
                }
            }
        }

        else if(elemso[i].type==NUMBER)
        {
            if(dbg) deb<<"Found: number, val="<<elemso[i].number<<" \n";

            if(!nowWritingIntoChunk)
            {
                if(dbg) deb<<"Assigning number to currentChunk... \n";

                currentChunk.push_back(elemso[i]);
            }
            else
            {
                if(dbg) deb<<"Assigning number to insideChunk... \n";

                areNumsInInnerChunk=true;
                insideChunk.push_back(elemso[i]);
                innerChunkLenght++;
            }
        }

        if(nowWritingIntoChunk && areNumsInInnerChunk && par2skInChun>par1skInChun) //inside chunk end (if par2 sk > par1 sk, then it is.)
        {
            if(dbg) deb<<"\nReached inner chunk end (par2sk>par1sk). Sending innerChunk to another recursiveFunction!!! \n";

            double innerChunkResult = recursiveChunkyCalculation(insideChunk, level+1);

            if(dbg) deb<<"\nrecursiveChunkyCalc() returned value: "<<innerChunkResult<<"\nCreating a new element... \n";

            CalcElement cel;
            cel.type=NUMBER;
            cel.number=innerChunkResult;

            if(dbg) deb<<"Erasing innerChunk elements in main chunk... \n";
            elemso.erase(elemso.begin()+innerChunkStartPos, elemso.begin()+i+1);

            if(dbg) deb<<"Inserting new value in main chunk to position:"<<innerChunkStartPos<<" \n";

            if(afterMultiparam)
            {
                CalcElement sep;
                sep.type=OPERATOR;
                sep.oper.operation=SEPAR;

                elemso.insert(elemso.begin()+innerChunkStartPos, sep);
                elemso.insert(elemso.begin()+innerChunkStartPos+1, cel);

                i=i-innerChunkLenght;
                afterMultiparam=false;
            }
            else
            {
                elemso.insert(elemso.begin()+innerChunkStartPos, cel);
                i=i-innerChunkLenght-1;
            }

            if(dbg) deb<<"\nChunk after erasing and inserting:\n";
            Transformer::showElements(elemso,1);
            if(dbg) deb<<"\n";

            if(dbg) deb<<"Defaulting innerChunk-related state vars... \n";

            par1skInChun=0;
            par2skInChun=0;
            innerChunkStartPos=0;
            areNumsInInnerChunk=false;
            nowWritingIntoChunk=false;
            innerChunkLenght=0;
        }
    }

    if(dbg) deb<<"\n*****\nLoop end. Final equation:\n";
    Transformer::showElements(elemso, 1);
    if(dbg) deb<<"Returning value from makeCalculationFromChunk()...\n------------------- \n";

    return makeCalculationFromChunk(elemso);
}

double Calculator::makeCalculationFromChunk(std::vector<CalcElement> chunk)
{
    double calcuResult=(double)chunk.size();

    identifyNegatives(chunk);
    calculateSpecials(chunk); //Exponents
    calculateDaugs(chunk); //Multiplication, Division

    calcuResult = calculateSudDaug(chunk,1); //Addition, Subtraction

    return calcuResult;
}

double Calculator::calculateSudDaug(std::vector<CalcElement> elem, int mode) //works kind of good.
{
    double rezl=0;
    int startPos=-1;
    int lastOp=NONE;

    for(int i=0; i<elem.size(); i++)
    {
        if(elem[i].type==NUMBER)
        {
            startPos=i;
            break;
        }
    }

    if(startPos>=0)
    {
        if(elem[startPos].type==NUMBER) rezl=elem[startPos].number;

        for(int i=startPos+1; i<elem.size(); i++)
        {
            if(elem[i].type==OPERATOR) lastOp=elem[i].oper.operation;

            if(elem[i].type==NUMBER)
            {
                if(mode==1)
                {
                    if(lastOp==PLU) rezl+=elem[i].number;
                    if(lastOp==MIN) rezl-=elem[i].number;
                }
                if(mode==2)
                {
                    if(lastOp==DAU) rezl*=elem[i].number;
                    if(lastOp==DAL) rezl/=elem[i].number;
                }
            }
        }
    }

    return rezl;
}

void Calculator::identifyNegatives(std::vector<CalcElement> &elems)
{
    bool dbg = DEBUG_CALCULATOR_IDENTIFYNEGATIVES;
    if(dbg) deb<<"\n- - - - -\nCalc::identifyNegatives():\nGot vector:\n";
    Transformer::showElements(elems,1);

    CalcElement temp;
    bool needToDoStuff=false;

    for(int i=0; i<elems.size(); i++)
    {
        if((elems[i].type==OPERATOR && elems[i].oper.operation == MIN))
        {
            if(i==0) needToDoStuff=true;
            else if(elems[i-1].type != NUMBER && elems[i-1].oper.operation != PAR2) needToDoStuff=true;
        }

        if(needToDoStuff)
        {
            temp.clear();
            temp.type=OPERATOR;
            temp.oper.operation=PAR1;

            elems.insert(elems.begin()+i, temp);
            i++;

            temp.clear();
            temp.type=NUMBER;
            temp.number=0;

            elems.insert(elems.begin()+i, temp);
            i++;

            temp.clear();
            temp.type=OPERATOR;
            temp.oper.operation=PAR2;

            int j=0;
            for(j=0; j<elems.size()-i; j++)
            {
                if(elems[i+j].type==NUMBER)
                {
                    elems.insert(elems.begin()+i+j+1, temp);
                    break;
                }
            }

            needToDoStuff=false;
        }
    }

    deb<<"Elems after negative id-cation:\n";
    Transformer::showElements(elems,1);
}

void Calculator::calculateDaugs(std::vector<CalcElement> &elems) //Bug!
{
    bool dbg = DEBUG_CALCULATOR_CALCULATEDAUGS;

    if(dbg) deb<<"\n- - - - -\nCalc::calculateDaugs():\nGot vector:\n";
    Transformer::showElements(elems,1);

    std::vector<CalcElement> daugs;
    bool onDaug=false, onDaugFirstTime=false, mustEnd=false;
    int daugStartPos=0, daugLenght=0;

    if(dbg) deb<<"Starting loop...\n";

    for(int i=0; i<elems.size(); i++)
    {
        if(dbg) deb<<"\n* * *\nLoop of i= "<<i<<":\nState: ";
        if(dbg) deb<<"onDaug: "<<onDaug<<", onDaugFirstTime: "<<onDaugFirstTime<<", daugStartPos= "<<daugStartPos<<", daugLenght= "<<daugLenght<<"\n~~~\n";

        if(!onDaug && elems[i].type == OPERATOR && (elems[i].oper.operation==DAU || elems[i].oper.operation==DAL))
        {
            if(dbg) deb<<"!onDaug: Found DAU/DAL operator! Setting onDaug and others->true.\n";

            onDaug=true;
            onDaugFirstTime=true;
            if(i>0) daugStartPos=i-1;
            daugs.clear();
        }

        if(onDaug)
        {
            if(onDaugFirstTime && i>0)
            {
                if(dbg) deb<<"onDaugFirstTime. inserting an element from i-1.\n";
                daugs.push_back(elems[i-1]);
                daugLenght++;
                onDaugFirstTime=false;
            }
            if(dbg) deb<<"inserting daug element.\n";
            daugs.push_back(elems[i]);
            daugLenght++;
        }

        if( onDaug && ((elems[i].type == OPERATOR && (elems[i].oper.operation!=DAU && elems[i].oper.operation!=DAL)) || i>=(elems.size()-1)) )
        {
            if(i>=(elems.size()-1)) {if(dbg) deb<<"onDaug: End of equation! calculating daug result: ";}
            else {if(dbg) deb<<"onDaug: found not DAU op! calculating daug result: ";}

            if(i>=(elems.size()-1)) mustEnd=true;

            double daures = calculateSudDaug(daugs, 2);

            if(dbg) deb<<daures<<"\nAssigning to vector, erasing dauged members...\n";

            CalcElement ce;
            ce.type=NUMBER;
            ce.number=daures;

            if(!mustEnd) elems.erase(elems.begin()+daugStartPos, elems.begin()+i);
            else elems.erase(elems.begin()+daugStartPos, elems.begin()+i+1);

            elems.insert(elems.begin()+daugStartPos, ce);

            if(dbg) deb<<"Vector after modification:\n";
            Transformer::showElements(elems,1);

            if(dbg) deb<<"Defaulting daug vars...\n";

            i=i-daugLenght;

            onDaug=false;
            daugLenght=0;
            daugs.clear();
        }

        if(mustEnd) break;
    }
    if(dbg) deb<<"\nDauging ended: vector now:\n";
    Transformer::showElements(elems,1);
}

void Calculator::calculateSpecials(std::vector<CalcElement> &elems)
{
    bool dbg=DEBUG_CALCULATOR_CALCULATESPECIALS;
    if(dbg) deb<<"\n++++++++++++++++\nCalc::calculateSpecials() \nGot vector:";
    Transformer::showElements(elems, 1);

    int specialStartPos=0, specialEndPos=0, multiParamCount=0, specialLenght=0;
    bool onMultiSpecial=false, onSpecial=false, specialReady=false, onOtherSpecial=false;

    CalcElement specel;

    for(int i=0; i<elems.size(); i++)
    {
        if(dbg) deb<<"* * * *\nLoop of i= "<<i<<".\nStates: ";
        if(dbg) deb<<"onMultiSpecial: "<<onMultiSpecial<<", onOtherSpecial: "<<onOtherSpecial<<", onSpecial: "<<onSpecial<<", specialReady: "<<specialReady<<"\n";
        if(dbg) deb<<"specialStartPos= "<<specialStartPos<<", multiParamCount= "<<multiParamCount<<", specialLenght= "<<specialLenght<<"\n~ ~ ~\n";

        if(elems[i].type==OPERATOR){ if(dbg) deb<<"Found: Operator, val= "<<Transformer::codeToOperator(elems[i].oper.operation)<<"\n"; }
        else if(elems[i].type==NUMBER){ if(dbg) deb<<"Found: Number, val= "<<elems[i].number<<"\n"; }
        else if(elems[i].type==CONSTANT){ if(dbg) deb<<"Found: Constant, val= "<<Transformer::codeToOperator(elems[i].number)<<"\n"; }

        if(elems[i].type==OPERATOR && elems[i].oper.operation>=ADVANCED_START && elems[i].oper.operation<NON_CALCULATIVE_START && !onSpecial)
        {
            if(dbg) deb<<"Fount advanced calc. operator: "<<Transformer::codeToOperator(elems[i].oper.operation)<<"\n";
            if(elems[i].oper.operation>=MULTIPARAM_START && elems[i].oper.operation<MULTIPARAM_END)
            {
                if(dbg) deb<<"Operator multiSpecial: true\n";
                onMultiSpecial=true;
            }
            if(elems[i].oper.operation>=OTHERSPECIAL_START && elems[i].oper.operation<OTHERSPECIAL_END)
            {
                if(dbg) deb<<"Operator OtherSpecial: true\n";
                onOtherSpecial=true;
            }

            onSpecial=true;
            specialStartPos=i;
            specialEndPos=0;
            specialLenght=0;
            multiParamCount=0;

            specel.clear();
            specel.oper.operation=elems[i].oper.operation;
        }

        if(onSpecial)
        {
            if(dbg) deb<<"onSpecial: \n";
            bool endOfSpecial=false;

            if(onMultiSpecial && (elems[i].type==NUMBER || elems[i].type==CONSTANT))
            {
                if(dbg) deb<<"onMultiSpecial, number found: "<<elems[i].number<<"\n";
                if(i<elems.size()-1)
                {
                    if(elems[i+1].type==OPERATOR && elems[i+1].oper.operation==SEPAR && multiParamCount<MAX_MULTIPARAMS)
                    {
                        if(dbg) deb<<"Assigning multiparam.\n";
                        specel.oper.param[multiParamCount].val = elems[i].number;
                        if(elems[i].type==CONSTANT) specel.oper.param[multiParamCount].isConst=true;
                        multiParamCount++;
                        specel.oper.paramCount++;
                    }
                    else if(elems[i+1].type==OPERATOR && elems[i+1].oper.operation!=SEPAR) endOfSpecial=true;

                }
                else endOfSpecial=true;
            }
            else if(!onMultiSpecial && (elems[i].type==NUMBER || elems[i].type==CONSTANT)) endOfSpecial=true;

            if(onOtherSpecial)
            {
                if(elems[i].type==OPERATOR && elems[i].oper.operation==POWS && specel.oper.paramCount==0 && multiParamCount==0)
                {
                    if(i>0){
                        if(elems[i-1].type==NUMBER) specel.oper.param[multiParamCount].val = elems[i-1].number;
                        else specel.oper.param[multiParamCount].val = 1;
                    }
                    else specel.oper.param[multiParamCount].val = 1;
                    multiParamCount++;

                    if(i+1 < elems.size())
                    {
                        if(elems[i+1].type==NUMBER) specel.oper.param[multiParamCount].val = elems[i+1].number;
                        else specel.oper.param[multiParamCount].val = 1;
                    }
                    else specel.oper.param[multiParamCount].val = 1;
                    multiParamCount++;
                    specel.oper.paramCount=multiParamCount;

                    specialLenght=1;
                    if(i>0) specialStartPos=i-1;
                    specialEndPos=i+2;

                    endOfSpecial=true;
                }
            }
            //else if(!onOtherSpecial && (elems[i].type==NUMBER || elems[i].type==CONSTANT)) endOfSpecial=true;

            if(endOfSpecial)
            {
                if(dbg) deb<<"End of special reached. Defaulting vars, assigning number and type to struct...\n";
                if(onMultiSpecial) onMultiSpecial=false;
                if(onOtherSpecial) onOtherSpecial=false;
                specel.type = elems[i].type;
                specel.number = elems[i].number;
                onSpecial=false;
                specialReady=true;

                if(specialEndPos<i) specialEndPos=i+1;
            }

            specialLenght++;
        }

        if(specialReady)
        {
            if(dbg) deb<<"Special ready for calculation:\n";
            if(dbg) Transformer::showCalcElementContents(specel);
            if(dbg) deb<<"\nCalc.result= ";
            double speRes = skaicSpecial(specel);
            if(dbg) deb<<speRes<<"\n";

            CalcElement tmp;
            tmp.type=NUMBER;
            tmp.number=speRes;

            if(dbg) deb<<"Erasing used elems in vector, inserting new value...\n";

            elems.erase(elems.begin()+specialStartPos, elems.begin()+specialEndPos);
            elems.insert(elems.begin()+specialStartPos, tmp);

            if(dbg) deb<<"Vector after modding:\n";
            Transformer::showElements(elems, 1);

            i=i-specialLenght;

            specialReady=false;
        }
    }

    if(dbg) deb<<"calculateSpecial() ended. Final vector:\n";
    Transformer::showElements(elems, 1);
    if(dbg) deb<<"++++++++++++++\n";
}

double Calculator::skaicSpecial(CalcElement numop)
{
    bool dbg=DEBUG_CALCULATOR_SKAICSPECIAL;
    if(dbg){ deb<<"\n+-+-+-+\nCalc::skaicSpecial():\nGot CalcElement:\n"; Transformer::showCalcElementContents(numop); }

    double ress=0;

    switch(numop.oper.operation)
    {
    case SIN: ress= sin( Transformer::getValueFromElement(numop) ); break;
    case COS: ress= cos( Transformer::getValueFromElement(numop) ); break;
    case TAN: ress= tan( Transformer::getValueFromElement(numop) ); break;
    case CTAN: ress= 1 / tan( Transformer::getValueFromElement(numop) ); break;
    case ASIN: ress= asin( Transformer::getValueFromElement(numop) ); break;
    case ACOS: ress= acos( Transformer::getValueFromElement(numop) ); break;
    case ATAN: ress= atan( Transformer::getValueFromElement(numop) ); break;
    case ACTAN: ress= atan( 1 / Transformer::getValueFromElement(numop) ); break;
    case LG: ress= log10( Transformer::getValueFromElement(numop) ); break;
    case LN: ress= log( Transformer::getValueFromElement(numop) ); break;
    //multiparams:
    case LOG: ress= log( Transformer::getValueFromElement(numop)) / log(Transformer::getValueFromElement(numop,0) ); break;
    case LAIP: ress= pow( Transformer::getValueFromElement(numop), Transformer::getValueFromElement(numop,0) ); break;
    case SAK:
        {
            if(numop.oper.paramCount==0){ ress= pow( Transformer::getValueFromElement(numop), 0.5 );}
            else{ ress= pow( Transformer::getValueFromElement(numop), 1 / Transformer::getValueFromElement(numop,0) );}
            break;
        }
    //case POWS: ress= pow( Transformer::getValueFromElement(numop,0), Transformer::getValueFromElement(numop,1) ); break;
    case POWS: ress= pow( numop.oper.param[0].val, numop.oper.param[1].val ); break;
    }

    if(dbg) deb<<"Res= "<<ress<<"\n+-+-+-+\n";

    return ress;
}

//end.
