#include "compute/datachunk.h"
#include "tools/fun.h"

dataChunk::dataChunk()
{

}

dataChunk::~dataChunk()
{
    numbers.clear();
    operators.clear();
    chunks.clear();
}

void dataChunk::assignString(std::string str)
{
    bool areNums=false;
    bool areParentheses=false;
    bool chunkStart=false;
    bool canAssignNumber=false;

    int parent1sk=0;
    int parent2sk=0;

    int howManyNumsPassed=0;

    std::string chunkStr;

    for(int i=0; i<str.size(); i++)
    {
        double currentNum;
        char currentChar;

        if(str[i]>='0' && str[i]<='9' && (str[i-1]<'0' || str[i-1]>'9') )
        {
            areNums=true;
            howManyNumsPassed++;

            currentNum = Fun::stringToDouble(str, i);
            canAssignNumber=true;
        }

        if(str[i]<'0' || str[i]>'9')
        {
            currentChar = str[i];
        }

        if(str[i]=='(')
        {
            parent1sk++;
            areParentheses=true;
        }

        if(str[i]==')')
        {
            parent2sk++;
        }

        if(parent2sk==parent1sk && chunkStart)
        {
            chunkStart=false;

            dataChunk chnk;

            chnk.assignString(chunkStr);

            chunks.insert(chunks.end(),chnk);
        }

        if((parent1sk > 1 || areNums ) && str[i]=='(')
        {
            chunkStart=true;
        }

        if(chunkStart)
        {
            chunkStr += str[i];
        }
        else
        {
            if(str[i]>='0' && str[i]<='9')
            {
                if(canAssignNumber)
                {
                    numbers.insert(numbers.end(), currentNum);
                    canAssignNumber=false;
                }
            }
            else
            {
                int op=setOperator(str,i);

                if(op == NONE) op= PLU;

                operators.insert(operators.end(), op);
            }
        }
    }
}

int dataChunk::setOperator(std::string str, int pos)
{

}

//end
