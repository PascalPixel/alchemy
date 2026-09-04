#include "types.h"

#define GraphicsPalette_DecrementSelectedCounter Func_0801ce90

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern u8 Data_02000240[];

void GraphicsPalette_DecrementSelectedCounter(s32 work) {
    u8 *p;
    u16 sel;
    s32 off;

    sel = FIELD_AT_OFFSET(work, u16 *, 0x574);
    switch (sel) {
    case 0:
        off = 0x20C;
        p = &Data_02000240[off];
        break;
    case 1:
        off = 0x205;
        p = &Data_02000240[off];
        break;
    case 2:
        off = 0x206;
        p = &Data_02000240[off];
        break;
    default:
        return;
    }
    if (*p) {
        (*p)--;
    }
}
