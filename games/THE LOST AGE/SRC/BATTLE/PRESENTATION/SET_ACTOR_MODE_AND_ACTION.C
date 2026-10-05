#include "OWNER_STATE.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 *GetBattleObjectSlot(s32);
void ObjectDispatch_ApplyValueToChildrenFar(s32, s32);

s32 BattlePres_SetActorModeAndAction(s32 id)
{
    struct BattleUnit *state;
    s32 value;

    state = Owner_GetState(id);
    value = 1;
    if (state->hp != 0) {
        if (state->sleep != 0 || state->stun != 0 || state->cannot_move != 0)
            value = (state->status_12a != 1) * 4;
    } else {
        s32 changed = state->status_12a ^ value;
        value = (u32)(-changed | changed) >> 31;
        value = 5 - value;
    }

    Object_SetMode(*GetBattleObjectSlot(id), value);
    ObjectDispatch_ApplyValueToChildrenFar(*GetBattleObjectSlot(id), (id & 3) + 14);
}

