#include "types.h"

#define BattlePresentation_SetActorModeAndAction Func_080b7aac

u8 *Runtime_GetObject(s32);
s32 *GetBattleObjectSlot(s32);
void Object_SetMode(s32, s32);
void Object_SetAction(s32, s32);

s32 BattlePresentation_SetActorModeAndAction(s32 id)
{
    u8 *state;
    s32 value;

    state = Runtime_GetObject(id);
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
    Object_SetAction(*GetBattleObjectSlot(id), (id & 3) + 14);
}
