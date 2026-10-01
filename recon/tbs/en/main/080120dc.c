/* 2026-10-01 (matcher 2): 585 (20 register-only, 2 operand, 4 reordered,
 * 2 deleted). The buffers are RAM_BUFFER.H's constants now, as in
 * Map_GetTerrainHeight (08011f54): the collision byte and the height bytes
 * come from Ram_MapCollision and Ram_MapCollision + 1 scaled by the same
 * metatile * 4, which gives the two pool words. The difference is that
 * draft's too: the reference keeps metatile * 4 in r1 (and so runs short
 * of low registers and parks the scaled kind in ip), where here it takes r0
 * and the height address reuses it; the cell pointer also lands in r1, not
 * r2. A kind local, a function-pointer local, masking x and z first, a
 * height-address local, defaulting the cells first or inverting the layer
 * test all score 635-1300. */
/* Not-yet-C: whole 192-byte terrain-height comparison and literal pool.
 * Corrected the old call-via pseudo-call to its three-argument callback.
 * Direct signed /16 division preserves rounding toward zero and improves
 * the layer/cell address calculation. Candidate 184/192 bytes, 62 differing
 * halfwords / 32 aligned edits. Callback index/argument setup is eight bytes
 * shorter; cell-base and mask registers differ. The allocator decoder finds
 * no unique source repair, so no permutation search was run. */
#include "TYPES.H"
#include "MAP.H"
#include "GLOBAL_CELLS.H"
#include "RAM_BUFFER.H"
extern u8 gMapWork[];

/* Same object-field shape check_object_tile.c already established (x@8,
   y@16, map_layer@34); this owner additionally reads a u32 "height" field
   at offset 20 that check_object_tile.c leaves as unread padding. */
struct MapObject {
    u8 unknown_00[20];
    s32 height;
    u8 unknown_18[10];
    u8 map_layer;
};

/* The candidate position: only the integer tile part of a Q16.16 x/z pair
   survives (the compiler narrows the >>16 into a direct halfword load at
   offset+2/+10), matching the 12-byte {x,unused,z} triple the 0800dd70.s
   call sites build on the stack from an object's own x/unused/z words. */
struct MapPosition {
    u8 unknown_00[2];
    s16 x;
    u8 unknown_04[6];
    s16 z;
};

/* Two parallel byte-strided tables share one attr_idx*4 index rather than
   one struct's two fields: the reference emits a SEPARATE literal-pool
   constant for each base (0x0202c000 and 0x0202c001, both scaled by the
   same attr_idx*4), not one struct base plus a +1 field offset. Byte 0 of
   the 0202c000 table selects the height-handler entry (low nibble indexes
   the Func_080134fc function-pointer table); the ADDRESS (not the value)
   of the matching 0202c001 byte is handed to the handler, so the handler
   reads whatever it needs from that second table itself. */
extern u8 gMapCollision[];
extern u8 Data_0202c001[];
typedef s32 (*TerrainHeightFn)(u8 *, s32, s32);
extern TerrainHeightFn Func_080134fc[];


s32 Func_080120dc(struct MapObject *object, struct MapPosition *position)
{
    struct MapState *state;
    s32 x;
    s32 z;
    s32 tile_x;
    s32 tile_z;
    u8 *cells;
    u8 *cell;
    s32 idx;
    s32 kind;
    s32 height;
    s32 delta;

    x = position->x;
    z = position->z;
    state = *(struct MapState **)gMapWork;
    if (state == 0)
        return 0;

    if (object->map_layer <= 2)
        cells = (u8 *)state->layers[object->map_layer].cells;
    else
        cells = Ram_MapCellBuffer;

    tile_x = x / 16;
    tile_z = z / 16;

    cell = cells + ((tile_x + (tile_z << 7)) << 2);
    if (cell[2] == 0xff)
        return 2;

    idx = cell[3];
    height = Func_080134fc[Ram_MapCollision[idx * 4] & 15](Ram_MapCollision + 1 + idx * 4, x & 15, z & 15);

    delta = height - object->height;
    if (delta > 0x80000)
        return 1;
    if (delta < (s32)0xfff40000)
        return -1;
    return 0;
}
