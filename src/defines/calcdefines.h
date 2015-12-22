#ifndef CALCDEFINES_H_INCLUDED
#define CALCDEFINES_H_INCLUDED

//---calculation symbols--
#define NONE 0

//simple
#define PLU 1
#define MIN 2
#define DAU 3
#define DAL 4

//advanced
#define SIN 9
#define COS 10
#define TAN 11
#define CTAN 12
#define ASIN 13
#define ACOS 14
#define ATAN 15
#define ACTAN 16
#define LG 17
#define LN 18

//multiparam
#define LOG 30
#define LAIP 31
#define SAK  32

//grouping symbols
#define PAR1 40
#define PAR2 41
#define SEPAR 42

//constants:
#define CON_PI 50
#define CON_E 51

//types
#define NUMBER 70
#define OPERATOR 71
#define CONSTANT 72
#define GROUPING 73
#define TYPE 74
#define UNKNOWN 75

//directive
#define ADVANCED_START 9

#define STANDART_OPERATOR_START 1
#define STANDART_OPERATOR_END 50

#define MULTIPARAM_START 30
#define MULTIPARAM_END 40

#define GROUPING_START 40
#define GROUPING_END 50

#define CONSTANT_START 50
#define CONSTANT_END 60

#define TYPE_START 70
#define TYPE_END 80

#define NON_CALCULATIVE_START 40

//programmic defines
#define MAX_MULTIPARAMS 4

#endif // CALCDEFINES_H_INCLUDED
