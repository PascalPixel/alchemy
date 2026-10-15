#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "mem.h"

struct FxRunWork {
    u8 unknown_0000[0x78e4];
    s32 active;
    u8 unknown_78e8[8];
    s32 *script;
};

extern void (*Data_08197aa0[])(s32 *script);
extern u8 gMapCellBuffer[];

// Runs one battle effect script: takes the effect's three work blocks, calls
// the handler its first word selects, then gives the blocks back.
void Func_0814cb60(s32 *script)
{
    struct FxRunWork *work;

    Runtime_AllocateHeapBlock(100, 0x302);
    Runtime_AllocateHeapBlock(92, 0x7b7c);
    Runtime_AllocateHeapBlock(96, 0x4000);
    /* FAKEMATCH: the one-pass block loads the work pointer before the 0x78f0 offset is started (differ 2 without it). */
    do
    {
        work = (struct FxRunWork *)Ram_HeapSlots->battle_fx_work[0];
    } while (0);
    work->script = script;
    work->active = 1;
    /* FAKEMATCH: the one-pass block loads the handler table before the script's first word and sets the argument after the index (differ 2 without it). */
    do
    {
        Data_08197aa0[*script - 1](script);
    } while (0);
    Runtime_ReleaseHeapBlock(96);
    Runtime_ReleaseHeapBlock(92);
    Runtime_ReleaseHeapBlock(100);
}

void BattleFx_BuildWindowEdgeTable(void)
{
    u16 *edge;
    u16 *line;
    u8 *inset;
    s32 i;
    s32 right;

    edge = (u16 *)gMapCellBuffer;
    line = (u16 *)(Ram_MapCellBuffer + 0x82);
    inset = Ram_MapCellBuffer + 2;
    for (i = 0; i != 160; i++) {
        if ((u32)(i - 8) <= 127) {
            right = *edge - inset[i - 8];
            if (right < 0)
                right = 0;
            if (right > 240)
                right = 240;
            line[i] = right;
        } else {
            line[i] = 0xfff1;
        }
    }
    {
        volatile u16 *channel = (volatile u16 *)0x040000b0;
        u16 control;

        /* FAKEMATCH: the one-pass blocks read the control word before the mask is built and store it before the next mask (differ 4 without them). */
        do {
            control = channel[5];
        } while (0);
        channel[5] = control & 0xc5ff;
        channel[5] &= 0x7fff;
        /* FAKEMATCH: the native stores the control word before it starts the window address (differ 2 without the barrier). */
        __asm__("");
        (void)channel[5];
        Dma_Set(gMapCellBuffer + 0x82, (void *)0x04000040, 0xa2600001, (volatile u32 *)channel);
    }
}
