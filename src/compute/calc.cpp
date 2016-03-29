#include <iostream>
#include <cmath>
#include "compute/calc.h"
#include "compute/transform.h"
#include "defines/globaldefines.h"
#include "defines/calcdefines.h"
#include "tools/fun.h"
#include "tools/debug.h"
#include "defines/formal.h" //there debug start info and debugger itself local declared.


Calculator::Calculator()
{
    if(deb.getLastModif().size()==0)
    {
        deb.setLastModifTag("Calculator::()");

        startDebugger_printDebugInfo(deb.getLastModif().c_str());
    }
}

Calculator::Calculator(int mode)
{
    Calculator();

    if(mode==1) advancedMath = true;
}

Calculator::~Calculator()
{
    this->clear(1); //just to use a destructor
}

void Calculator::clear(int mode)
{
    salyga.clear();
    result=0;
    calcErrCode=0;

    if(mode==1)
    {
        lastNum=0;
        advancedMath=false;
        isCalculated=true;
    }
}

void Calculator::sendString(std::string str)
{
    bool dbg=DEBUG_CALCULATOR_SENDSTRING;

    isCalculated=false;
    if(dbg) deb<<"\nNew operation assigned: "<<str<<"\n";

    //correctString(str);
    //if(dbg) deb<<"\nAfter correction: "<<str<<"\n";

    salyga=str;
    int ero = startCalculation();

    if(ero!=0) calcErrCode= ero;
    if(std::isnan(result)) calcErrCode= Result_is_NaN;
    if(std::isinf(result)) calcErrCode= Result_is_INF;

    if(calcErrCode)
    {
        deb<<"Something's wrong with the equation... Correct it, madafaka!";
        deb<<"(error: "<<Transformer::getErrorString((CalcErrorCode)calcErrCode)<<")\n";
    }

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
        else return "Error (not nyanyanya). errCode: "+Transformer::getErrorString((CalcErrorCode)calcErrCode);
    }
    return "Still Calculating!";
}

int Calculator::startCalculation()
{
    bool dbg=DEBUG_CALCULATOR_STARTCALCULATION;
    std::vector<CalcElement> elemen;

    int erval = Transformer::setCalculationElements(salyga, elemen);

    if(erval!=0)
        return erval;

    result = recursiveChunkyCalculation(elemen, 1);

    //if(dbg) deb<<"\nstartCalcultation:\nElems after Everything:\n";
    //Transformer::showElements(elemen, 1);

    /*if(!equational && elemen.size()!=1)
        return CalcErrorCode::Incomplete_Calculation;*/

    lastNum = result;
    return 0;
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

    if(dbg) deb<<"Declared vars.\n";

    //This was used in older version when identifying was based on parenthesis adding
    //if(dbg) deb<<"Identifying negatives... (might be temporary)\n";
    //identifyNegatives(elemso);

    if(dbg) deb<<"Starting loop...\n";

    for(i=0; i<elemso.size(); i++)
    {
        if(dbg) deb<<"\n*****\nLoop no. "<<i<<" \n";
        if(dbg) deb<<"States: areChunksInside: "<<areChunksInside<<", nowWritingIntoChunk: "<<nowWritingIntoChunk<<", areNumsInInnerChunk: "<<areNumsInInnerChunk<<"\n";
        if(dbg) deb<<"par1skInChun="<<par1skInChun<<", par2skInChun="<<par2skInChun<<", innerChunkStartPos="<<innerChunkStartPos<<", innerChunkLenght="<<innerChunkLenght<<"\n~~~\n";

        if(elemso[i].type==CALC_OPERATOR)
        {
            if(dbg) deb<<"Found: operator, val= "<<Transformer::codeToOperator(elemso[i].oper.operation)<<" \n";

            if(!nowWritingIntoChunk) //on main chunk (NOT writing into inner chunk)
            {
                if(elemso[i].oper.operation == CALC_PAR1)
                {
                    if(dbg) deb<<"Par1 found! Starting inner chunk writing. \n";

                    nowWritingIntoChunk=true;
                    areChunksInside=true;
                    innerChunkStartPos=i;
                    insideChunk.clear();

                    if(dbg) deb<<"innerChunkStartPos="<<innerChunkStartPos<<" \n";
                }
                else if(elemso[i].oper.operation == CALC_PAR2)
                {
                    if(dbg) deb<<"Error with parenthesis! (!writingIntoChunk && ')' found)\n";
                    calcErrCode = CalcErrorCode::Bad_Pars;
                    break;
                }

                //assign op to a current vec.
                if(!nowWritingIntoChunk)
                {
                    if(dbg) deb<<"Assigning op to currentChunk... \n";
                    currentChunk.push_back(elemso[i]);

                    if(afterMultiparam &&  elemso[i].oper.operation != CALC_SEPAR) afterMultiparam=false;

                    if(elemso[i].oper.operation >= CALC_MULTIPARAM_START && elemso[i+1].oper.operation != CALC_SEPAR) afterMultiparam=true;
                }
            }
            else //writing into insider chunk.
            {
                if(elemso[i].oper.operation == CALC_PAR1) par1skInChun++;
                if(elemso[i].oper.operation == CALC_PAR2) par2skInChun++;

                if(par2skInChun<=par1skInChun)
                {
                    if(dbg) deb<<"Assigning op to insideChunk... \n";
                    insideChunk.push_back(elemso[i]); //assign op to insideChunk
                    innerChunkLenght++;
                }
            }
        }

        else if(elemso[i].type==CALC_NUMBER)
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

            if(calcErrCode)
            {
                if(dbg) deb<<"Error occured!\n Breaking operation...\n";
                //deb.setWriteDebugs(true);
                return calcErrCode;
            }

            if(dbg) deb<<"\nrecursiveChunkyCalc() returned value: "<<innerChunkResult<<"\nCreating a new element... \n";

            CalcElement cel;
            cel.type=CALC_NUMBER;
            cel.number=innerChunkResult;

            if(dbg) deb<<"Erasing innerChunk elements in main chunk... \n";
            elemso.erase(elemso.begin()+innerChunkStartPos, elemso.begin()+i+1);

            if(dbg) deb<<"Inserting new value in main chunk to position:"<<innerChunkStartPos<<" \n";

            if(afterMultiparam)
            {
                CalcElement sep;
                sep.type=CALC_OPERATOR;
                sep.oper.operation=CALC_SEPAR;

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

    if(calcErrCode)
    {
        if(dbg) deb<<"RecursiveChunkyCalc: Error happened!\n";
        //deb.setWriteDebugs(true);
        return calcErrCode;
    }

    if(dbg) deb<<"\n*****\nLoop end! Final equation:\n";
    Transformer::showElements(elemso, 1);
    if(dbg) deb<<"Returning value from makeCalculationFromChunk()...\n------------------- \n";
    //deb.setWriteDebugs(true);

    return makeCalculationFromChunk(elemso); //get our result;
}

double Calculator::makeCalculationFromChunk(std::vector<CalcElement> chunk)
{
    double calcuResult=(double)chunk.size();

    identifyNegatives(chunk); //identify negs by adding -1* instead of -
    calculateSpecials(chunk); //Exponents
    calculateDaugs(chunk); //Multiplication, Division

    calcuResult = calculateSudDaug(chunk,1); //Addition, Subtraction

    return calcuResult;
}

double Calculator::calculateSudDaug(std::vector<CalcElement> elem, int mode) //works kind of good.
{
    double rezl=0;
    int startPos=-1;
    int lastOp=CALC_NONE;

    for(int i=0; i<elem.size(); i++)
    {
        if(elem[i].type==CALC_NUMBER)
        {
            startPos=i;
            break;
        }
    }

    if(startPos>=0)
    {
        if(elem[startPos].type==CALC_NUMBER) rezl=elem[startPos].number;

        for(int i=startPos+1; i<elem.size(); i++)
        {
            if(elem[i].type==CALC_OPERATOR) lastOp=elem[i].oper.operation;

            if(elem[i].type==CALC_NUMBER)
            {
                if(mode==1)
                {
                    if(lastOp==CALC_PLU) rezl+=elem[i].number;
                    if(lastOp==CALC_MIN) rezl-=elem[i].number;
                }
                if(mode==2)
                {
                    if(lastOp==CALC_DAU) rezl*=elem[i].number;
                    if(lastOp==CALC_DAL) rezl/=elem[i].number;
                }
            }
        }
    }

    return rezl;
}

void Calculator::identifyNegatives(std::vector<CalcElement> &elems) //TODO - optimize.
{
    bool dbg = DEBUG_CALCULATOR_IDENTIFYNEGATIVES;
    if(dbg) deb<<"\n- - - - -\nCalc::identifyNegatives():\nGot vector:\n";
    Transformer::showElements(elems,1);

    CalcElement temp;
    bool needToDoStuff=false;

    for(int i=0; i<elems.size()-1; i++)
    {
        if((elems[i].type==CALC_OPERATOR && elems[i].oper.operation == CALC_MIN))
        {
            if(i==0) needToDoStuff=true;
            else if(elems[i-1].type != CALC_NUMBER && elems[i-1].oper.operation != CALC_PAR2) needToDoStuff=true;
        }

        if(needToDoStuff)
        {
            temp.clear();
            temp.type = CALC_NUMBER;
            temp.number = -1;

            elems.erase(elems.begin()+i);
            elems.insert(elems.begin()+i, temp); //inserted -1 on pos: i
            i++; //now points to elem after (-) (now -1)

            temp.clear();
            temp.type = CALC_OPERATOR;
            temp.oper.operation = CALC_DAU;

            elems.insert(elems.begin()+i, temp); //inserted * on pos: i+1 (now i)

            needToDoStuff=false;
        }
    }

    if(dbg) deb<<"Elems after negative id-cation:\n";
    if(dbg) Transformer::showElements(elems,1);
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

        if(!onDaug && elems[i].type == CALC_OPERATOR && (elems[i].oper.operation==CALC_DAU || elems[i].oper.operation==CALC_DAL))
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

        if( onDaug && ((elems[i].type == CALC_OPERATOR && (elems[i].oper.operation!=CALC_DAU && elems[i].oper.operation!=CALC_DAL)) || i>=(elems.size()-1)) )
        {
            if(i>=(elems.size()-1)) {if(dbg) deb<<"onDaug: End of equation! calculating daug result: ";}
            else {if(dbg) deb<<"onDaug: found not DAU op! calculating daug result: ";}

            if(i>=(elems.size()-1)) mustEnd=true;

            double daures = calculateSudDaug(daugs, 2);

            if(dbg) deb<<daures<<"\nAssigning to vector, erasing dauged members...\n";

            CalcElement ce;
            ce.type=CALC_NUMBER;
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
        if(i<0) i=0;
        if(dbg) deb<<"* * * *\nLoop of i= "<<i<<".\nStates: ";
        if(dbg) deb<<"onMultiSpecial: "<<onMultiSpecial<<", onOtherSpecial: "<<onOtherSpecial<<", onSpecial: "<<onSpecial<<", specialReady: "<<specialReady<<"\n";
        if(dbg) deb<<"specialStartPos= "<<specialStartPos<<", multiParamCount= "<<multiParamCount<<", specialLenght= "<<specialLenght<<"\n~ ~ ~\n";

        if(elems[i].type==CALC_OPERATOR){ if(dbg) deb<<"Found: Operator, val= "<<Transformer::codeToOperator(elems[i].oper.operation)<<"\n"; }
        else if(elems[i].type==CALC_NUMBER){ if(dbg) deb<<"Found: Number, val= "<<elems[i].number<<"\n"; }
        else if(elems[i].type==CALC_CONSTANT){ if(dbg) deb<<"Found: Constant, val= "<<Transformer::codeToOperator(elems[i].number)<<"\n"; }

        if(elems[i].type==CALC_OPERATOR && elems[i].oper.operation>=CALC_ADVANCED_START && elems[i].oper.operation<CALC_NON_CALCULATIVE_START && !onSpecial)
        {
            if(dbg) deb<<"Fount advanced calc. operator: "<<Transformer::codeToOperator(elems[i].oper.operation)<<"\n";
            if(elems[i].oper.operation>=CALC_MULTIPARAM_START && elems[i].oper.operation<CALC_MULTIPARAM_END)
            {
                if(dbg) deb<<"Operator multiSpecial: true\n";
                onMultiSpecial=true;
            }
            if(elems[i].oper.operation>=CALC_OTHERSPECIAL_START && elems[i].oper.operation<CALC_OTHERSPECIAL_END)
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

            if(onMultiSpecial && (elems[i].type==CALC_NUMBER || elems[i].type==CALC_CONSTANT))
            {
                if(dbg) deb<<"onMultiSpecial, number found: "<<elems[i].number<<"\n";
                if(i<elems.size()-1)
                {
                    if(elems[i+1].type==CALC_OPERATOR && elems[i+1].oper.operation==CALC_SEPAR && multiParamCount<CALC_MAX_MULTIPARAMS)
                    {
                        if(dbg) deb<<"Assigning multiparam.\n";
                        specel.oper.param[multiParamCount].val = elems[i].number;
                        if(elems[i].type==CALC_CONSTANT) specel.oper.param[multiParamCount].isConst=true;
                        multiParamCount++;
                        specel.oper.paramCount++;
                    }
                    else if(elems[i+1].type==CALC_OPERATOR && elems[i+1].oper.operation!=CALC_SEPAR) endOfSpecial=true;

                }
                else endOfSpecial=true;
            }
            else if(!onMultiSpecial && (elems[i].type==CALC_NUMBER || elems[i].type==CALC_CONSTANT)) endOfSpecial=true;

            if(onOtherSpecial)
            {
                if(dbg) deb<<"onOtherSpecial: \n";
                if(elems[i].type==CALC_OPERATOR && elems[i].oper.operation==CALC_POWS && specel.oper.paramCount==0 && multiParamCount==0)
                {
                    if(dbg) deb<<"\nPOW CheckBlock:\n";
                    if(i>0){
                        if(elems[i-1].type==CALC_NUMBER)
                        {
                            specel.oper.param[multiParamCount].val = elems[i-1].number;
                            if(dbg) deb<<"Assigning specel["<<multiParamCount<<"].value from The Number Before! ( "<<elems[i-1].number<<" )\n";
                        }
                        else specel.oper.param[multiParamCount].val = 1;
                    }
                    else specel.oper.param[multiParamCount].val = 1;
                    multiParamCount++;

                    if(i+1 < elems.size())
                    {
                        if(elems[i+1].type==CALC_NUMBER)
                        {
                            specel.oper.param[multiParamCount].val = elems[i+1].number;
                            if(dbg) deb<<"Assigning NextNum (param.2)! val = "<< elems[i+1].number <<"\n";
                        }
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
                if(dbg) deb<<"\nEnd of special reached. Defaulting vars, assigning number and type to struct...\n";
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

            if(calcErrCode)
            {
                if(dbg) deb<<"Err happened on skaicSpecial!\n";
                break;
            }

            CalcElement tmp;
            tmp.type=CALC_NUMBER;
            tmp.number=speRes;

            if(dbg) deb<<"Erasing used elems in vector, inserting new value...\n";

            elems.erase(elems.begin()+specialStartPos, elems.begin()+specialEndPos);
            elems.insert(elems.begin()+specialStartPos, tmp);

            if(dbg) deb<<"Vector after modding:\n";
            Transformer::showElements(elems, 1);

            if(dbg) deb<<"i = i("<<i<<") - specialLenght("<<specialLenght<<") + 2\n";
            i = i - specialLenght + 2; //important, +1 because specialLenght comes up 1 higher, and another +1 because we'll check next element

            specialReady=false;
        }
    }

    if(dbg) deb<<"calculateSpecial() ended. Final vector:\n";
    Transformer::showElements(elems, 1);
    if(dbg) deb<<"++++++++++++++\n";
}

double Calculator::skaicSpecial(CalcElement numop)
{
    deb.setWriteDebugs( DEBUG_CALCULATOR_SKAICSPECIAL );

    deb<<"\n+-+-+-+\nCalc::skaicSpecial():\nGot CalcElement:\n";
    Transformer::showCalcElementContents(numop);
    deb<<"\n";

    if( !Transformer::isMultiparamElementGood(numop) )
    {
        deb<<"Error on Multiparams!\n";
        calcErrCode = CalcErrorCode::Bad_Multiparams;
        return calcErrCode;
    }

    double ress=0;

    switch(numop.oper.operation)
    {
    //standart
    case CALC_SIN:   ress= sin( Transformer::getValueFromElement(numop) ); break;
    case CALC_COS:   ress= cos( Transformer::getValueFromElement(numop) ); break;
    case CALC_TAN:   ress= tan( Transformer::getValueFromElement(numop) ); break;
    case CALC_CTAN:  ress= 1 / tan( Transformer::getValueFromElement(numop) ); break;
    case CALC_ASIN:  ress= asin( Transformer::getValueFromElement(numop) ); break;
    case CALC_ACOS:  ress= acos( Transformer::getValueFromElement(numop) ); break;
    case CALC_ATAN:  ress= atan( Transformer::getValueFromElement(numop) ); break;
    case CALC_ACTAN: ress= atan( 1 / Transformer::getValueFromElement(numop) ); break;
    case CALC_LG:    ress= log10( Transformer::getValueFromElement(numop) ); break;
    case CALC_LN:    ress= log( Transformer::getValueFromElement(numop) ); break;

    //multiparams:
    case CALC_LOG: ress= log( Transformer::getValueFromElement(numop)) / log(Transformer::getValueFromElement(numop,0) ); break;
    case CALC_POWFUN: ress= pow( Transformer::getValueFromElement(numop), Transformer::getValueFromElement(numop,0) ); break;
    case CALC_SAK:
        if(numop.oper.paramCount==0){ ress= pow( Transformer::getValueFromElement(numop), 0.5 );}
        else{ ress= pow( Transformer::getValueFromElement(numop), 1 / Transformer::getValueFromElement(numop,0) );}
        break;

    //case POWS: ress= pow( Transformer::getValueFromElement(numop,0), Transformer::getValueFromElement(numop,1) ); break;
    case CALC_POWS: ress= pow( numop.oper.param[0].val, numop.oper.param[1].val ); break;

    }
    deb<<"Res= "<<ress<<"\n+-+-+-+\n";

    deb.setWriteDebugs(true);

    return ress;
}

//end.
