#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_08017a64(u16 *text) {
    s32 width;
    u16 *cursor;
    u32 character;
    u32 index;

    character = *text;
    width = 0;
    cursor = text + 1;
    if (character != 0) {
        do {
            if (character == 0x20) {
                width += 4;
            } else if (character <= 0xFFU) {
                index = character - 0xDE;
                if (index > 1U) {
                    width += M2C_FIELD(((index + 0xBE) << 5), u16, 0x08032224);
                }
            } else {
                width += 0xA;
            }
            character = *cursor;
            cursor += 1;
        } while (character != 0);
    }
    return width;
}
