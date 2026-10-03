#include "TYPES.H"
#include "OWNER_STATE.H"
#include "PARTY_STATE.H"

s32 GameFlag_Test(s32);
void GameFlag_SetBit(s32 flag);
extern const u16 Djinn_Definitions[];

const u16 *Djinn_GetDefinition(u32 group, u32 index)
{
    s32 entry;

    entry = 0;
    if ((group <= 3U) && (index <= 0x13U)) {
        entry = (group * 0x14) + index;
    }
    return (const u16 *)((u8 *)Djinn_Definitions + entry * 0xC);
}

s32 Djinn_AddToLeastLoadedOwner(s32 index, s32 bit)
{
    s32 flag = bit + index * 20 + 48;
    s32 best_no = 0;
    s32 best_val = 999;
    s32 count;
    s32 result;
    u8 *owners;

    if (GameFlag_Test(flag) != 0)
        return -1;

    result = Party_CountActiveOwners();
    if (best_no < result) {
        struct PartyState *party = &gPartyState;

        owners = party->active_owners;
        count = result;
        do {
            struct OwnerDjinnState *state = Owner_GetState(*owners);

            if (state->counts.banks.owned[index] <= 9) {
                u8 *p = state->counts.banks.owned;
                s32 value = 0;
                s32 i = 3;

                do {
                    u8 byte = *p;
                    p++;
                    value += byte;
                    i--;
                } while (i >= 0);

                if (best_val > value) {
                    best_val = value;
                    best_no = *owners;
                }
            }
            count--;
            owners++;
        } while (count != 0);
    }

    if (best_val == 999)
        return -2;

    Djinn_AddToOwner(best_no, index, bit);
    Trade_AddOffer((u32)best_no, (u32)index, (u32)bit);
    GameFlag_SetBit(flag);
    return best_no;
}

/* ☀️'s; ⚓️ reads the owned word into a temporary before testing it. */
s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state = Owner_GetState(owner);
    u32 owned;

    if (state->counts.slots[index] > 9)
        return -1;
    owned = state->flags.slots[index];
    if ((owned & (1 << bit)) != 0)
        return -1;
    state->counts.slots[index]++;
    state->flags.slots[index] |= 1 << bit;
    return 0;
}
