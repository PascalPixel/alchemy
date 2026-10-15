#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);

s32 Func_02000038(void)
{
    struct ObjectRuntime *object = Object_GetById(10);

    if ((u32)(object->x - 0x940000) <= 0x80000) {
        s32 z = object->z;

        /* FAKEMATCH: ⚓️ loads z before it builds the near limit; the plain test starts the constant first (one pair of instructions swapped). */
        __asm__ volatile("");
        if (z >= 0x26c0000) {
            if (z <= 0x2740000)
                return 0;
        }
    }
    return 1;
}

s32 Func_0200006c(void)
{
    struct ObjectRuntime *object = Object_GetById(9);

    if ((u32)(object->x - 0x840000) <= 0x80000) {
        s32 z = object->z;

        /* FAKEMATCH: ⚓️ loads z before it builds the near limit; the plain test starts the constant first (one pair of instructions swapped). */
        __asm__ volatile("");
        if (z >= 0x26c0000) {
            if (z <= 0x2740000)
                return 0;
        }
    }
    return 1;
}
