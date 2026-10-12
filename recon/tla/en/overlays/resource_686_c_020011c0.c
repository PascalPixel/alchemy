/* Near miss: differ 4 of 54 (two reordered rows): at the offset call and at
 * Func_02002e94 the native builds the second argument before it loads the
 * owner; the draft loads the owner first. Tried: both calls through inline
 * forwarders (same difference). */
#include "../../../../games/tla/src/field/overlay_686/ov686.h"

void Func_020011c0(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);

    Func_02002e44();
    Func_02002edc(0);
    Object_SetModeById(gPartyState.current_owner, 0);
    Func_02002e9c(gPartyState.current_owner, 258);
    object->motion_flags = 0;
    object->action_flags = 0;
    ObjectMotion_OffsetPositionAndResetMotion(gPartyState.current_owner, 0, 8);
    ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.current_owner);
    Func_02002e94(gPartyState.current_owner, 256, 0);
    Battle_WaitMode0(10);
    Ov686_CamBounds(-1, -1, -1, 0);
    Func_02002eec(204);
    Func_02000b30(gPartyState.current_owner, 188);
    object->action_flags = 1;
    Func_02002e4c();
}
