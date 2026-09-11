#include "types.h"
#include "scene.h"
#include "abi/graphics/palette/decrement_selected_counter.h"

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
