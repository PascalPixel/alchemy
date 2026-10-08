#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "glyph.h"
#include "battle_unit.h"
#include "resource.h"
#include "scene.h"
#include "btlsys.h"

extern u8 UiIcon_ItemIconPointers[];

extern u8 UiIcon_PsynergyIconPointers[];

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *glyph, s32 outlined);


s32 Ui_CountIconTableEntries(void)
{
    return (UiIcon_ItemIconPointersEnd - UiIcon_ItemIconPointers) >> 2;
}

s32 Ui_CountSecondTableEntries(void)
{
    return (UiIcon_PsynergyIconPointersEnd - UiIcon_PsynergyIconPointers) >> 2;
}

void Ui_BuildPairedPatternsToSlot(s32 no0, s32 no1, s32 *slot, s32 *ret, s32 flag)
{
    GlyphTransfer *work;

    work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
    work->encoded = UiIcon_FramePointerTable[no1];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 0);
    work->encoded = UiIcon_OverlayPointerTable[no0];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 1);
    if (flag == 0) {
        *slot = Resource_FindFreeEntry();
    }
    *ret = VramBlock_LoadCached(*slot, 0x80, work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
}

void Ability_RequestGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse)
{
    UiIcon_BuildItemIconTiles(BtlActGet(action)->unknown_04[0],
        with_base, slot, tile, reuse);
}
