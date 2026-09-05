#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void GraphicsPalette_DecrementSelectionWrap(void *base) {
    s32 v;
    u16 t;
    s32 cur;

    base = (u8 *)base + 0x574;
    v = FIELD_AT_OFFSET(base, u16 *, 0);
    t = v;
    cur = t;

    if (cur == 0) {
        cur = 2;
    } else {
        cur = v + 0xFFFF;
    }
    FIELD_AT_OFFSET(base, u16 *, 0) = cur;
}
