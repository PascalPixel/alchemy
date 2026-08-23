#include "owner_state.h"
#include "party_state.h"

u32 Func_080022ec(s32, s32);
u32 Func_08077348(void)
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
        total += ((u8 *)OwnerState_Get(
            Data_02000240.active_owners[i]))[15];
    }
    total = Func_080022ec(total, count);
    return total;
}
