#include "types.h"
#include "owner_state.h"
u32 Djinn_IsActive(s32 owner, s32 index, s32 bit);
s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit);
u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit);
s32 Djinn_Activate(s32 owner, s32 index, s32 bit);
s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit);
u32 *Trade_AddOffer(u32 owner, u32 index, u32 bit);

s32 Djinn_Transfer(s32 source, s32 index, s32 bit, s32 target)
{
    struct OwnerTransferState *state = Owner_GetState(source);
    /* Retained across calls for the two accesses to available[index]. */
    s32 avail_off = index * 4 + 0xf8;
    u32 mask = 1U << bit;
    u32 present;

    if ((*(u32 *)((u8 *)state + avail_off) & mask) != 0) {
        present = Djinn_IsActive(source, index, bit);
        if (Djinn_AddToOwner(target, index, bit) == 0) {
            Djinn_Deactivate(source, index, bit);
            *(u32 *)((u8 *)state + avail_off) &= ~mask;
            state->owned_counts[index]--;

            if (present != 0) {
                Djinn_Activate(target, index, bit);
            } else {
                Trade_RemoveOffer(source, index, bit);
                Trade_AddOffer(target, index, bit);
            }
            return 0;
        }
    }
    return -1;
}
