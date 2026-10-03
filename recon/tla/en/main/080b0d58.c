/*
 * Earlier private-view trial: 8 bytes differed from +0x3e.
 * EN 2026-10-03: canonical offer fields score 5465, 82 differing instructions;
 * compiled 122 bytes versus native 172. The scan and compaction loops use
 * different registers and the compiler reduces compaction to a counted loop.
 * Links as recon/tla/raw/080b0d58.s.
 */
#include "OWNER_STATE.H"

s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit)
{
    struct TradeOfferState *table;
    s32 found = 0;
    s32 i;

    table = Trade_GetOfferState((u32)owner > 7);
    for (i = 0; i < (s32)table->count; i++) {
        if (index == table->offers[i].element && bit == table->offers[i].djinn) {
            table->count--;
            found = 1;
            break;
        }
    }
    for (; i < (s32)table->count; i++) {
        table->offers[i] = table->offers[i + 1];
    }
    return found;
}
