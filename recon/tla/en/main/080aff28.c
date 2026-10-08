#include "types.h"
#include "btlsmn.h"
#include "owner_state.h"

extern u8 Summon_OrderList[29];

s32 Trade_ListFlaggedEntries(u8 *output)
{
    u8 *source = Summon_OrderList;
    u8 *end = Summon_OrderList + 28;
    s32 count = 0;

    do {
        u8 value = *source++;
        if ((((const struct BattleSummonState *)Trade_GetOfferState(0))->available_mask & (1 << value)) != 0) {
            *output++ = value;
            count++;
        }
    } while (source <= end);
    *output = 32;
    return count;
}
