#ifndef OPTFENCE
//+doc prevent gcc to optimize away registers and variables
// the macro OPTFENCE(...) can be invoked with any parameter.
// The parameters will get calculated, even if gcc doesn't recognize
// the use of the parameters, e.g. cause they are needed for an inlined asm syscall.
//
// The macro translates to an asm jmp and a function call to the function 
// opt_fence, which is defined with the attribute "noipa" -
// (the compiler "forgets" the function body, so gcc is forced
// to generate all arguments for the function)
// The generated asm jump hops over the call to the function,
// but this gcc doesn't recognize.
//
// This generates some overhead, 
// (a few (never reached) bytes for setting up the function call, and the jmp)
// but I didn't find any other solution,
// which gcc wouldn't cut for optimizations from time to time.
// (volatile, volatile asm, optimize attributes, 
// andsoon have all shown up to be unreliable - sometimes(!)).
//
// Had some fun debugging these bugs, which naturally showed up only sometimes.
// (Many syscalls also work with scrambled arguments..)
// And, I believe it IS a compiler bug. 
// Volatile should be volatile for sure, not only sometimes.
// I mean, why the heck do I write volatile?? 
//
// (The needed function is given below)
// static void __attribute__((noipa,cold,naked)) opt_fence(void*p,...){}
#define _optjmp(a,b) asm( a "OPTFENCE_"#b )
#define _optlabel(a) asm( "OPTFENCE_" #a ":" )
#define __optfence(a,...) _optjmp("jmp ", a ); opt_fence(__VA_ARGS__); _optlabel(a)
#define OPTFENCE(...) __optfence(__COUNTER__,__VA_ARGS__)
#endif

