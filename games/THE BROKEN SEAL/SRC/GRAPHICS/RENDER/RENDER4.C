#include "TYPES.H"
#include "RESOURCE.H"
#include "SCENE.H"

extern u8 UiIcon_ItemIconPointersEnd[];
extern u8 UiIcon_ItemIconPointers[];

extern u8 UiIcon_PsynergyIconPointersEnd[];
extern u8 UiIcon_PsynergyIconPointers[];

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

typedef struct {
    u8 pad0[0x400];
    u8 f400;
    u8 pad1[0x600 - 0x401];
    s16 f600;
    s16 f602;
    s32 f604;
} T;

extern s32 Runtime_AllocateHeapBlock(s32 no0, s32 no1);
extern s32 UiGlyph_DecodeWithHeapRoutines(T *, s32);
extern s32 VramBlock_LoadCached(s32, s32, u8 *);
extern s32 Runtime_ReleaseHeapBlock(s32);
extern s32 UiIcon_FramePointerTable[];
extern s32 UiIcon_OverlayPointerTable[];

void UiIcon_BuildItemIconTiles(s32, s32, s32, s32, s32);
u8 *BattleAction_Get(s32);

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
    T *work;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
    work->f604 = UiIcon_FramePointerTable[no1];
    work->f600 = 2;
    work->f602 = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 0);
    work->f604 = UiIcon_OverlayPointerTable[no0];
    work->f600 = 2;
    work->f602 = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 1);
    if (flag == 0) {
        *slot = Resource_FindFreeEntry();
    }
    *ret = VramBlock_LoadCached(*slot, 0x80, &work->f400);
    Runtime_ReleaseHeapBlock(0x11);
}

void Ability_RequestGlyph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    UiIcon_BuildItemIconTiles(BattleAction_Get(arg0)[4], arg1, arg2, arg3, arg4);
}
