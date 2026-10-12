#include "types.h"
#include "object_runtime.h"
#include "call.h"

void Func_02000df8();
struct ObjectRuntime *Object_GetById();
void WaitFrames();

void Func_02000ae0(s32 mode, s32 id, s32 x, s32 z)
{
    struct ObjectRuntime *object;
    s32 scale;

    object = Object_GetById(id);
    if (mode == 1) {
        Func_02000df8(id, x << 16, z << 16);
        object->scale_x = 0;
        object->scale_y = 0;
    }
    if (mode == 2) {
        /* FAKEMATCH: ⚓️ loads the scale before it builds the bound; without the barrier the bound's first byte comes first. */
        __asm__ volatile("" : "+l"(object));
        while (object->scale_x < 0x10000) {
            WaitFrames(1);
            scale = object->scale_x;
            /* FAKEMATCH: and loads it again before it builds the step. */
            __asm__ volatile("" : "+l"(scale));
            scale += 0x51e;
            object->scale_x = scale;
            object->scale_y = scale;
        }
        object->scale_x = 0x10000;
        object->scale_y = 0x10000;
    }
}

void Func_02000b50(s32 mode, s32 id, s32 x, s32 z)
{
    struct ObjectRuntime *object;

    object = Object_GetById(id);
    if (mode == 2) {
        Call3((void (*)())Func_02000df8, id, x << 16, z << 16);
        object->y = 0x200000;
        object->motion_flags = 3;
        object->gravity = 0x170a;
        Call1((void (*)())WaitFrames, 50);
    }
}
