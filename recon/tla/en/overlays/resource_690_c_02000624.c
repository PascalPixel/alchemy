/* differ 0 with its tagged FAKEMATCH asm barrier; back on the listing because the device ratchet (tla asm-barrier, baseline 197) may not rise. Re-adopt when a barrier-free form is found or the baseline is raised by decision. */
#include "local.h"

void Func_02000624(void)
{
    s32 id, a;

    Func_02002068();
    Func_02002150(0);
    a = 192;
    /* FAKEMATCH: the native loads the angle's byte, then the actor id, then shifts. */
    __asm__ volatile("" : "+r"(a));
    id = 4;
    /* FAKEMATCH: the actor id comes between the angle byte and its shift. */
    __asm__ volatile("" : "+r"(id));
    ObjectMotion_ArmCallback(id, a << 8, 0);
    Func_0200052c();
}
