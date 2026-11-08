// Draft, differ 2 of 84 (tla-en, 2026-11-08). Left: the grid's pool load is
// scheduled one slot early, before layer 2's table load (the listing loads the
// table first); register and everything else match.
// Why, from the dumps: sched2 ties the two loads (priority 149, three
// dependents each) and takes the one first in RTL order. The listing's shape
// row is what reload produces when the grid's pseudo gets no hard register
// and is rematerialized from its constant right before the row's add: reload's
// round-robin then picks r1, after the table load. Checked by giving the grid
// a use after the last call (it then crosses calls and loses its register): the
// shape row comes out exactly as listed. Here the grid instead gets r1 from
// local-alloc, which needs its load between the cell-table index (r3) and the
// cell read so that its first-try lifetime overlaps r3 and the map in r2; that
// puts the load before the table load in RTL order, hence the one swap.
// Every edition measures the same 2, the same swap, at its own address (ja
// 080ead40, de 080f7dfc, es 080f3df0, fr 080efdf0, it 080eadf0; 2026-11-08).
// To reach 0: find what leaves the grid's pseudo without a register when its
// only use comes before the first call (in Func_080eabd0 and Func_080eace8 the
// grid crosses calls in their loops).
// Round 5 (2026-11-08), none below 2: &gMapShapeGrid[0], grid + index, a u32
// cast in the index (4), the grid after the cell read (19), blocks first (45),
// u8 *const grid (8), u8 *volatile grid (11), gMapShapeGrid inline (8); 12
// minutes of permute at seed 31 (148,000 candidates), nothing.
// Tried this round (2026-11-08), from 4: the grid as a local at function
// scope (33, 42), before the map (35), between the cells and the blocks (11);
// shapes before blocks (8); the cell table read through a layer pointer or a
// pointer to the field with the grid between (13: the address becomes
// (map + index) + 312, one more add); the row as an integer sum (4);
// the empty once-only block before or after each of map, cells, blocks (6 to
// 10); the body inside a once-only do or for loop (33, 35); permuter 10
// minutes from the 4 form, nothing. The offset form below no longer needs the
// once-only block or a block-scoped z (both 2 without them).
// Earlier, differ 4 (2026-11-08): an empty once-only block before the first
// call and z read into a block-scoped local finished the shape row before x
// was loaded; the grid's address took r3 for r1. Before that 8 and 20: x for
// Map_GetTerrainHeightFar read through ActorX (found by permute) settles the
// second and third calls' registers; the shape stored right after
// Func_080eab98 through struct ShapeCell, as in Func_080eace8, gives the store
// an alias set of its own so sched2 places it after the cell's byte 3 (a plain
// u8 store gives 22). Every edition measured the same at 20 (2026-11-07).
// Raises the map again under the switch actor recorded in the
// event work's word at 0xcbc (by Func_080eace8 or Func_080eaa14), as
// Func_080eabd0 does for each listed actor: the cell's shape takes
// Func_080eab98's tag four half-units higher, the cell entry's byte 3 is
// saved in the actor's word at 76 before its raised bit is set, and layer
// 2's slot is blocked.
#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "stddef.h"

struct SwitchCell {
  u8 unk00[2];
  u8 block;
  u8 flags;
};

// Only so the shape store has an alias set of its own; the grid holds bytes.
struct ShapeCell {
  u8 shape;
};

struct RestoreLayer {
  u8 unk00[0x138 - 0x108];
  struct SwitchCell *cells;
  u8 unk34[4];
};

struct RestoreMap {
  u8 unk000[0x108];
  struct RestoreLayer layers[3];
};

extern u8 gMapShapeGrid[];

s32 Func_080eab70(struct Actor *object);
u32 Func_080202f0(s32 terrain, s32 x, s32 z);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 Func_080eab98(s32 unused, s32 key, s32 value);

// Only for the compiler: see the FAKEMATCH tag in Func_080eadfc.
static inline s32 ActorX(struct Actor *actor) { return actor->x; }

void Func_080eadfc(void) {
  struct Actor *target =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);

  if (target != NULL) {
    struct RestoreMap *map = (struct RestoreMap *)Ram_HeapSlots->mapWork;
    /* FAKEMATCH(temporary): the cell table read split into its byte offset
     * and the read, with the grid's address read between them; as
     * cells = map->layers[target->terrainId].cells, 35 of 84 instructions
     * differ in tla-en. */
    s32 offset = offsetof(struct RestoreMap, layers[target->terrainId].cells);
    u8 *grid = gMapShapeGrid;
    struct SwitchCell *cells = *(struct SwitchCell **)((u8 *)map + offset);
    struct SwitchCell *blocks = map->layers[2].cells;
    u8 *shapes = &grid[((s32)cells - 0x02010000) >> 2];
    u32 key = Func_080202f0(target->terrainId, target->x, target->z);
    /* FAKEMATCH(wrapper): x read through ActorX for the height call; as
     * target->x, 14 of 84 instructions differ in tla-en. */
    s32 level =
        Map_GetTerrainHeightFar(target->terrainId, ActorX(target), target->z);
    s32 slot = Func_080eab70(target);
    s32 tag;
    u8 flags;

    shapes += slot;
    tag = Func_080eab98(target->terrainId, key, (level >> 19) + 4);
    ((struct ShapeCell *)shapes)->shape = tag;
    cells += slot;
    flags = cells->flags;
    target->unk4c = flags;
    cells->flags = flags | 0x80;
    blocks += slot;
    blocks->block = 0xff;
  }
}
