/*
 * Draft of Func_0811d79c. 14 instructions differ: the listing moves the
 * argument to r3 and builds the default 0 in r0 before the range test; with
 * returns in the cases the draft tests r0 and builds the 0 last, and with a
 * result variable (= 0 before the switch) the result lives in r3 (22).
 */
#include "types.h"

s32 Func_0811d79c(u32 kind)
{
    switch (kind) {
    case 0:
    case 1:
    case 4:
    case 5:
    case 7:
        return 0x4001;
    case 2:
    case 3:
    case 6:
        return 0x4004;
    }
    return 0;
}
