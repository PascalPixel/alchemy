#include "types.h"

struct Window_08020a60 {
    u8 padding[12];
    u16 x;
    u16 y;
};

typedef char Window_08020a60_size[
    sizeof(struct Window_08020a60) == 0x10 ? 1 : -1
];
typedef char Window_08020a60_x_offset[
    (u32)&(((struct Window_08020a60 *)0)->x) == 0x0c ? 1 : -1
];

void Func_08020a60(const struct Window_08020a60 *object,
    s32 x, s32 y, s32 width, s32 height, u32 field) {
    u8 *base = *(u8 **)0x03001E8C;

    x += object->x + 1;
    y += object->y + 1;
    field <<= 12;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29) {
        width = 30 - x;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29) {
        height = 20 - y;
    }
    if (width > 0 && height > 0) {
        y <<= 6;
        x = y + (x << 1);
        do {
            u16 *pixel = (u16 *)((u32)x + (u32)base);
            s32 remaining = width;
            while (remaining != 0) {
                u32 value = *pixel;
                value &= 0xFFFFEFFF;
                value |= field;
                remaining--;
                *pixel = value;
                pixel++;
            }
            height--;
            x += 64;
        } while (height != 0);
        base[0xEA3] = 1;
    }
}
