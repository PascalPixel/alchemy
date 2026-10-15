/* Near miss, differ 2 of 41: the listing loads gSceneState[0] before it builds
 * 0xfff6; this draft builds the constant first. An empty asm barrier on the
 * loaded value moves the copy of the object pointer (adds r6, r0) instead.
 * In sched2 the constant's first move has priority 6 and the load 5 (the
 * three-insn constant outranks the load's latency of 2). Also measured: the
 * test as *state - 10 or a range check, a volatile or const pointer (all
 * differ 2), gSceneState read without the pointer (21), a one-pass block on
 * the load (the copy moves ahead, 4), the call's result in a temporary, a
 * barrier after the call (4), alchemy permute for 4 minutes (2). */
#include "../../../../games/tla/src/scene/islets/islets.h"

void Func_0200253c(void)
{
    struct ObjectRuntime *object;
    u16 *state;
    u32 value;

    if (gInput.held & 0x80) {
        state = gSceneState;
        object = Object_GetById(gPartyState.currentOwner);
        value = *state;
        if ((u16)(value + 0xfff6) > 200) {
            Func_02003e64();
            Func_02003fa4(0);
            object->motionFlags = 0;
            Call3((void (*)())ObjectMotion_SetPositionAndReset, gPartyState.currentOwner, 72, 296);
            object->motionFlags = 3;
            Func_02003e6c();
        }
    }
}
