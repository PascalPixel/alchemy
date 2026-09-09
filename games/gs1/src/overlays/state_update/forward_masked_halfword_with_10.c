#include "types.h"

/*
 * resource_3b9 owner at 0x02001c5c, 12 bytes with no literal pool: a
 * single-call trampoline forwarding `arg0' unchanged, `arg1' masked to its
 * low 16 bits, and a fixed third argument.  `arg0' is never freshly loaded
 * before the call, so it is a pass-through parameter.  The callee is not
 * identified beyond call shape.
 */

extern void Func_02004922(s32 arg0, u16 arg1, s32 arg2);

void SceneState_ForwardMaskedHalfwordWith10(s32 arg0, s32 arg1)
{
    Func_02004922(arg0, (u16)arg1, 10);
}
