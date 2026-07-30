typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

struct ObjectPosition {
    u8 unknown_00[0x14];
    s32 height_14;
    u8 unknown_18[0x0a];
    u8 layer_22;
};

struct ProbePosition {
    u8 unknown_00[2];
    s16 x_02;
    u8 unknown_04[6];
    s16 y_0a;
};

struct Cell {
    u8 unknown_0;
    u8 unknown_1;
    u8 kind;
    u8 tile;
};

struct MapLayer {
    struct Cell *cells;
    u8 unknown_04[0x2c];
};

struct MapState {
    u8 unknown_000[0x130];
    struct MapLayer layers[3];
};

extern struct MapState *Data_03001e70;
extern const s32 Data_080134fc[];

s32 Func_080072f0(const u8 *tile, s32 x, s32 y, s32 shape);

s32 Func_080120dc(const struct ObjectPosition *object,
                  const struct ProbePosition *position)
{
    struct MapState *map = Data_03001e70;
    struct Cell *cells;
    struct Cell *cell;
    const u8 *tile_data;
    s32 cell_x;
    s32 cell_y;
    s32 difference;
    s16 x = position->x_02;
    s16 y = position->y_0a;

    if (map == 0)
        return 0;

    if (object->layer_22 <= 2)
        cells = map->layers[object->layer_22].cells;
    else
        cells = (struct Cell *)0x02010000;

    cell_x = x < 0 ? (x + 15) >> 4 : x >> 4;
    cell_y = y < 0 ? (y + 15) >> 4 : y >> 4;
    cell = &cells[cell_x + cell_y * 128];
    if (cell->kind == 0xff)
        return 2;

    tile_data = (const u8 *)(0x0202c000 + cell->tile * 4);
    difference = Func_080072f0(
        tile_data + 1,
        x & 15,
        y & 15,
        Data_080134fc[tile_data[0] & 15]) - object->height_14;

    if (difference > 0x80000)
        return 1;
    if (difference < -0xc0000)
        return -1;
    return 0;
}
