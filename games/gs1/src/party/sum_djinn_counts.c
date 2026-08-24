#include "types.h"
#include "owner_state.h"
#include "party_state.h"

s32 Party_SumDjinnCounts(s32 index)
{
    u16 owners[16];
    s32 result = 0;
    s32 count = Party_ListActiveOwners(owners);

    if (result < count) {
        u16 *owner = owners;
        s32 remaining = count;

        do {
            struct OwnerValueState *state = OwnerState_Get(*owner++);

            if (index == -1) {
                result += state->values[0];
                result += state->values[1];
                result += state->values[2];
                result += state->values[3];
            } else {
                result += state->values[index];
            }
            remaining--;
        } while (remaining != 0);
    }
    return result;
}
