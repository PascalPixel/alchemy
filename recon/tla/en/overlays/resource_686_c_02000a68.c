/* Near miss: differ 2 of 86, one reordered pair in the wait loop's decrement:
 * the native loads the pooled -0x10000 before it copies the shifted count
 * (adds r3, r7, #0); the draft copies first. With the id pinned to r5 and
 * the object to r6 the registers match (differ 30 plain: the id and the
 * count's shifted copy trade r5 and r7). In sched2 the copy and the pool
 * load tie at priority 3 and the copy has two dependents to the load's one.
 * Tried: a one-pass block around the decrement, frames = (s16)(frames - 1),
 * a for loop (all 2); a 4-minute permute found nothing better. */
#include "../../../../games/tla/src/scene/ankohl_ruins/ankohl_ruins.h"

void Func_02000a68(s32 unused, s32 objectId)
{
    register s32 id asm("r5") = objectId;
    register struct ObjectRuntime *object asm("r6") = Object_GetById(id);
    s16 frames = 60;

    if (object->z >> 20 == 31) {
        object->motionFlags = 3;
        Func_02002e34(object, 3);
        Func_02002eec(204);
        while (frames != 0) {
            WaitFrames(1);
            if (object->velocityY == 0)
                break;
            frames--;
        }
        Func_02002e6c(id, 0, 0);
        if (id == 10)
            GameFlag_SetBit(0x9d2);
        if (id == 11)
            GameFlag_SetBit(0x9d3);
        if (id == 12)
            GameFlag_SetBit(0x9d4);
        if (id == 13)
            GameFlag_SetBit(0x9d5);
        if (id == 14)
            GameFlag_SetBit(0x9d6);
        if (id == 15)
            GameFlag_SetBit(0x9d7);
        if (id == 16)
            GameFlag_SetBit(0x9d8);
    }
}
