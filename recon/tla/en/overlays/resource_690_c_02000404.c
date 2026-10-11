/* differ 0 with its tagged FAKEMATCH asm barrier; back on the listing because the device ratchet (tla asm-barrier, baseline 197) may not rise. Re-adopt when a barrier-free form is found or the baseline is raised by decision. */
#include "local.h"

void Func_02000404(void)
{
    s32 mode;

    Func_02002068();
    Func_02002150(0);
    Func_020020c0(11, 0, 0);
    GameFlag_SetBit(0xf5d);
    Func_02002130(193, 3);
    mode = 0;
    /* FAKEMATCH: the native loads the mode before the item; direct calls and forwarding inlines load the item first. */
    __asm__ volatile("" : "+r"(mode));
    PartyInventory_GiveItem(193, mode);
    Func_02002070();
}
