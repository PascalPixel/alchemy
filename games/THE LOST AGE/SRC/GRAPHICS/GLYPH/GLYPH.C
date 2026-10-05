#include "GLOBAL.H"
#include "RUNTIME_MEM.H"
#include "GLYPH.H"
#include "RESOURCE.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

extern char ResourceId_Icons;

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 overlay);

void Ui_PrepareTransferFromTableEntry(u32 index)
{
    GlyphTransfer *state = Ram_HeapSlots->glyph_work;

    state->encoded = UiIcon_MiscIconPointers[index];
    state->width = 2;
    state->height = 2;
    UiGlyph_DecodeWithHeapRoutines(state, 0);
}

void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile, s32 palette, s32 reuse)
{
    GlyphTransfer *work;
    u16 *table;
    u8 *entry;
    u32 index;

    work = Runtime_AllocateHeapBlock(68, 0x608);
    table = Resource_GetTableEntry((s32)&ResourceId_Icons);
    if (icon <= 127)
        index = icon;
    else
        index = icon - 112;
    entry = (u8 *)table + table[index];
    work->encoded = entry + 32;
    work->width = 4;
    work->height = 4;
    UiGlyph_DecodeWithHeapRoutines(work, 0);
    if (reuse == 0)
        *slot = Resource_FindFreeEntry();
    *tile = VramBlock_LoadCached(*slot, 0x200, work->tiles);
    Runtime_ReleaseHeapBlock(68);
    Dma_Set(entry, (void *)(0x05000200 + palette * 32), 0x80000010, REG_DMA3);
}

void UiGlyph_ReservedNoOp(void)
{
}
