#include "types.h"

struct BattleTransitionEntry {
    u32 value;
    u32 sum;
    u32 field8;
    u32 fieldc;
};

#define BattlePresentation_InitializeTransitionEntries Func_080c0eb8

void BattlePresentation_InitializeTransitionEntries(struct BattleTransitionEntry *entries)
{
    u32 previous = entries[0].value;
    u32 one = 0x10000;
    u32 zero = 0;

    entries[0].value = one;
    entries[0].sum = zero;
    entries[0].field8 = zero;
    entries[0].fieldc = zero;
    entries[1].value = one;
    entries[1].sum = zero;
    entries[1].field8 = zero;
    entries[1].fieldc = zero;
    entries[2].value = one;
    entries[2].sum = zero;
    entries[2].field8 = zero;
    entries[2].fieldc = zero;
    entries[0].sum = previous + entries[0].value;
}
