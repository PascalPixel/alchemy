#include "types.h"

u8 *Func_08077330(s32);

s32 Func_0807a550(u8 *counts)
{
    s32 found = 0;
    u8 *base = Func_08077330(0);
    u8 *entry = base + 8;
    u32 *limit;
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
    limit = (u32 *)(base + 264);
    index = 0;
    if (*limit != 0) {
        do {
            if (*(s8 *)(entry + 3) == -1) {
                if (counts != 0) {
                    counts[entry[0]]++;
                }
                found++;
            }
            index++;
            entry += 4;
        } while (index != (s32)*limit);
    }
    return found;}
