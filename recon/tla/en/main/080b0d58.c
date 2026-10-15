/* Near miss, differ 2 of 85 (ported from The Broken Seal's
   Trade_RemoveOffer): in the loop's second copy of the found branch the
   native stores the lowered count before it sets found to 1 in r2; this draft
   sets r2 first, as both copies do here (the first copy matches). found set
   before the count, a block between them, or count - 1 written out differ
   as much or more; permute (3 min) found nothing. */
#include "types.h"
#include "owner_state.h"

struct TradeOfferList {
    struct TradeOffer offers[72];
    s32 count;
};

struct TradeOfferTable {
    u8 unk00[8];
    struct TradeOfferList list;
};

s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit)
{
    struct TradeOfferList *table;
    s32 found = 0;
    s32 i;

    table = &((struct TradeOfferTable *)Trade_GetOfferState((owner < 0 || owner > 7)))->list;
    for (i = 0; i < table->count; i++) {
        if (index == table->offers[i].element && bit == table->offers[i].djinn) {
            table->count--;
            found = 1;
            break;
        }
    }
    for (; i < table->count; i++)
        table->offers[i] = table->offers[i + 1];
    return found;
}
