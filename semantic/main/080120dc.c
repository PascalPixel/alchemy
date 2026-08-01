// __call_via_rN veneer site, resolved per-site against the ROM.
//
// The `bl Func_080072f0` at 0x0801215a is `__call_via_r3` -- an indirect call
// through r3. The draft's fourth argument is the callee load, not an argument.
//
// Callee: `ldr r3, [r4, r2]` at 0x08012154, with r4 loaded at 0x08012148 from
// the pool word at 0x08012190. That word was READ FROM THE ROM at this site
// rather than carried over from the neighbour: it is 0x080134fc, the same
// sixteen-entry sampler table 0x08011f54 dispatches through, and r2 is
// (attribute & 15) << 2 routed via ip. Same table, checked here.
//
// Register agreement at the branch: r0 set at 0x08012150, r1 at 0x08012156,
// r2 at 0x08012158; the callee lands in r3 at 0x08012154. Three arguments,
// callee in the r3 slot -- the r0-r3 argument-register domain again.
//
// The result is consumed arithmetically right after the branch
// (`ldr r3, [r7, #20]; subs r0, r0, r3`), so the sampler returns a value.
// That is an instrument independent of the tail-position evidence at
// 0x08011f54, and the two agree.
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

/// One of the sixteen samplers at 0x080134fc, selected by the tile's low
/// attribute nibble. See 0x08011f54 for the table dump and the callee-side
/// arity evidence.
typedef s32 (*TileSampler)(const u8 *parameters, s32 subX, s32 subY);

extern struct MapState *Data_03001e70;
extern const TileSampler Data_080134fc[];

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
    difference = Data_080134fc[tile_data[0] & 15](
        tile_data + 1, x & 15, y & 15) - object->height_14;

    if (difference > 0x80000)
        return 1;
    if (difference < -0xc0000)
        return -1;
    return 0;
}
