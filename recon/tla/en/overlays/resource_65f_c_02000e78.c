/* Draft of overlay 65f Func_02000e78. differ 2: the listing copies the id
 * (adds r7, r0) after the motion flag store (strb), this draft before it.
 * Measured: a volatile barrier after the store, with or without the id as an
 * in-out operand, and a second variable assigned after the barrier all leave
 * the copy at the entry (the two pseudos are tied to r7). Its callers
 * Func_02000eb0 and Func_02000eec match in lift.c behind a barrier. Also
 * tried: a one-pass block around the store or around a copy of the id (2),
 * a goto loop (6). */
#include "../../../../games/tla/src/field/overlay_65f/ov65f.h"

void Func_02000e78(s32 id, struct ObjectRuntime *object)
{
    s32 i;

    object->motion_flags = 0;
    for (i = 16; i > 0; i--) {
        object->y -= 0x20000;
        Battle_WaitMode0(1);
    }
    Func_02005394(136);
    Func_020012a8(id);
}
