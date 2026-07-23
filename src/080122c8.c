typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct Position_080122c8 {
    s32 x;
    s32 pad4;
    s32 y;
};

s32 Func_08012204(struct Position_080122c8 *);
extern u8 Data_0801353c[];

u8 Func_080122c8(struct Position_080122c8 *position, s32 *kind)
{
    s32 selector = Func_08012204(position);
    s32 x = position->x;
    s32 y;
    s32 flag = 0;
    u32 tile_x;
    u32 index;
    u32 *tile;

    if (x < 0)
        x += 0x1fffff;
    tile_x = (x >> 21) & 31;
    y = position->y;
    if (y < 0)
        y += 0x1fffff;

    index = tile_x + (((y >> 21) & 31) << 5);
    tile = (u32 *)0x02020000 + index;
    if (((u8 *)tile)[3] & 0x80)
        flag = 0x10;

    *kind = (*tile << 1) >> 25;
    if (*kind == 21)
        flag = 0x20;

    return Data_0801353c[flag + selector];
}
