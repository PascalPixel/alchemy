#include "TYPES.H"
#include "TRANSFORM.H"

struct BattleTransitionEntry {
    u32 value;
    u32 sum;
    u32 field8;
    u32 fieldc;
};

void BattlePresentation_InitializeTransitionEntries(struct BattleTransitionEntry *entries)
{
    u32 previous = entries[0].value;

    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(entries);
    entries[0].sum = previous + entries[0].value;
}
