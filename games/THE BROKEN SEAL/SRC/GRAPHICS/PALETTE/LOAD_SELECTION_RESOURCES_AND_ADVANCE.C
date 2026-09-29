#include "TYPES.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern u8 *gSelectionWork;

void ShopCursor_AdvanceFar(void *);
void ShopCursor_MoveTowardTargetFar(void *);
s32 VramBlock_LoadCached(s32, s32, s32);
void Ui_ApplyTableScaleToObject(void *);

void GraphicsPalette_LoadSelectionResourcesAndAdvance(void)
{
    s32 src0;
    s32 src1;
    s32 sel;
    void *base;

    base = gSelectionWork;
    sel = FIELD(base, u16, 0x574);
    ShopCursor_AdvanceFar(base + 0x5A4);
    ShopCursor_MoveTowardTargetFar(base + 0x5B4);
    ShopCursor_MoveTowardTargetFar(base + 0x5C4);

    if (sel == 0) {
        src0 = (FIELD(base, u16, 0x57C) & 7) +
            (s32)&ResourceId_ShopCursorFrames;
    } else {
        src0 = (s32)&ResourceId_ShopCursorFrames;
    }
    VramBlock_LoadCached(
        FIELD(FIELD(base, void *, 0x5B4), u8, 14),
        0x100,
        (s32)Resource_GetTableEntry(src0));

    if (sel == 1) {
        src1 = (FIELD(base, u16, 0x57C) & 7) +
            (s32)&ResourceId_ShopCursorFrames;
    } else {
        src1 = (s32)&ResourceId_ShopCursorFrames;
    }
    VramBlock_LoadCached(
        FIELD(FIELD(base, void *, 0x5C4), u8, 14),
        0x100,
        (s32)Resource_GetTableEntry(src1));

    if (sel > 1) {
        s32 idx = sel * 3;
        s32 adj = 0x594 + sel;
        s32 off;

        adj = ((s8 *)base)[adj];
        idx += adj;
        off = 0x5D4 + idx * 4;
        Ui_ApplyTableScaleToObject(*(void **)((u8 *)base + off));
    }
    FIELD(base, u16, 0x57C)++;
}
