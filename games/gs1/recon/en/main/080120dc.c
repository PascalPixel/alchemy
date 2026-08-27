#include "types.h"
#include "map.h"
#include "global_cells.h"

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
extern u8 Data_0202c000[];
extern u8 Data_0202c001[];
extern u32 Data_080134fc[];

/* 0x080072e4 begins the GCC __call_via_rN veneer bank; a bl into it is an
   indirect call through the named register, not a call to a function at
   the branch target. This site's bl targets the r3 slot (0x080072f0), so
   the real callee is the function pointer passed as the trailing arg. */
s32 Func_080072f0(u32, s32, s32, u32);

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
    state = *(struct MapState **)ADDR_03001E70;
    if (state == 0)
        return 0;

    if (object->map_layer <= 2)
        cells = (u8 *)state->layers[object->map_layer].cells;
    else
        cells = (u8 *)0x02010000;

    tile_x = x;
    if (tile_x < 0)
        tile_x += 15;
    tile_x >>= 4;

    tile_z = z;
    if (tile_z < 0)
        tile_z += 15;
    tile_z >>= 4;

    cell = cells + ((tile_x + (tile_z << 7)) << 2);
    if (cell[2] == 0xff)
        return 2;

    idx = cell[3] << 2;
    kind = Data_0202c000[idx];

    height = Func_080072f0((u32)&Data_0202c001[idx], x & 15, z & 15,
        Data_080134fc[kind & 15]);

    delta = height - object->height;
    if (delta > 0x80000)
        return 1;
    if (delta < (s32)0xfff40000)
        return -1;
    return 0;
}
