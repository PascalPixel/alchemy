#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"

void BattleFx_BeginCanvasLayer(s32 mode);
void Func_08182898(s32 arg);

void Func_0818284c(s32 arg)
{
    u8 *work = Ram_HeapSlots->battle_fx_work[0];

    BattleFx_BeginCanvasLayer(0);
    {
        /* FAKEMATCH: the native loads the blend word from the pool before it starts the register address; the plain store starts the address first (one reordered pair, differ 2). Only a halfword register loads from the pool and a u16 local is widened, so the value is pinned to the r3 it already takes, as in overlay 696's s43f8.c. */
        register u16 blend __asm__("r3") = 0x3f44;

        /* FAKEMATCH: the barrier of the pin above: it holds r2 until the value is loaded. */
        __asm__("" : "+l"(blend) : : "r2");
        REG_BLDCNT = blend;
    }
    REG_BLDALPHA = 0x1010;
    *(s32 *)(work + 0x77b4) = 24;
    *(s32 *)(work + 0x77b8) = 0;
    Func_08182898(arg);
}
