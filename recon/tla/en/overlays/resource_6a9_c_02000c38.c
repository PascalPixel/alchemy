/* Draft, differ 24 of 67: the native gives position r6 and the motion flags address r7; this form swaps them, and the party state load comes before the position loads. Tried: a flags pointer local, the position read straight into the call (differ 31), permuter 4 minutes. */
#include "../../../../games/tla/src/field/overlay_6a9/ov6a9.h"

s32 Func_02000c38(struct FieldActor *object, s16 *position)
{

    u8 saved = object->motion_flags;
    s32 x, z;

    if (Func_020010b4(object) == 0) {
        Func_020010dc();
        Func_0200113c(0);
        Func_02001074(object, 6);
        WaitFrames(6);
        Func_02001164(152);
        Func_02001074(object, 7);
        object->speed = 0x30000;
        object->acceleration = 0x20000;
        object->velocity_y = 0x40000;
        object->motion_flags &= 0x7e;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        x = position[1];
        z = position[5];
        ObjectMotion_SetPositionAndCommit(gPartyState.current_owner, x, z);
        Func_02001074(object, 6);
        ObjectDispatch_SetSingleChildFlags(object, 1);
        object->motion_flags = saved;
        Func_020010e4();
        return 1;
    }
    return 0;
}
