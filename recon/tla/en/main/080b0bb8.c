#include "owner_state.h"

s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state = UnitGet(owner);
    struct TradeOfferState *table;
    s32 i;
    s32 status;
    u32 owned;
    u32 active;

    if (state->counts.banks.owned[index] == 0)
        return 0;
    if (state->counts.banks.active[index] > 9) {
        state->counts.banks.active[index] = 10;
        return 0;
    }
    owned = state->flags.banks.owned[index];
    if ((owned & (1 << bit)) == 0)
        return 0;
    active = state->flags.banks.active[index];
    if ((active & (1 << bit)) != 0)
        return 0;
    table = Trade_GetOfferState((u32)owner > 7);
    for (i = 0; i < (s32)table->count; i++) {
        if (index == table->offers[i].element && bit == table->offers[i].djinn)
            break;
    }
    if (i == (s32)table->count || ((status = table->offers[i].status) <= 0 && status != -2))
        return 1;
    return 0;
}
