#include "optfence.h"
//void __attribute__((noipa,cold,naked)) optfence(void*p,...){}

static int a,b;
OPTFENCE(a,b);
