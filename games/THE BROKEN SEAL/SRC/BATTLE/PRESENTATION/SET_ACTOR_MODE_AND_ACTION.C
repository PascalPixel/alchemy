#include "TYPES.H"

u8 *Owner_GetStateFar(s32);
s32 *GetBattleObjectSlot(s32);
void Object_SetMode(s32, s32);
void ObjectDispatch_ApplyValueToChildrenFar(s32, s32);

s32 BattlePres_SetActorModeAndAction(s32 id)
{
    u8 *state;
    s32 value;

    state = Owner_GetStateFar(id);
    value = 1;
    if (*(s16 *)(state + 56) != 0) {
        if (state[316] != 0 || state[315] != 0 || state[325] != 0)
            value = (state[298] != 1) * 4;
    } else {
        s32 changed = state[298] ^ value;
        value = (u32)(-changed | changed) >> 31;
        value = 5 - value;
    }

    Object_SetMode(*GetBattleObjectSlot(id), value);
    ObjectDispatch_ApplyValueToChildrenFar(*GetBattleObjectSlot(id), (id & 3) + 14);
}
