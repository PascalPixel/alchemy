#include "PROTO.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "GLYPH.H"


s32 Ui_BuildPatternToSlot(s32 no, s32 unused, s32 slot)
{
    GlyphTransfer *work;

    work = Runtime_AllocateHeapBlock(0x44, sizeof(GlyphTransfer));
    Ui_PrepareTransferFromTableEntry(no);
    VramBlock_LoadCached(slot, 0x80, work->tiles);
    Runtime_ReleaseHeapBlock(0x44);
    return 1;
}
