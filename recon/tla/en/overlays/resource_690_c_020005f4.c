/* differ 0 with its tagged FAKEMATCH asm barrier; back on the listing because the device ratchet (tla asm-barrier, baseline 197) may not rise. Re-adopt when a barrier-free form is found or the baseline is raised by decision. */
#include "local.h"

void Func_020005f4(void)
{
    s32 id, x, z;

    Func_02002068();
    Func_02002150(0);
    CommitPos(4, -2, -6);
    x = 14;
    z = 8;
    /* FAKEMATCH: the native loads both distances, then the actor id, then negates. */
    __asm__ volatile("" : "+r"(x), "+r"(z));
    id = 4;
    /* FAKEMATCH: the actor id comes between the distances and their negation. */
    __asm__ volatile("" : "+r"(id));
    ObjectMotion_CommitPositionAndActivate(id, -x, -z);
    Func_0200052c();
}
