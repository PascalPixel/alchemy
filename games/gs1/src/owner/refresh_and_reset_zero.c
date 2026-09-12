#include "types.h"
#include "scene.h"
#include "owner_state.h"
#include "party_state.h"

/* owner/refresh_and_reset_zero.c */
void Owner_RefreshAndResetZero(void)
{
    Sys_Check();
    Sys_unk2(0);
}

/* trade/get_offer_state.c */
void *Owner_GetState();

s32 Trade_GetOfferState(s32 arg0)
{
    if (arg0 != 0) {
        return Owner_GetState(0x83);
    }
    return 0x0200024C;
}

/* party/get_average_level.c */
u32 FixedPoint_Ratio(s32, s32);
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
        total += ((u8 *)OwnerState_Get(
            gCell.active_owners[i]))[15];
    }
    total = FixedPoint_Ratio(total, count);
    return total;
}
