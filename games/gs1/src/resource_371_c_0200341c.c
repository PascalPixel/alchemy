#include "types.h"

/* Complete four-slot opposing-pair total helper. */
extern s32 Func_0200686a(s32 slot);
extern s32 Func_02006872(s32 slot);
extern s32 Func_0200687a(s32 slot);
extern s32 Func_02006882(s32 slot);

s32 Func_0200341c(void)
{
    s32 plus = Func_0200686a(0);
    s32 minus;

    plus += Func_02006872(2);
    minus = Func_0200687a(1);
    minus += Func_02006882(3);
    return plus - minus;
}
