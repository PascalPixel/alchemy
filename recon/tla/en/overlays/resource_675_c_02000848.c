/* Near miss, differ 2 of 47: after the priority flag store (strb) the native copies the second argument (mov r1, r8) before the two position loads; this form copies it after them. The same shape as several near misses of this queue: a load right after a store waits one more step in the native. A forced temporary moves the argument out of r8 (differ 12). */
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
s32 Func_0200129c(s32 id, s32 x, s32 z);
s32 Func_02001234(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

static __inline__ void Func_02001234_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline evaluates both stack arguments before either is stored; a direct call stores the first before it loads the second (differ 8). */
    Func_02001234(a, b, c, d, e, f);
}

void Func_02000848(s32 id, s32 a, s32 b, s32 x, s32 z, s32 unused, s32 flag)
{
    struct ObjectRuntime *object;

    if (Object_GetById(id) != 0) {
        ObjectMotion_SetActionVariant(id, 3);
        if (GameFlag_Test(flag))
            Func_0200129c(id, x << 16, z << 16);
        object = Object_GetById(id);
        object->priority_flags |= 2;
        Func_02001234_(a, b, 1, 3, object->x >> 20, (object->z >> 20) - 1);
    }
}
