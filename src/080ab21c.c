typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_080ab21c(s32 x, s32 y, s32 width, s32 height, u32 palette)
{
    u8 *base;
    u8 *status;
    u32 palette_bits;
    u32 status_bit;
    s32 row;

    base = *(u8 **)0x03001e8c;
    palette_bits = palette;
    palette_bits <<= 12;

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
        status = base + 0xea3;
        status_bit = 2;
        row = (y << 6) + (x << 1);
        do {
            u16 *entry = (u16 *)(base + row);
            s32 remaining = width;

            if (remaining != 0) {
                u32 high_mask = 15;
                u32 clear_mask = 0xffff0fff;
                do {
                    u32 value = *entry;
                    u32 upper = value >> 12;
                    upper &= high_mask;
                    if (upper == 15) {
                        value &= clear_mask;
                        value |= palette_bits;
                        *entry = value;
                    }
                    remaining--;
                    entry++;
                } while (remaining != 0);
            }

            *status |= status_bit << ((u32)y >> 2);
            height--;
            row += 64;
            y++;
        } while (height != 0);
    }
}
