#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "OWNER_STATE.H"
#include "BATTLE_STATUS_ICON.H"

s32 GetBattleObjectSlot(s32);

s32 BattleActor_ResetRuntimeFields(s32 actor)
{
    u8 *state;
    u8 *cursor;
    s32 count;
    u8 zero;

    state = Owner_GetState(actor);
    zero = 0;
    count = 3;
    cursor = state + 303;
    do {
        count--;
        *cursor-- = zero;
    } while (count >= 0);

    state[305] = 0;
    state[306] = 0;
    state[307] = 0;
    state[308] = 0;
    state[309] = 0;
    state[310] = 0;
    state[311] = 0;
    state[312] = 0;
    state[313] = 0;
    state[314] = 0;
    state[315] = 0;
    state[316] = 0;
    state[317] = 0;
    state[318] = 0;
    state[319] = 0;
    state[320] = 0;
    state[321] = 0;
    state[322] = 0;
    state[323] = 0;
    state[324] = 0;
    state[325] = 0;
    state[326] = 0;
    state[327] = 0;
    state[328] = 0;

    Owner_RecalculateStatsFar(actor);
    return BattleUnit_BuildStatusFlags(actor, GetBattleObjectSlot(actor));
}
