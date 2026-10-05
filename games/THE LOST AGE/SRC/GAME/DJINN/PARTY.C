#include "PROTO.H"
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


void Djinn_ActivateOfferedForParty(void)
{
    u16 owners[16];
    s32 count = Party_ListActiveOwners((s16 *)owners);
    s32 i;
    s32 element;
    s32 djinn;

    BattlePlacement_UpdateTimedEntriesTwentyTimesFar();
    for (i = 0; i < count; i++) {
        for (element = 0; element <= 3; element++) {
            for (djinn = 0; djinn <= 19; djinn++) {
                if (Trade_CanOfferDjinn(owners[i], element, djinn)) {
                    Djinn_Activate(owners[i], element, djinn);
                    Trade_RemoveOffer(owners[i], element, djinn);
                }
            }
        }
    }
}
