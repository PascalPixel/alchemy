#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "GLYPH.H"
#include "GRAPHICS.H"

s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2)
{
    GlyphTransfer *work;

    work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
    ItemIcon_Compose(arg0, arg1);
    VramBlock_LoadCached(arg2, 128, work->tiles);
    Runtime_ReleaseHeapBlock(17);
    return 1;
}
