#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "GLYPH.H"

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
