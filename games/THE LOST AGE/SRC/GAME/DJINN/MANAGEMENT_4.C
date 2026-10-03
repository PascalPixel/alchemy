#include "OWNER_STATE.H"
#include "PARTY_STATE.H"

s32 Party_SumDjinnCounts(s32 index)
{
    u16 owners[16];
    s32 result = 0;
    s32 count = Party_ListActiveOwners((s16 *)owners);

    if (result < count) {
        u16 *owner = owners;
        s32 remaining = count;

        do {
            struct OwnerDjinnState *state = Owner_GetState(*owner++);

            if (index == -1) {
                result += state->counts.banks.owned[0];
                result += state->counts.banks.owned[1];
                result += state->counts.banks.owned[2];
                result += state->counts.banks.owned[3];
            } else {
                result += state->counts.banks.owned[index];
            }
            remaining--;
        } while (remaining != 0);
    }
    return result;
}
