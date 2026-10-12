/* Near misses of Func_020007dc and Func_02000848 of overlay 675 (48 and 47 instructions,
 * 2 differ each): the native moves the argument it keeps in r8 into place before it loads
 * the position, the draft after. Measured: direct calls 8 differ each; a one-pass block before
 * the call the same 2 and 6; a "+l" barrier on that argument takes other registers (13, 12). */
#include "types.h"
#include "object_runtime.h"
#include "call.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_0200129c(s32 id, s32 x, s32 z);
void Func_02001234(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);

void Func_020007dc(s32 id, s32 a, s32 b, s32 x, s32 z, s32 unused, s32 flag)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object != NULL) {
        if (GameFlag_Test(flag))
            Func_0200129c(id, x << 16, z << 16);
        ObjectMotion_SetActionVariant(id, 3);
        object->priority_flags |= 2;
        Call6((void (*)())Func_02001234, a, b, 3, 1, (object->x >> 20) - 1, object->z >> 20);
    }
}

void Func_02000848(s32 id, s32 a, s32 b, s32 x, s32 z, s32 unused, s32 flag)
{
    struct ObjectRuntime *object;

    if (Object_GetById(id) != NULL) {
        ObjectMotion_SetActionVariant(id, 3);
        if (GameFlag_Test(flag))
            Func_0200129c(id, x << 16, z << 16);
        object = Object_GetById(id);
        object->priority_flags |= 2;
        Call6((void (*)())Func_02001234, a, b, 1, 3, object->x >> 20, (object->z >> 20) - 1);
    }
}
