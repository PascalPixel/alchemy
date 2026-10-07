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
#include "OWNER_STATE.H"


s32 Owner_GetResistanceValue(s32 owner, s32 index)
{
    struct OwnerInventoryState *state = UnitGet(owner);
    s32 values[4];
    s32 result = 0;

    if (index <= 3) {
        Owner_GetDigitValues(state->class_id, (const u8 *)&((struct OwnerDjinnState *)state)->flags.banks.owned, values);
        result = values[index] / 10;
    }
    return result;
}
