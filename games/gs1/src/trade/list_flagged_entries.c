#include "types.h"
#include "scene.h"
#include "abi/trade/list_flagged_entries.h"

struct State_0807977c {
    u32 flags;
};

extern u8 gRom[16];
struct State_0807977c *Trade_GetOfferState(s32);

s32 Trade_ListFlaggedEntries(u8 *output)
{
    u8 *source = gRom;
    u8 *end = gRom + 15;
    s32 count = 0;

    do {
        u8 value = *source++;
        if ((Trade_GetOfferState(0)->flags & (1 << value)) != 0) {
            *output++ = value;
            count++;
        }
    } while (source <= end);
    *output = 32;
    return count;
}
