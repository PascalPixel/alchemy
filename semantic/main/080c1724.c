typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

s32 Func_080c1724(const u16 *source, u16 *destination, s32 scale, s32 count) {
    const u16 *cursor = source;

    source = (const u16 *)scale;
    if ((s32)source > 0x10000)
        source = (const u16 *)0x10000;

    while (count > 0) {
        u32 color = *cursor;
        u32 red = ((color & 0x001F) * (u32)source >> 16) & 0x001F;
        u32 green = ((color & 0x03E0) * (u32)source >> 16) & 0x03E0;
        u32 blue = ((color & 0x7C00) * (u32)source >> 16) & 0x7C00;

        count--;
        *destination = red | green | blue;
        cursor++;
        destination++;
    }
    return 0;
}
