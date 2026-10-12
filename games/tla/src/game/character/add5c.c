#include "owner_state.h"
#include "party_state.h"

s32 UnitEpAdd();

static __inline__ s32 Owner_GetEpRatio(s32 owner)
{
    struct BtlUnit *unit = UnitGet(owner);

    if (unit->max_ep <= 0)
        return 0;
    return (unit->ep << 16) / unit->max_ep;
}

s32 Func_080adc90(void)
{
    s32 ep = 0;
    s32 max = 0;
    s32 count = Party_CountActiveOwners();
    s32 i;

    if (count == 0)
        return 0;
    for (i = 0; i < count; i++) {
        struct BtlUnit *unit = UnitGet(gPartyState.active_owners[i]);

        ep += unit->ep;
        max += unit->max_ep;
    }
    if (max == 0)
        return 0;
    return (ep << 16) / max;
}

s32 Func_080adcec(void)
{
    s32 best = 0;
    s32 best_owner = -1;
    s32 count = Party_CountActiveOwners();
    s32 i;

    if (count == 0)
        return 0;
    for (i = 0; i < count; i++) {
        s32 ratio = Owner_GetEpRatio(gPartyState.active_owners[i]);

        if (ratio > best) {
            best = ratio;
            best_owner = gPartyState.active_owners[i];
        }
    }
    if (best_owner == -1)
        return 0;
    return UnitEpAdd(best_owner, -1);
}

void Func_080add5c(s32 count)
{
    s32 i;

    for (i = 0; i < count; i++)
        Func_080adcec();
    Func_080adc90();
}
