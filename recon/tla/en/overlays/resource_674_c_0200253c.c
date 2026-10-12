/* Near miss, differ 2 of 41: the listing loads gSceneState[0] before it builds
 * 0xfff6; this draft builds the constant first. An empty asm barrier on the
 * loaded value moves the copy of the object pointer (adds r6, r0) instead. */
#include "../../../../games/tla/src/field/overlay_674/ov674.h"

void Func_0200253c(void)
{
    struct ObjectRuntime *object;
    u16 *state;
    u32 value;

    if (gInput.held & 0x80) {
        state = gSceneState;
        object = Object_GetById(gPartyState.current_owner);
        value = *state;
        if ((u16)(value + 0xfff6) > 200) {
            Func_02003e64();
            Func_02003fa4(0);
            object->motion_flags = 0;
            Call3((void (*)())ObjectMotion_SetPositionAndReset, gPartyState.current_owner, 72, 296);
            object->motion_flags = 3;
            Func_02003e6c();
        }
    }
}
