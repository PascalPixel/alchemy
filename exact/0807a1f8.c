#include "types.h"

struct OwnerTradeState {
    u8 unknown_000[0xf8];
    u32 owned[4];
    u32 pledged[4];
    u8 owned_counts[4];
    u8 offer_counts[4];
};

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

struct OwnerTradeState *Func_08077394(s32 owner);
u8 *Func_08077330(s32 which);

s32 Func_0807a1f8(s32 owner, s32 index, s32 bit)
{
    struct OwnerTradeState *state = Func_08077394(owner);
    struct TradeOfferTable *table;
    s32 i;
    s32 status;

    if (state->owned_counts[index] == 0)
        return 0;
    if (state->offer_counts[index] > 9) {
        state->offer_counts[index] = 10;
        return 0;
    }
    if ((state->owned[index] & (1 << bit)) == 0)
        return 0;
    if ((state->pledged[index] & (1 << bit)) != 0)
        return 0;

    table = (struct TradeOfferTable *)(Func_08077330((u32)owner > 7) + 8);
    for (i = 0; i < table->count; i++) {
        if (index == table->offers[i].index && bit == table->offers[i].bit)
            break;
    }
    if (i == table->count ||
        ((status = (s8)table->offers[i].status) <= 0 && status != -2))
        return 1;
    return 0;
}
