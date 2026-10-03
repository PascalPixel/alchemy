#include "TYPES.H"
#include "GLYPH.H"

s32 Runtime_ReleaseHeapBlock(s32);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Ui_PrepareTransferFromTableEntry(u32 index);

s32 Ui_BuildPatternToSlot(s32 icon, s32 unused, s32 slot)
{
    GlyphTransfer *work;

    work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
    Ui_PrepareTransferFromTableEntry(icon);
    VramBlock_LoadCached(slot, 0x80, work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
    return 1;
}
