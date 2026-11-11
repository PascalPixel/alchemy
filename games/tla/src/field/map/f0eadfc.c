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
    s32 index = ((s32)cells - (s32)Ram_MapCellBuffer) >> 2;
    u8 *shapes;
    u32 key;
    s32 level;
    s32 slot;
    s32 tag;
    u8 flags;

    /* FAKEMATCH(barrier): an empty asm on the grid index that claims to read
     * blocks, giving layer 2's table load a fourth dependent, so sched2 takes
     * it before the grid's pool load; without it, 2 of 84 instructions differ
     * in tla-en. */
    asm("" : "+l"(index) : "l"(blocks));
    shapes = &grid[index];
    key = Func_080202f0(target->terrainId, target->x, target->z);
    /* FAKEMATCH(wrapper): x read through ActorX for the height call; as
     * target->x, 12 of 84 instructions differ in tla-en. */
    level =
        Map_GetTerrainHeightFar(target->terrainId, ActorX(target), target->z);
    slot = Func_080eab70(target);
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
