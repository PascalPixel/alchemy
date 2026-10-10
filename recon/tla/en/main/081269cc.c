/* Pin-free Transform_SetIdentity helper trial: this transition caller is
   16 native instructions, 16 built, 16 common, differ 0 with the helper's
   scratch registers written directly and declared as real clobbers.
   The former free-scratch form differed 20: entries/previous used r3/r5
   instead of native r5/r6, while the row zero moved from r3 to r6.
   Early-clobber and tying that scratch to the input did not improve it.
   Applying the literal-register form to both shared identity helpers fails
   all twelve complete builds: inputs can no longer overlap scratch outputs.
   Splitting pointer transfer from the row stores with an ordinary explicit
   pointer output also fails, with and without early-clobber. The shared
   linked helper retains its bindings until every caller matches. */
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

    CAMELOT_ASM("three stmia with no paired ldmia, the proven Transform_SetIdentity helper")
    __asm__ volatile("mov r0, %0\n\t"
                     "mov r1, #128\n\t"
                     "mov r2, #0\n\t"
                     "mov r3, #0\n\t"
                     "mov r4, #0\n\t"
                     "lsl r1, r1, #9\n\t"
                     "stmia r0!, {r1, r2, r3, r4}\n\t"
                     "stmia r0!, {r1, r2, r3, r4}\n\t"
                     "stmia r0!, {r1, r2, r3, r4}"
        :
        : "r"(entries)
        : "r0", "r1", "r2", "r3", "r4", "memory", "cc");
    entries[0].sum = previous + entries[0].value;
}
