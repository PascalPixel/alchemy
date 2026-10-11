#include "types.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "call.h"

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
        /* FAKEMATCH: ⚓️ loads the scale before it builds the bound. */
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

void Func_02000418(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct ObjectRuntime *object0;

    object0 = Object_GetById(arg1);
    if (arg0 != 2) goto L_0200845e;
    Call3((void (*)())Func_020005c8, arg1, (arg2 << 16), (arg3 << 16));
    object0->y = 0x200000;
    object0->motion_flags = 3;
    object0->gravity = 0x170a;
    Call1((void (*)())WaitFrames, 50);
L_0200845e:
    return;
}
