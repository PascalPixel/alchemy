#include "types.h"

/*
 * resource_3b9 owner at 0x02001c48, 12 bytes plus one alignment halfword: a
 * two-call trampoline forwarding the argument to the first callee with a
 * fixed second argument, then making a second call with a fixed constant.
 * The argument is never freshly loaded before the first call, so it is a
 * pass-through parameter.  Neither callee is identified beyond call shape.
 */

extern void Func_020048f2(s32 no, s32 arg1);
extern void Func_02004840(s32 no);

void FieldScene_CallPairWith10(s32 no)
{
    Func_020048f2(no, 0);
    Func_02004840(10);
}
