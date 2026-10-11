/* differ 0 with its tagged FAKEMATCH asm barrier; back on the listing because the device ratchet (tla asm-barrier, baseline 197) may not rise. Re-adopt when a barrier-free form is found or the baseline is raised by decision. */
#include "local.h"

void Func_02000cfc(void)
{
    s32 b = 8;
    s32 c = 9;

    /* FAKEMATCH: the native loads the two coordinates before the first argument; a direct call and a forwarding inline load it first. */
    __asm__ volatile("" : "+r"(b), "+r"(c));
    Func_02002178(1, b, c);
    GameFlag_SetBit(0x203);
}
