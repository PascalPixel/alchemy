/* Not-yet-C, main:08011f54, complete 132-byte function and pool.
 * The table is three-argument terrain callbacks, not a four-argument service.
 * Typed callback recovery keeps 54 instructions but chooses r4 for the call
 * and omits the reference's r7 base. Shared mask/indexed symbol accesses give
 * 52 instructions and the correct r3 call but fold two address operations.
 * Integer addresses share the attribute base with +1, unlike the two pools
 * in the reference. Three structural hypotheses stopped; no byte credit.
 */
#include "TYPES.H"
#include "GLOBAL_CELLS.H"

extern u8 Data_0202c001[];
typedef s32 (*TerrainHeight)(u8 *, s32, s32);
extern TerrainHeight Data_080134fc[];

s32 Func_08011f54(s32 index, s32 x, s32 y)
{
    u8 *table = *(u8 **)ADDR_03001E70;
    u8 *map;
    s32 offset;
    s32 column;
    s32 row;
    u32 entry;
    u8 config;
    TerrainHeight target;
    x >>= 16;
    y >>= 16;
    map = (u8 *)0x02010000;
    if (table != 0) {
        offset = (index & 3) * 48 + 304;
        map = *(u8 **)(table + offset);
    }
    column = x / 16;
    row = y / 16;
    map += (column + (row << 7)) * 4;
    entry = map[3];
    config = *(u8 *)(0x0202c000 + entry * 4) & 15;
    x &= 15;
    y &= 15;
    target = Data_080134fc[config];
    return target(&Data_0202c001[entry * 4], x, y);
}
