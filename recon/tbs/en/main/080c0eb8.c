#include "TYPES.H"

/* Draft: scalar fields fold the final reload and emit scalar stores, not the
   three four-word stmia stores. An aggregate initializer spills 16 bytes,
   calls memset, and copies three words plus a scalar in the Thumb block path.
   Retained scalar candidate: 36 bytes, 18 differing halfwords; no pool. */

struct BattleTransitionEntry {
    u32 value;
    u32 sum;
    u32 field8;
    u32 fieldc;
};

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
