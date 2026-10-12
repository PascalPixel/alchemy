/* Near miss, differ 4 of 36: the native starts 0x10000 (movs r1, #128) before
   it reads the anchor x and finishes it after; this draft reads x first. A
   step temporary and the y statement before the x statement do not move it. */
#include "types.h"
#include "object_runtime.h"

s32 Math_Sine(s32 angle);
void Func_02001fd4(struct ObjectRuntime *object);

void Func_02001a34(struct ObjectRuntime *object)
{
    struct ObjectRuntime *anchor = object->linked_object;
    s32 frame;
    s32 scale;

    object->action++;
    frame = object->action;
    if (frame > 31) {
        Func_02001fd4(object);
        return;
    }
    scale = Math_Sine(frame << 10);
    object->scale_x = scale;
    object->scale_y = scale;
    object->x = anchor->x;
    object->y += 0x10000;
    object->z = anchor->z + (0x10000 - scale) * 5 + 0x80000;
}
