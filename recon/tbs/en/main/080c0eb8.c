#include "TYPES.H"

/* Draft: aggregate initialization spills to a 16-byte stack temporary and
   calls memset; copies use three-word transfers plus scalar stores.
   Scalar fields instead fold the reload and never emit the three stmia stores. */

struct BattleTransitionEntry {
    u32 value;
    u32 sum;
    u32 field8;
    u32 fieldc;
};

void BattlePresentation_InitializeTransitionEntries(struct BattleTransitionEntry *entries)
{
    u32 previous = entries[0].value;
    struct BattleTransitionEntry entry = { 0x10000, 0, 0, 0 };

    entries[0] = entry;
    entries[1] = entry;
    entries[2] = entry;
    entries[0].sum = previous + entries[0].value;
}
