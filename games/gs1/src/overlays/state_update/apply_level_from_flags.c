#include "types.h"

/*
 * Level selection from scene flags, overlay resource_3b1. Each callee name
 * refers to its own call word rather than to a shared runtime address.
 */

extern u8 Value_0000092b;
extern u8 Value_0000092a;
extern u8 Value_00000929;

s32 Func_02007784();
s32 Func_02007792();
s32 Func_020077a0();
s32 Func_020062b6();

/*
 * Picks a level from the highest flag that is set and applies it. The
 * 72-byte owner at 0x020012dc includes its three pool words, which are the
 * addresses taken as Value_0000092b, Value_0000092a and Value_00000929.
 */
s32 SceneState_ApplyLevelFromFlags(void)
{
    s32 ret = 0;

    if (Func_02007784((s32)&Value_0000092b) != 0) {
        ret = 3;
    } else if (Func_02007792((s32)&Value_0000092a) != 0) {
        ret = 2;
    } else if (Func_020077a0((s32)&Value_00000929) != 0) {
        ret = 1;
    }

    return Func_020062b6(ret, 1);
}
