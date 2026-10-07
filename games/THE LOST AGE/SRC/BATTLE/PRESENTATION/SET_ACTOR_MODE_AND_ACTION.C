#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "OWNER_STATE.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"

#define ObjectDispatch_ApplyValueToChildrenFar ((void (*)(s32,s32))ObjectDispatch_ApplyValueToChildrenFar)

s32 *BtlObjGet(s32);

s32 BattlePres_SetActorModeAndAction(s32 id)
{
    struct BtlUnit *state;
    s32 value;

    state = UnitGet(id);
    value = 1;
    if (state->hp != 0) {
        if (state->nemuri != 0 || state->shibire != 0 || state->miugoki != 0)
            value = (state->side != 1) * 4;
    } else {
        s32 changed = state->side ^ value;
        value = (u32)(-changed | changed) >> 31;
        value = 5 - value;
    }

    Object_SetMode(*BtlObjGet(id), value);
    ObjectDispatch_ApplyValueToChildrenFar(*BtlObjGet(id), (id & 3) + 14);
}

