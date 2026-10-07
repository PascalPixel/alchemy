#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "GLYPH.H"
#include "RESOURCE.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "SELECT.H"

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

extern u8 Tile_Decompress4bppCode[];
extern u8 Tile_ExpandMaskedCode[];
extern u8 Tile_ExpandOpaqueCode[];
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *glyph, s32 outlined)
{
    void *code;

    {
        u32 size;

        /* FAKEMATCH: retain the size load after saving both arguments. */
        do {
            size = (u32)Tile_Decompress4bppCodeSize;
        } while (0);
        code = MemAlloc(size);
        size >>= 2;
        Dma_Set((const void *)Tile_Decompress4bppCode, code, 0x84000000 | size, REG_DMA3);
    }
    ((void (*)(const void *, u8 *))code)(glyph->encoded, glyph->input);
    MemFree(code);
    if (outlined) {
        u32 size;

        size = (u32)Tile_ExpandMaskedCodeSize;
        code = MemAlloc(size);
        size >>= 2;
        Dma_Set((const void *)Tile_ExpandMaskedCode, code, 0x84000000 | size, REG_DMA3);
    } else {
        u32 size;

        size = (u32)Tile_ExpandOpaqueCodeSize;
        code = MemAlloc(size);
        size >>= 2;
        Dma_Set((const void *)Tile_ExpandOpaqueCode, code, 0x84000000 | size, REG_DMA3);
    }
    ((void (*)(u8 *, u8 *, u32, u32))code)(glyph->input, glyph->tiles, (u16)glyph->width, (u16)glyph->height);
    MemFree(code);
}
