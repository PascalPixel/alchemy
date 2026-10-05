#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "GLYPH.H"

void Ui_PrepareTransferFromTableEntry(u32 index);

/* ☀️'s: decodes a table pattern into the glyph transfer work and loads its
   tiles into a VRAM slot; ⚓️ passes the heap slot's byte offset. */
s32 Ui_BuildPatternToSlot(s32 no, s32 unused, s32 slot)
{
    GlyphTransfer *work;

    work = Runtime_AllocateHeapBlock(0x44, sizeof(GlyphTransfer));
    Ui_PrepareTransferFromTableEntry(no);
    VramBlock_LoadCached(slot, 0x80, work->tiles);
    Runtime_ReleaseHeapBlock(0x44);
    return 1;
}
