#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 UiText_GetWideStringWidth(u16 *text) {
    s32 width;
    u16 *p;
    u32 c;
    u32 idx;

    c = *text;
    width = 0;
    p = text + 1;
    if (c != 0) {
        do {
            if (c == 0x20) {
                width += 4;
            } else if (c <= 0xFFU) {
                idx = c - 0xDE;
                if (idx > 1U) {
                    width += FIELD_AT_OFFSET(((idx + 0xBE) << 5), u16, 0x08032224);
                }
            } else {
                width += 0xA;
            }
            c = *p;
            p += 1;
        } while (c != 0);
    }
    return width;
}
