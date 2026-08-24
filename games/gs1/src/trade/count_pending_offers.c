#include "types.h"

u8 *Trade_GetOfferState(s32);

s32 Trade_CountPendingOffers(u8 *counts)
{
    s32 found = 0;
    u8 *base = Trade_GetOfferState(0);
    u8 *entry = base + 8;
    s32 index;

    if (counts != 0) {
        u8 *slot;

        slot = counts + 3;
        *slot = found;
        slot = counts + 2;
        *slot = found;
        slot = counts + 1;
        *slot = found;
        counts[0] = found;
    }
    index = 0;
    if (*((u32 *)(base + 264)) != 0) {
        do {
            if (*(s8 *)(entry + 3) == -1) {
                if (counts != 0) {
                    counts[entry[0]]++;
                }
                found++;
            }
            index++;
            entry += 4;
        } while (index != (s32)*((u32 *)(base + 264)));
    }
    return found;}
