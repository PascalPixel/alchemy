#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "RAM_BUFFER.H"
extern u8 gMapCellBuffer[];
extern u8 Data_03001e70[];

s32 Map_GetCellAttributeLowNibble(s32 index, s32 x, s32 y)
{
    u8 *state = *(u8 **)((u32)&Data_03001e70);
    u8 *map;
    s32 off;
    s32 col;
    s32 row;
    u32 attr;

    x >>= 16;
    y >>= 16;
    map = (u8 *)gMapCellBuffer;
    if (state != 0) {
        off = (index & 3) * 48 + 304;
        map = *(u8 **)(state + off);
    }
    col = x / 16;
    row = y / 16;
    map += (col + (row << 7)) * 4;
    attr = map[3];
    return *(Ram_MapCollision + attr * 4) & 15;
}
