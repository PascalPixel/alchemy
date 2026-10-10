/* Pin-free Transform_SetIdentity helper trial in its TLA transition caller:
   16 native instructions, 16 built, 6 common, differ 20. With one unbound
   low-register scratch, entries stays in r3 and the row zero moves to r6;
   the native keeps entries/previous in r5/r6 and the row zero in r3.
   Early-clobber does not change this residual. Tying matrix to the scratch
   and leaving them untied both changed the full twelve-edition builds.
   The linked helper keeps its four bindings until its callers all match. */
#include "types.h"

struct BattleTransitionEntry {
    u32 value;
    u32 sum;
    u32 field8;
    u32 fieldc;
};

void BattlePresentation_InitializeTransitionEntries(struct BattleTransitionEntry *entries)
{
    u32 previous = entries[0].value;
    u32 zero;

    CAMELOT_ASM("three stmia with no paired ldmia, the proven Transform_SetIdentity helper")
    __asm__ volatile("mov r0, %1\n\t"
                     "mov r1, #128\n\t"
                     "mov r2, #0\n\t"
                     "mov %0, #0\n\t"
                     "mov r4, #0\n\t"
                     "lsl r1, r1, #9\n\t"
                     "stmia r0!, {r1, r2, %0, r4}\n\t"
                     "stmia r0!, {r1, r2, %0, r4}\n\t"
                     "stmia r0!, {r1, r2, %0, r4}"
        : "=&l"(zero)
        : "r"(entries)
        : "r0", "r1", "r2", "r4", "memory", "cc");
    entries[0].sum = previous + entries[0].value;
}
