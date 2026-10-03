#include "TYPES.H"
#include "GLYPH.H"
#include "BATTLE_UNIT.H"
#include "RESOURCE.H"
#include "SCENE.H"

extern u8 UiIcon_ItemIconPointersEnd[];
extern u8 UiIcon_ItemIconPointers[];

extern u8 UiIcon_PsynergyIconPointersEnd[];
extern u8 UiIcon_PsynergyIconPointers[];


void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void UiGlyph_DecodeWithHeapRoutines(void *glyph, s32 outlined);
extern s32 VramBlock_LoadCached(s32, s32, u8 *);
extern s32 Runtime_ReleaseHeapBlock(s32);
extern u8 *UiIcon_FramePointerTable[];
extern u8 *UiIcon_OverlayPointerTable[];

void UiIcon_BuildItemIconTiles(u32, s32, s32 *, s32 *, s32);
struct BattleAction *BattleAction_Get(s32);

/* 2つの境界アドレス間を4バイト単位で数える。 */
s32 Ui_CountIconTableEntries(void)
{
    return (s32)((u32)UiIcon_ItemIconPointersEnd - (u32)UiIcon_ItemIconPointers) >> 2;
}

s32 Ui_CountSecondTableEntries(void)
{
    return (UiIcon_PsynergyIconPointersEnd - UiIcon_PsynergyIconPointers) >> 2;
}

void Ui_BuildPairedPatternsToSlot(s32 no0, s32 no1, s32 *slot, s32 *ret, s32 flag)
{
    GlyphTransfer *work;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
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
    UiIcon_BuildItemIconTiles(BattleAction_Get(action)->unknown_04[0],
        with_base, slot, tile, reuse);
}
