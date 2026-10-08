#include "resource.h"
#include "mem.h"
#include "types.h"
#include "glyph.h"
#include "graphics.h"

s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2)
{
    GlyphTransfer *work;

    work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
    ItemIcon_Compose(arg0, arg1);
    VramBlock_LoadCached(arg2, 0x80, work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
    return 1;
}
