/*
 * Draft: Trade_RemoveOffer, ☀️'s list-pointer form. 4 instructions differ:
 * the native fills two load delay slots (mov r6 after the ldrb, adds r4
 * after the compaction load). With -mtune=arm9tdmi (load scheduling) 2.
 * Links as recon/tla/raw/080b0d58.s.
 */
#include "OWNER_STATE.H"

struct TradeOfferList {
    struct TradeOffer offers[72];
    s32 count;
};

s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit)
{
    struct TradeOfferList *table;
    s32 found = 0;
    s32 i;

    table = (struct TradeOfferList *)Trade_GetOfferState((u32)owner > 7)->offers;
    for (i = 0; i < table->count; i++) {
        if (index == table->offers[i].element && bit == table->offers[i].djinn) {
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
