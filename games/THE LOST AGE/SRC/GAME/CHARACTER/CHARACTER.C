#include "TYPES.H"
#include "OWNER_STATE.H"
#include "PARTY_STATE.H"


u32 Party_GetAverageLevel(void)
{
    s32 count;
    s32 total;
    s32 i;

    total = 0;
    count = Party_CountActiveOwners();
    if (count == 0) {
        return 0;
    }
    for (i = 0; i < count; i++) {
        total += ((struct BattleUnit *)Owner_GetState(
            gPartyState.active_owners[i]))->level;
    }
    total = total / count;
    return total;
}
