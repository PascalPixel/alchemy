/* Near miss, differ 2. The first scale test matches with an empty asm barrier after the
 * load. In the loop the native starts the 0xffff limit (movs r2, #255), loads the scale,
 * then shifts and adds; this draft shifts before the load. Measured in the sched2 dump:
 * the load and the shift tie at priority 3 and the scheduler prefers the insn that
 * depends on the last one scheduled; a load-use latency of 3 instead of ARM9TDMI's 2
 * gives the native order in both tests. Tried: while and do forms, an inline accessor,
 * a non-volatile register-use asm (cost 0, lowers the load), a one-pass block (load
 * first), a limit started before a barrier (registers swap). */
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void WaitFrames(s32 frames);

struct ObjectRuntime *Func_080df820(s32 x, s32 y, s32 z, s32 angle)
{
    struct ObjectRuntime *object;
    s32 scale;

    Audio_PlayCue(138);
    object = Object_Spawn(278, x, y, z);
    if (object != 0) {
        object->scale_y = 0x4000;
        object->scale_x = 0x4000;
        object->speed_limit = 0x30000;
        object->acceleration = 0x30000;
        object->action_flags = 0;
        Object_SetMode(object, 1);
        scale = object->scale_x;
        /* FAKEMATCH: the barrier loads the scale before the 0x10000 limit is started; without it the limit is started first (one reordered pair, differ 2). */
        asm volatile("");
        if (scale < 0x10000) {
            do {
                scale += 0x800;
                object->scale_y = scale;
                object->scale_x = scale;
                object->angle += 0x2000;
                WaitFrames(1);
                scale = object->scale_x;
            } while (scale <= 0xffff);
        }
        object->angle = angle;
    }
    return object;
}
