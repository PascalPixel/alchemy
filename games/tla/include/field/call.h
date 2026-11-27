#ifndef ALCHEMY_CALL_H
#define ALCHEMY_CALL_H

// The call layer of the scene files that have not yet moved to script_inline.h: Call1 to Call11
// call a function with that many arguments, Value0 to Value4 return its int result.

#include "types.h"

static __inline__ void Call1(void (*f)(), Sint32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), Sint32 a0, Sint32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), Sint32 a0, Sint32 a1, Sint32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), Sint32 a0, Sint32 a1, Sint32 a2, Sint32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void
Call6(void (*f)(), Sint32 a0, Sint32 a1, Sint32 a2, Sint32 a3, Sint32 a4, Sint32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void
Call7(void (*f)(), Sint32 a0, Sint32 a1, Sint32 a2, Sint32 a3, Sint32 a4, Sint32 a5, Sint32 a6)
{
    f(a0, a1, a2, a3, a4, a5, a6);
}

static __inline__ void Call11(void   (*f)(),
                              Sint32 a0,
                              Sint32 a1,
                              Sint32 a2,
                              Sint32 a3,
                              Sint32 a4,
                              Sint32 a5,
                              Sint32 a6,
                              Sint32 a7,
                              Sint32 a8,
                              Sint32 a9,
                              Sint32 a10)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

static __inline__ Sint32 Value0(Sint32 (*f)())
{
    return f();
}

static __inline__ Sint32 Value1(Sint32 (*f)(), Sint32 a0)
{
    return f(a0);
}

static __inline__ Sint32 Value2(Sint32 (*f)(), Sint32 a0, Sint32 a1)
{
    return f(a0, a1);
}

static __inline__ Sint32 Value3(Sint32 (*f)(), Sint32 a0, Sint32 a1, Sint32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ Sint32 Value4(Sint32 (*f)(), Sint32 a0, Sint32 a1, Sint32 a2, Sint32 a3)
{
    return f(a0, a1, a2, a3);
}

#endif
