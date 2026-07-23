typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

void Func_0801fda8(u8 *object, s32 x, s32 y, s32 width, s32 height) {
    u16 *map = *(u16 **)0x03001E8C;
    s32 sum;

    sum = x + *(u16 *)(object + 12);
    x = sum + 1;
    sum = y + *(u16 *)(object + 14);
    y = sum + 1;

    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29)
        width = 30 - x;

    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29)
        height = 20 - y;

    if (width > 0 && height > 0) {
        s32 yOffset = y << 6;
        s32 xOffset = x << 1;
        s32 offset = yOffset + xOffset;
        do {
            s32 remaining = width;
            u16 *cell = (u16 *)(offset + (s32)map);
            if (remaining != 0) {
                do {
                    remaining--;
                    *cell++ = 0xE006;
                } while (remaining != 0);
            }
            height--;
            offset += 64;
        } while (height != 0);
        ((u8 *)map)[0xEA3] = 1;
    }
}
