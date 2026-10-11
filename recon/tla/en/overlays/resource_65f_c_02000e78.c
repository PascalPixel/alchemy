/* Draft of overlay 65f Func_02000e78 and its callers Func_02000eb0 and
 * Func_02000eec. differ 2 each: in _0e78 the listing copies the id (r7 = r0)
 * after the motion flag store, this draft before it; in _0eb0 and _0eec the
 * listing shifts the flag id (lsls r0) after it loads z, this draft before.
 * Func_02000f3c, the same shape with a two-instruction flag id, matches in
 * lift.c. */
#include "ov65f.h"

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

void Func_02000eb0(void)
{
    struct ObjectRuntime *object = Object_GetById(8);
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;

    if (!Flag_IsSet(0x201) && x == 10 && z == 29) {
        Func_02000e78(8, object);
        Flag_Set(0x201);
    }
}

void Func_02000eec(void)
{
    struct ObjectRuntime *object = Object_GetById(9);
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;

    if (!Flag_IsSet(0x202) && x == 15 && z == 28) {
        Func_02000e78(9, object);
        Flag_Set(0x202);
    }
}
