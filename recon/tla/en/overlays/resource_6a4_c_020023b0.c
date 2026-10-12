/* Near miss: differ 2 of 56 (one reordered row): the native copies the object
 * to r10 after it loads x and z; the draft copies it first. */
#include "../../../../games/tla/src/field/overlay_6a4/ov6a4.h"

void Func_020023b0(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 x = object->x;
    s32 z = object->z;

    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x40000, 0x20000);
    x >>= 20;
    z >>= 20;
    ObjectMotion_SetPositionAndCommit(gPartyState.current_owner, (x << 4) + 8, (z << 4) + 8);
    WaitFrames(1);
    Object_SetPositionAndResetMotion(object, (x << 20) + 0x80000, object->y, (z << 20) + 0x80000);
    Battle_WaitMode0(4);
    Func_02002af8()->y -= 0x80000;
}
