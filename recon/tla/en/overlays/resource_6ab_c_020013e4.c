/* Near miss of Func_020013e4 (29 instructions, 4 differ): the native stores y before it starts
 * the constant 0x51e and loads scale_x after the constant first half; the draft starts the
 * constant before the store. sched2 would need the load one priority step higher. */
#include "types.h"
#include "object_runtime.h"

struct RiseTimer {
    u8 unknown_00[0x68];
    s32 frames;
};

void Func_020041ec(struct ObjectRuntime *object);

void Func_020013e4(struct ObjectRuntime *object)
{
    struct RiseTimer *timer = (struct RiseTimer *)object;

    object->y += 0x8000;
    object->scale_x += 0x51e;
    object->scale_y += 0x51e;
    timer->frames--;
    if (timer->frames == 1)
        Object_SetPositionAndResetMotion(object, 0, 0, 0);
    else if (timer->frames == 0)
        Func_020041ec(object);
}
