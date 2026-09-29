#include "TYPES.H"

/* Draft: scalar fields fold the final reload and emit scalar stores, not the
   three four-word stmia stores. An aggregate initializer spills 16 bytes,
   calls memset, and copies three words plus a scalar in the Thumb block path.
   Retained scalar candidate: 36 bytes, 18 differing halfwords; no pool.
   2026-09-29 slice 4: the reference's three stmia r0!, {r1, r2, r3, r4}
   stores with 0x10000 and three zeros are the same fixed-register store as
   080049ac's (and SceneTransform_ResetMatrix's and the rest of that family
   at 08004a94-08004d04): an asm block like the reviewed Dma_Set, which GCC
   2.96 does not emit for structure copies or scalar stores. It needs a
   reviewed store macro. alchemy permute (3 jobs, 10 minutes) moved only
   operand order (2320 to 2310); not kept. */

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
