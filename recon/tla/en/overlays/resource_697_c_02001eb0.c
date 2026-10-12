/* Draft, differ 2 of 32: the native builds 0x88000 (movs r2, #136) before it loads y; this form loads y first, as Func_0200185c does with its constant. */
#include "../../../../games/tla/src/field/overlay_697/ov697.h"

void Func_02001dd4(void);

void Func_02001eb0(void)
{
    if (!GameFlag_Test(123)) {
        struct ObjectRuntime *object = Object_GetById(8);
        s32 started = GameFlag_Test(0x288);
        u8 *flags = &object->motion_flags;

        if (!started) {
            *flags = 5;
            object->y += 0x88000;
        } else {
            *flags = 3;
            object->y = 0;
        }
        Scheduler_AddOrUpdateCallback(Func_02001dd4, 0x480);
    }
}
