#include "mem.h"
#include "types.h"
#include "scene.h"
#include "resource.h"

extern u8 gRomBytes_080308a0[];

typedef struct {
    u8 pad0[0x400];
    u8 f400;
    u8 pad401[0x600 - 0x401];
    s16 f600;
    s16 f602;
    s32 f604;
} FontTransfer;

extern void UiGlyph_DecodeWithHeapRoutines(FontTransfer *work, s32 slot);

extern s32 gUiIcon_FramePointerTable[];
extern s32 gUiIcon_PsynergyIconPointers[];

void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 withBase, s32 *src,
                   s32 *dst, s32 reuse)
{
    FontTransfer *work;
    s32 slot;

    work = Runtime_AllocateHeapBlock(0x44, 0x608);
    slot = 0;

    if (glyph >= Ui_CountSecondTableEntries())
        glyph = 0;

    if (withBase != 0) {
        work->f604 = gUiIcon_FramePointerTable[2];
        work->f600 = 2;
        work->f602 = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 0);
        slot = 1;
    }

    work->f604 = gUiIcon_PsynergyIconPointers[glyph];
    work->f600 = 2;
    work->f602 = 2;
    UiGlyph_DecodeWithHeapRoutines(work, slot);

    if (reuse == 0)
        *src = Resource_FindFreeEntry();

    *dst = VramCache_Load(*src, 0x80, &work->f400);
    Runtime_ReleaseHeapBlock(0x44);
}
