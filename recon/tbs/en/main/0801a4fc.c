/* Draft, not exact (2026-09-24): 164 of 164 bytes (the 4-byte "bx lr" at
   0x0801a5a0 is a separate empty function). Residual: the reference copies
   the icon into a scratch register before the >127 adjustment and saves
   the tile pointer argument before the slot pointer. */

#include "DMA.H"

typedef struct {
    u8 pad0[0x400];
    u8 f400;
    u8 pad401[0x600 - 0x401];
    s16 f600;
    s16 f602;
    u8 *f604;
} GlyphTransfer;

GlyphTransfer *Runtime_AllocateHeapBlock(s32 kind, s32 size);
u16 *Resource_GetTableEntry(u32 index);
void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 overlay);
s32 Func_08004080(void);
s32 Func_08003fa4(u32 slot, u32 size, const void *source);
void Runtime_ReleaseHeapBlock(s32 kind);

extern u8 Value_000000f0[];

void Func_0801a4fc(u32 icon, s32 unused, s32 *slot, s32 *tile, s32 palette, s32 reuse)
{
    GlyphTransfer *work;
    u16 *table;
    u8 *entry;
    u32 index;

    work = Runtime_AllocateHeapBlock(17, 0x608);
    table = Resource_GetTableEntry((u32)Value_000000f0);
    index = icon;
    if (icon > 127)
        index -= 112;
    entry = (u8 *)table + table[index];
    work->f604 = entry + 32;
    work->f600 = 4;
    work->f602 = 4;
    UiGlyph_DecodeWithHeapRoutines(work, 0);
    if (reuse == 0)
        *slot = Func_08004080();
    *tile = Func_08003fa4(*slot, 0x200, &work->f400);
    Runtime_ReleaseHeapBlock(17);
    Dma_Set(entry, (void *)(0x05000200 + palette * 32), 0x80000010, (volatile u32 *)0x040000d4);
}
