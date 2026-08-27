#include "types.h"

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
