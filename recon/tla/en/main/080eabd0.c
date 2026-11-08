// Matches: differ 0 (tla-en, 2026-11-08), no device.
// What closed it from 40 (8 November): three plain-C shapes, each settled from
// the lreg and greg dumps rather than by permuting.
// - shapes += slot before Func_080eab98: combine does not fold an add across a
//   call into the store, so the shape row's own register takes the slot and the
//   slot then dies into slot * 4 (the listing's r5 and r8); sched2 moves the add
//   back after the call. Written after the call it folds into the store (58).
// - One cell pointer reused for the cell and then the block: a local that dies
//   twice is left to global-alloc, so neither address ties to the cell table or
//   to slot * 4 in local-alloc (the listing's r1, with the table in r10).
// - grid as a function-scope local, as in Func_080eaa14 and Func_080eace8: it
//   gets no register and is reloaded from the pool each pass, which moves
//   reload's round-robin onto the listing's choices (slot * 4 in r1, the step
//   of actors in r2). The row's add then needs the index first, hence the
//   integer sum; &grid[row] adds the grid first (2).
// The empty once-only block between the two flag stores is gone: it hoisted
// the zero out of the loop, which this shape no longer wants.
// Editions, 7 November: each listing is 0x118 bytes, one function, and its
// instructions are the English ones (ja 080eab14, de 080f7bd0, es 080f3bc4,
// fr 080efbc4, it 080eabc4).
// Raises the map under a list of actors (ids until 0xffff). For each
// it reads the actor's cell key and height, clears its motion and collision
// flags, shows its sprite, gives the cell's shape in gMapShapeGrid
// Func_080eab98's tag four half-units higher, saves the cell entry's byte 3
// in the actor's word at 76 before setting its raised bit, and blocks the
// same slot in layer 2's cell table. The leader is then lifted onto the
// terrain if below it, as in Func_080ea7c0.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

struct SwitchCell {
  u8 unk00[2];
  u8 block;
  u8 flags;
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

struct Actor *Actor_GetById(u32 actor);
s32 Func_080eab70(struct Actor *object);
u32 Func_080202f0(s32 terrain, s32 x, s32 z);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 Func_080eab98(s32 unused, s32 key, s32 value);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           s32 flags);

void Func_080eabd0(u16 *actors) {
  struct RestoreMap *map = (struct RestoreMap *)Ram_HeapSlots->mapWork;
  struct SwitchCell *blocks = map->layers[2].cells;
  u8 *grid = gMapShapeGrid;
  struct Actor *leader;
  s32 height;

  for (; *actors != 0xffff; actors++) {
    struct Actor *object = Actor_GetById(*actors);
    u32 key = Func_080202f0(object->terrainId, object->x, object->z);
    s32 level =
        Map_GetTerrainHeightFar(object->terrainId, object->x, object->z) >> 19;
    s32 slot = Func_080eab70(object);
    struct SwitchCell *cells = map->layers[object->terrainId].cells;
    // The row's address as an integer sum, index first: &grid[row] adds the
    // grid first.
    u8 *shapes = (u8 *)((((s32)cells - 0x02010000) >> 2) + (s32)grid);
    struct SwitchCell *cell;
    s32 tag;
    u8 flags;

    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSingleChildFlagsFar(object, 0);
    shapes += slot;
    tag = Func_080eab98(object->terrainId, key, level + 4);
    cell = &cells[slot];
    flags = cell->flags;
    object->unk4c = flags;
    cell->flags = flags | 0x80;
    *shapes = tag;
    // Reused for the block in layer 2's table.
    cell = &blocks[slot];
    cell->block = 0xff;
  }
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeightFar(leader->terrainId, leader->x, leader->z);
  if (leader->y < height)
    leader->y = leader->terrainHeight = height;
}
