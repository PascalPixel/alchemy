// Differ 8 of 84 (tla-en, 2026-11-08, -mtune=gba); the same rows. Without the ActorX wrapper 20; ActorX or an
// ActorZ inline on the first call, the shape offset as u32 or as gMapShapeGrid + (...): 8;
// permuter 5 minutes, nothing better.
// Differ 8 of 84 (tla-en, 2026-11-07): x for Map_GetTerrainHeightFar read through an
// inline helper (a tagged wrapper, found by permute) settles the second and third calls'
// registers; left: the first call loads x before the shape row is finished and
// the grid address takes r3 for r1 (permute 6 min from here: nothing better).
// Before, 20 of 84 (2026-11-06): only scheduling and argument-register choice around the first two calls remain. The native loads gMapShapeGrid's address into r1 and finishes the shape row before loading z then x for Func_080202f0; here the address takes r3 and x is hoisted between. For Map_GetTerrainHeightFar the native holds the terrain byte's address in r2 and loads x before z; here r1 and z first; the third call's terrain address takes r2 for the native's r3.
// Allocation now matches the native (slot spilled to sp 0, blocks r10, shapes r6): the shape is stored right after Func_080eab98 through struct ShapeCell, as in Func_080eace8, so its store gets an alias set of its own and sched2 places it after the cell's byte 3 as the native does. Storing a plain u8 there gives 22; a tag stored later in the source spills blocks instead (36).
// Every edition has this one form: de, es, fr, it and ja measure the same 20 of 84 with the same differences, and the literal pool is alike in all six (2026-11-07).
// Earlier: 36 of 84 (2026-11-05) with the tag stored last; 40 with untagged empty asms, one-pass blocks and locals found by search.
// Raises the map again under the switch actor recorded in the
// event work's word at 0xcbc (by Func_080eace8 or Func_080eaa14), as
// Func_080eabd0 does for each listed actor: the cell's shape takes
// Func_080eab98's tag four half-units higher, the cell entry's byte 3 is
// saved in the actor's word at 76 before its raised bit is set, and layer
// 2's slot is blocked.
#include "types.h"
#include "actor.h"
#include "ram_buffer.h"

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
    struct SwitchCell *cells = map->layers[target->terrainId].cells;
    struct SwitchCell *blocks = map->layers[2].cells;
    u8 *shapes = &gMapShapeGrid[((s32)cells - 0x02010000) >> 2];
    u32 key = Func_080202f0(target->terrainId, target->x, target->z);
    /* FAKEMATCH(wrapper): x read through ActorX for the height call; as
     * target->x, 20 of 84 instructions differ in tla-en. */
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
