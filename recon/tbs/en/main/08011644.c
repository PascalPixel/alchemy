/* Draft, not exact: 52 differing halfwords, 256-byte candidate for the
   248-byte owner (2026-09-23). Residual: the halfword stores of 0 and 159
   load their constants from the pool (ldrh) where the reference builds them
   with movs, and the linked zero for state[252] is loaded before
   Map_UpdateCurrentTileBlock in the reference. Passing the constants through
   an int shares the zero with the early ldrsh offset and is worse (73). */

#include "DMA.H"

u32 Resource_GetTableEntry(u32 index);
s32 Resource_DecodeByteLz(const void *source, void *destination);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 WaitFrames(s32 frames);
void Map_UpdateCurrentTileBlock(void);
void Scheduler_DisableCallbacks(void *keep);
void Func_0801161c(void);
void Func_0801179c(void);

extern u8 *Data_03001e70;
extern u8 Value_000000d5;
extern u8 Data_00000000[];

#define BG_PALETTE ((s16 *)0x05000000)

/* Decodes the area's palette (keeping the backdrop colour) and its four
   tile banks, then installs the area's frame callback. */
void Map_LoadAreaGraphics(void)
{
    u8 *state;
    u32 *resources;
    u8 *buffer;
    s16 backdrop;

    state = Data_03001e70;
    buffer = (u8 *)0x02010000;
    resources = *(u32 **)(state + 0x11c);
    backdrop = BG_PALETTE[0];
    Resource_DecodeByteLz((const void *)Resource_GetTableEntry(resources[0]), buffer);
    *(s16 *)buffer = backdrop;
    Dma_Set(buffer, BG_PALETTE, 0x84000070, (volatile u32 *)0x040000d4);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[1]), (void *)0x02038000);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[2]), (void *)0x0203a000);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[3]), (void *)0x0203c000);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[4]), (void *)0x0203e000);
    *(void **)0x03001cfc = Func_0801161c;
    *(u16 *)(state + 0x100) = 0;
    *(u16 *)(state + 0x102) = 159;
    WaitFrames(1);
    Resource_DecodeType01((const void *)Resource_GetTableEntry((u32)&Value_000000d5), buffer);
    Map_UpdateCurrentTileBlock();
    state[252] = (u32)Data_00000000;
    Scheduler_DisableCallbacks(Func_0801179c);
    WaitFrames(1);
}
