/* Func_020003a8 of overlay 6B9 and Func_02000ae0 of 6BA (the same code but for its callee) match
 * instruction for instruction as below, the form 6B8's Func_02000950 has, but only with its two
 * empty asm barriers. They stay on the scaffold: the barrier count may only fall (S2). Without
 * the barriers the draft builds 0x10000 before it loads the scale, and 0x51e before it loads it
 * again; the native loads first both times. */
#include "types.h"
#include "object_runtime.h"

void Func_020005c8();
struct ObjectRuntime *Object_GetById();
void WaitFrames();

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
        while (object->scale_x < 0x10000) {
            WaitFrames(1);
            scale = object->scale_x;
            scale += 0x51e;
            object->scale_x = scale;
            object->scale_y = scale;
        }
        object->scale_x = 0x10000;
        object->scale_y = 0x10000;
    }
}
