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

// Readies object id: when the flag is set, places it at (x, z) in cells;
// marks it, then calls Func_02001234 with the cell to the left of it.
void Func_020007dc(s32 id, s32 a, s32 b, s32 x, s32 z, s32 unused, s32 flag)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object != NULL) {
        if (GameFlag_Test(flag))
            Func_0200129c(id, x << 16, z << 16);
        ObjectMotion_SetActionVariant(id, 3);
        object->priority_flags |= 2;
        {
            /* FAKEMATCH: the first argument pinned to r0 and taken by an empty asm: ⚓️ moves it into place right after the flag store, before the position loads; plain it moves after them (differ 2). */
            register s32 first asm("r0") = a;

            asm("" : : "l"(first));
            Func_02001234_(first, b, 3, 1, (object->x >> 20) - 1, object->z >> 20);
        }
    }
}

// As Func_020007dc, with the cell above the object.
void Func_02000848(s32 id, s32 a, s32 b_arg, s32 x, s32 z, s32 unused, s32 flag)
{
    struct ObjectRuntime *object;
    /* FAKEMATCH: b pinned to r8, where ⚓️ keeps it; without the pin the copy below takes a's register (differ 5). */
    register s32 b asm("r8") = b_arg;

    if (Object_GetById(id) != 0) {
        ObjectMotion_SetActionVariant(id, 3);
        if (GameFlag_Test(flag))
            Func_0200129c(id, x << 16, z << 16);
        object = Object_GetById(id);
        object->priority_flags |= 2;
        {
            /* FAKEMATCH: the second argument pinned to r1 and taken by an empty asm: ⚓️ moves it into place right after the flag store, before the position loads; plain it moves after them (differ 2). */
            register s32 second asm("r1") = b;

            asm("" : : "l"(second));
            Func_02001234_(a, second, 1, 3, object->x >> 20, (object->z >> 20) - 1);
        }
    }
}
