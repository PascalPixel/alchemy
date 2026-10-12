/* Near miss, differ 6 of 34: the native starts 0x10000 (movs r2, #128) before
   it reads the anchor x and finishes it after (lsls r2, r2, #9), and sets the
   return value before the add; this draft reads x first. A step temporary
   and the y statement before the x statement do not move it. */
#include "types.h"
#include "iwram_call.h"
#include "object_runtime.h"

s32 Math_Sine(s32 angle);

s32 Func_02001064(struct ObjectRuntime *object)
{
    struct ObjectRuntime *anchor = object->linked_object;
    s32 frame;
    s32 scale;

    object->action++;
    frame = object->action;
    if (frame > 31)
        return 0;
    scale = Iwram_MulQ16(Math_Sine(frame << 10), 0x14000);
    object->scale_x = scale;
    object->scale_y = scale;
    object->x = anchor->x;
    object->y += 0x10000;
    object->z = anchor->z;
    return 1;
}
