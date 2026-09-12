#include "types.h"
#include "scene.h"
#include "resource.h"

/* graphics/palette/palette_decrement_selected_counter.c */
/* graphics/palette/decrement_selected_counter.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern u8 gCell[];

void GraphicsPalette_DecrementSelectedCounter(s32 work)
{
    u8 *p;
    u16 sel;
    s32 off;

    sel = FIELD_AT_OFFSET(work, u16 *, 0x574);
    switch (sel) {
    case 0:
        off = 0x20C;
        p = &gCell[off];
        break;
    case 1:
        off = 0x205;
        p = &gCell[off];
        break;
    case 2:
        off = 0x206;
        p = &gCell[off];
        break;
    default:
        return;
    }
    if (*p) {
        (*p)--;
    }
}

/* graphics/palette/adjust_selection_counter.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))


void GraphicsPalette_AdjustSelectionCounter(s32 arg0)
{
    u8 *sp;
    u16 sel;
    s32 off;

    sel = FIELD_AT_OFFSET(arg0, u16, 0x574);
    switch (sel) {
    case 0:
        off = 0x20C;
        sp = &gCell[off];
        if (*sp <= 1) {
            break;
        }
        return;
    case 1:
        off = 0x205;
        sp = &gCell[off];
        if (*sp <= 23) {
            break;
        }
        return;
    case 2:
        off = 0x206;
        sp = &gCell[off];
        if (*sp <= 14) {
            break;
        }
        return;
    default:
        return;
    }
    (*sp)++;
}

/* graphics/palette/load_selection_resources_and_advance.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern u8 *gIw;
extern u8 gVal[];

s32 Resource_CopyData(s32, s32, s32);

void GraphicsPalette_LoadSelectionResourcesAndAdvance(void)
{
    s32 src0;
    s32 src1;
    s32 sel;
    void *base;

    base = gIw;
    sel = FIELD(base, u16, 0x574);
    Sys_Do(base + 0x5A4);
    ShopCursor_MoveTowardTarget(base + 0x5B4);
    ShopCursor_MoveTowardTarget(base + 0x5C4);

    if (sel == 0) {
        src0 = (FIELD(base, u16, 0x57C) & 7) +
            (s32)gVal;
    } else {
        src0 = (s32)gVal;
    }
    Resource_CopyData(
        FIELD(FIELD(base, void *, 0x5B4), u8, 14),
        0x100,
        (s32)GetResource(src0));

    if (sel == 1) {
        src1 = (FIELD(base, u16, 0x57C) & 7) +
            (s32)gVal;
    } else {
        src1 = (s32)gVal;
    }
    Resource_CopyData(
        FIELD(FIELD(base, void *, 0x5C4), u8, 14),
        0x100,
        (s32)GetResource(src1));

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
