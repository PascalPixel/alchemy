#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"


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
