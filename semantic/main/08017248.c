typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_08017248(s32 x, s32 y, u32 width, u32 height, s32 mode)
{
    u8 **state_address = (u8 **)0x03001e8c;
    u8 *state;
    u32 row_a, column_a, tile_a;
    u32 row_b, column_b, tile_b;

    y <<= 5;
    state = *state_address;
    y += x;
    y <<= 1;
    y += (u32)state;

    if (width <= 1 || height <= 1 || width > 30 || height > 30)
        return;

    y += 64;
    if (mode == 0) {
        row_a = 1;
        if (row_a < height - 1) {
            do {
                column_a = 1;
                y += 2;
                if (column_a < width - 1) {
                    tile_a = row_a + 0x127;
                    do {
                        *(u16 *)y = (tile_a & 0xfff) | 0xf000;
                        column_a++;
                        tile_a += height - 2;
                        y += 2;
                    } while (column_a < width - 1);
                }
                y += 2;
                row_a++;
                y += (32 - width) * 2;
            } while (row_a < height - 1);
        }
    } else {
        row_b = 1;
        if (row_b < height - 1) {
            do {
                column_b = 0;
                if (column_b < width) {
                    tile_b = row_b + 0x127;
                    do {
                        *(u16 *)y = (tile_b & 0xfff) | 0xf000;
                        column_b++;
                        tile_b += height - 2;
                        y += 2;
                    } while (column_b < width);
                }
                row_b++;
                y += (32 - width) * 2;
            } while (row_b < height - 1);
        }
    }
    state[0xea3] = 1;
}
