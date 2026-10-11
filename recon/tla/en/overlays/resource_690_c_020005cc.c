/* differ 0 with its tagged FAKEMATCH asm barrier; back on the listing because the device ratchet (tla asm-barrier, baseline 197) may not rise. Re-adopt when a barrier-free form is found or the baseline is raised by decision. */
#include "local.h"

void Func_020005cc(void)
{
    s32 id;

    Func_02002068();
    Func_02002150(0);
    CommitPos(4, 0, -14);
    id = 4;
    /* FAKEMATCH: the native loads the actor id before the other arguments. */
    __asm__ volatile("" : "+r"(id));
    ObjectMotion_CommitPositionAndActivate(id, 16, 0);
    Func_0200052c();
}
