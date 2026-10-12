#include "ov6ae.h"

void Func_02004d14(void)
{
    GameFlag_ClearBit(0x202);
}

void Func_02004d24(s32 id)
{
    u8 *work = Ram_HeapSlots->window_tiles;

    /* FAKEMATCH: ⚓️ loads the window tiles before it starts the held id's offset */
    asm volatile("" : "+l"(work));
    if (((struct Ov6aeParty *)&gPartyState)->held == id)
        work[0x138a] = 1;
}
