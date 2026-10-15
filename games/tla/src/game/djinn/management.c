#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"
#include "party_state.h"

s32 GameFlag_Test(s32);
void GameFlag_SetBit(s32 flag);

const u16 *Djinn_GetDefinition(u32 group, u32 index)
{
    s32 entry;

    entry = 0;
    if (group <= 3 && index <= 19) {
        entry = group * 20 + index;
    }
    return gDjinn_Definitions[entry];
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

        owners = party->activeOwners;
        count = result;
        do {
            struct OwnerDjinnState *state = UnitGet(*owners);

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
    Trade_AddOffer(best_no, index, bit);
    GameFlag_SetBit(flag);
    return best_no;
}

s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state = UnitGet(owner);
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
