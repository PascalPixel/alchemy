/* Draft, not exact: 3 differing halfwords, 220-byte candidate for the
   220-byte owner (2026-09-23). Residual: one instruction: the reference
   synthesizes the offset 0x20c (movs #131; lsls #2) before the store of 15,
   this candidate after it. */

#include "DMA.H"

u8 *Runtime_AllocateBlock(s32 slot, u32 size);
void Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
void Func_0801cf48(void);

extern u8 Data_02000240[];

/* Allocates the 0x628-byte work block and copies five settings bytes into it,
   each next to its number of choices. */
void OptionMenu_InitializeWork(void)
{
    u8 *work;
    volatile u32 zero;
    u8 setting; /* FAKEMATCH: each byte is read before its destination is formed. */

    work = Runtime_AllocateBlock(20, 0x628);
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x8500018a, (volatile u32 *)0x040000d4);
    setting = Data_02000240[0x205];
    work[0x594] = setting;
    work[0x599] = 24;
    setting = Data_02000240[0x206];
    work[0x595] = setting;
    work[0x59a] = 15;
    setting = Data_02000240[0x20c];
    work[0x596] = setting;
    work[0x59b] = 3;
    setting = Data_02000240[0x20a];
    work[0x597] = setting;
    work[0x59c] = 2;
    setting = Data_02000240[0x22a];
    work[0x598] = setting;
    work[0x59d] = 2;
    Scheduler_AddOrUpdateCallback(Func_0801cf48, 0xc80);
}
