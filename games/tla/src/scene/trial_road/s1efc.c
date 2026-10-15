#include "trial_road.h"

extern s32 gData_02007e24;

void Func_02001dd4(void);

// Unless flag 123 is set, readies object 8: before flag 0x288 it raises it
// 0x88000 with motion 5, after it grounds it with motion 3; then schedules
// Func_02001dd4.
void Func_02001eb0(void)
{
    if (!GameFlag_Test(123))
    {
        struct ObjectRuntime *object = Object_GetById(8);
        s32                   started = GameFlag_Test(0x288);
        u8                   *flags = &object->motionFlags;

        if (!started)
        {
            *flags = 5;
            {
                s32 y = object->y;

                /* FAKEMATCH: y passes through an empty asm, so ⚓️'s order holds: the step's first half, the y load, its second half; plain, y loads first (differ 2). */
                asm("" : "+l"(y));
                object->y = y + 0x88000;
            }
        }
        else
        {
            *flags = 3;
            object->y = 0;
        }
        Scheduler_AddOrUpdateCallback(Func_02001dd4, 0x480);
    }
}

void Func_02001efc(s32 value)
{
    gData_02007e24 = value;
}
