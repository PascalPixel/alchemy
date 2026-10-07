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

s32 BtlObjGet(s32);

s32 BattleActor_ResetRuntimeFields(s32 actor)
{
    u8 *state;
    u8 *cursor;
    s32 count;
    u8 zero;

    state = UnitGet(actor);
    zero = 0;
    count = 3;
    cursor = state + 0x12f;
    do {
        count--;
        *cursor-- = zero;
    } while (count >= 0);

    state[0x131] = 0;
    state[0x132] = 0;
    state[0x133] = 0;
    state[0x134] = 0;
    state[0x135] = 0;
    state[0x136] = 0;
    state[0x137] = 0;
    state[0x138] = 0;
    state[0x139] = 0;
    state[0x13a] = 0;
    state[0x13b] = 0;
    state[0x13c] = 0;
    state[0x13d] = 0;
    state[0x13e] = 0;
    state[0x13f] = 0;
    state[0x140] = 0;
    state[0x141] = 0;
    state[0x142] = 0;
    state[0x143] = 0;
    state[0x144] = 0;
    state[0x145] = 0;
    state[0x146] = 0;
    state[0x147] = 0;
    state[0x148] = 0;

    UnitCalcFar(actor);
    return BattleUnit_BuildStatusFlags(actor, BtlObjGet(actor));
}
