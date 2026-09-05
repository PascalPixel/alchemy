#include "types.h"

s32 Trade_RemoveOffer(s32 kind, s32 first, s32 second);
s32 Trade_GetOfferState(s32 arg0);

u32 *Trade_AddOffer(u32 kind, u32 first, u32 second)
{
    u8 *state;
    u8 *entries;
    u8 *entry;
    u32 *count_p;
    u32 count;
    u32 offset;

    Trade_RemoveOffer(kind, first, second);
    state = Trade_GetOfferState(kind > 7);
    entries = state + 8;
    count_p = (u32 *)(state + 0x108);
    count = *count_p;
    offset = count * 4;
    entries[offset] = first;
    count++;
    entry = entries + offset;
    entry[1] = second;
    entry[2] = kind;
    entry[3] = 0xFF;
    *count_p = count;
    return count_p;
}
