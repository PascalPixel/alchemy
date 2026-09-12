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
