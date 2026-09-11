#include "types.h"
#include "scene.h"

/* trade/remove_offer.c */
struct TradeOffer {
    u8 index;
    u8 bit;
    u8 unknown_02;
    u8 status;
};

struct TradeOfferTable {
    struct TradeOffer offers[64];
    s32 count;
};

u8 *Trade_GetOfferState(s32 which);

s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit)
{
    struct TradeOfferTable *table;
    s32 found = 0;
    s32 i;

    table = (struct TradeOfferTable *)(Trade_GetOfferState((u32)owner > 7) + 8);
    for (i = 0; i < table->count; i++) {
        if (index == table->offers[i].index && bit == table->offers[i].bit) {
            table->count--;
            found = 1;
            break;
        }
    }
    for (; i < table->count; i++) {
        table->offers[i] = table->offers[i + 1];
    }
    return found;
}

/* trade/add_offer.c */
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
