#ifndef CALCDEFINES_H_INCLUDED
#define CALCDEFINES_H_INCLUDED

//---The operators ID defininitons.---
//TODO: Use Enum or something similar.

#define NONE   0

//simple:
#define STANDART_OPERATOR_START 1

#define PLU    1
#define MIN    2
#define DAU    3
#define DAL    4

//advanced:
#define ADVANCED_START   9

#define SIN    9
#define COS    10
#define TAN    11
#define CTAN   12
#define ASIN   13
#define ACOS   14
#define ATAN   15
#define ACTAN  16
#define LG     17
#define LN     18

//other specials:
#define OTHERSPECIAL_START 25

#define POWS   25

#define OTHERSPECIAL_END   30

//multiparam:
#define MULTIPARAM_START   30

#define LOG    30
#define LAIP   31
#define SAK    32

//equational multiparams
#define EQUATIONAL_MULTIPARAM_START 35

#define LONG_SUM     35
#define LONG_SAND    36

#define DEF_INTEG    37
#define DIFF_N       38
#define LIMIT        39

#define EQUATIONAL_MULTIPARAM_END 50
#define MULTIPARAM_END     50

//grouping operators:
#define NON_CALCULATIVE_START 50
#define GROUPING_START     50

#define PAR1   50
#define PAR2   51
#define SEPAR  52

#define GROUPING_END       60
#define STANDART_OPERATOR_END   60

//constants:
#define CONSTANT_START   60

#define CON_PI 60
#define CON_E  61

#define CONSTANT_END     80

//equational:
#define EQUATIONAL_START 80

#define SIM_VAR      80
#define VECTOR_START 81
#define VECTOR_END   82

#define EQUATIONAL_END   90

//types:
#define TYPE_START      100

#define UNKNOWN    100
#define NUMBER     101
#define OPERATOR   102
#define CONSTANT   103
#define GROUPING   104
#define TYPE       105
#define EQUATIONAL 106

#define TYPE_END        105

//programmic defines:
#define MAX_MULTIPARAMS  4

#endif // CALCDEFINES_H_INCLUDED
