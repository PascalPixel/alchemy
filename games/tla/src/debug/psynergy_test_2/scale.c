#include "types.h"
#include "object_runtime.h"

void Func_020005a0();
void Func_020005c8();
struct ObjectRuntime *Object_GetById();
void WaitFrames();

void Func_02000384(s32 mode, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    if (mode == 1)
        Func_020005a0(a, b, c, d, e, f);
}

void Func_020003a8(s32 mode, s32 id, s32 x, s32 z)
{
    struct ObjectRuntime *object;
    s32 scale;

    object = Object_GetById(id);
    if (mode == 1) {
        Func_020005c8(id, x << 16, z << 16);
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
