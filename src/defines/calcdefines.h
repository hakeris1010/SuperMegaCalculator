#ifndef CALCDEFINES_H_INCLUDED
#define CALCDEFINES_H_INCLUDED

//---The operators ID defininitons.---
//TODO: Use Enum or something similar.

#define CALC_NONE   0

//simple:
#define CALC_STANDART_OPERATOR_START 1

#define CALC_PLU    1
#define CALC_MIN    2
#define CALC_DAU    3
#define CALC_DAL    4
#define CALC_MOD    5

//advanced:
#define CALC_ADVANCED_START   9

#define CALC_SIN    9
#define CALC_COS    10
#define CALC_TAN    11
#define CALC_CTAN   12
#define CALC_ASIN   13
#define CALC_ACOS   14
#define CALC_ATAN   15
#define CALC_ACTAN  16
#define CALC_LG     17
#define CALC_LN     18

//other specials:
#define CALC_OTHERSPECIAL_START 25

#define CALC_POWS   25

#define CALC_OTHERSPECIAL_END   30

//multiparam:
#define CALC_MULTIPARAM_START   30

#define CALC_LOG      30
#define CALC_POWFUN   31
#define CALC_SAK      32

//equational multiparams
#define CALC_EQUATIONAL_MULTIPARAM_START 35

#define CALC_LONG_SUM     35
#define CALC_LONG_SAND    36

#define CALC_DEF_INTEG    37
#define CALC_DIFF_N       38
#define CALC_LIMIT        39

#define CALC_EQUATIONAL_MULTIPARAM_END 50
#define CALC_MULTIPARAM_END            50

//grouping operators:
#define CALC_NON_CALCULATIVE_START 50
#define CALC_GROUPING_START        50

#define CALC_PAR1   50
#define CALC_PAR2   51
#define CALC_SEPAR  52

#define CALC_GROUPING_END          60
#define CALC_STANDART_OPERATOR_END 60

//constants:
#define CALC_CONSTANT_START   60

#define CALC_CON_PI 60
#define CALC_CON_E  61

#define CALC_CONSTANT_END     80

//equational:
#define CALC_EQUATIONAL_START 80

#define CALC_SIM_VAR      80
#define CALC_VECTOR_START 81
#define CALC_VECTOR_END   82

#define CALC_EQUATIONAL_END   90

//types:
#define CALC_TYPE_START      100

#define CALC_UNKNOWN    100
#define CALC_NUMBER     101
#define CALC_OPERATOR   102
#define CALC_CONSTANT   103
#define CALC_GROUPING   104
#define CALC_TYPE       105
#define CALC_EQUATIONAL 106

#define CALC_TYPE_END        105

//programmic defines:
#define CALC_MAX_MULTIPARAMS   4

#endif // CALCDEFINES_H_INCLUDED
