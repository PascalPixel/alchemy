#include "game_flags.h"
#include "owner_state.h"
#include "party_state.h"

struct OwnerState_0807a0f4 {
    u8 padding[280];
    u8 values[4];
};

s32 Djinn_AddToOwner(s32 owner, s32 index, u8 *state);
u32 *Trade_AddOffer(s32 owner, s32 index, u8 *state);

s32 Djinn_AddToLeastLoadedOwner(s32 index, u8 *state)
{
    void *entry = state + index * 20 + 48;
    s32 best_no = 0;
    s32 best_val = 999;
    s32 count;
    s32 result;
    u8 *owners;

    if (GameFlag_Test(entry) != 0)
        return -1;

    result = Party_CountActiveOwners();
    if (best_no < result) {
        s32 off = 252;

        owners = (u8 *)&Data_02000240 + off * 2;
        count = result;
        do {
            u8 *p = OwnerState_Get(*owners);

            if (((struct OwnerState_0807a0f4 *)p)->values[index] <= 9 &&
                (p += 280, 1)) {
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

    Djinn_AddToOwner(best_no, index, state);
    Trade_AddOffer(best_no, index, state);
    GameFlag_Set(entry);
    return best_no;
}
