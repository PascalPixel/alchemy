#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "owner_state.h"

u32 Djinn_IsActive(s32 owner, s32 index, s32 bit)
{
    s32 value =
        ((struct OwnerDjinnState *)UnitGet(owner))->flags.banks.active[index] &
        (1 << bit);

    return (u32)(-value | value) >> 31;
}


s32 Djinn_Activate(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state = UnitGet(owner);
    s32 result = Trade_CanOfferDjinn(owner, index, bit);

    if (result != 0) {
        u32 owned = state->flags.banks.owned[index];
        if (owned & (1 << bit)) {
            state->flags.banks.active[index] |= 1 << bit;
        } else {
            return 0;
        }
        state->counts.banks.active[index]++;
        Owner_RefreshDerivedData(owner);
    }
    return result;
}

u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state = UnitGet(owner);
    u32 present = Djinn_IsActive(owner, index, bit);

    if (present != 0) {
        state->counts.banks.active[index]--;
        state->flags.banks.active[index] &= ~(1 << bit);
        Owner_RefreshDerivedData(owner);
    }
    return present;
}
