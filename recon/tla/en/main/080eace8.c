// Draft, differ 2 of 128, skeleton gap 1 (tla-en, 2026-11-01)
// All registers, spills and every other instruction match. What is left is
// one swapped pair at the loop's end: the native has lsls r1 then ldrh, this
// has ldrh then lsls. The ldrh follows the spill store of the list pointer,
// and sched2 readies it a cycle before the native does, because
// -mthumb-slow-loads only delays a load read whole, not an extended one
// (arm_adjust_cost; alchemy-gcc's README, Scheduling). That is a compiler
// model question, not a source one. Tried without gain: a u16 or s32 id
// local, an end local, a cast, pre-increment, while, do/while, a next
// local; as devices, a volatile read (38), a wrapper (34), a one-pass step
// and an empty asm after it (4 each), all taken back out.
// What closed the rest: shapes += slot before Func_080eab98 (the native adds
// after the call only by sched2), grid and zero as one-constant locals so
// reload rebuilds them at their uses, and the shape store first in a struct
// of its own, so sched2 may move it past the cell store.
// Lowers the map again under the switch actor that Func_080eabd0
// raised. The actor is the effect work's object at 20 when an effect runs
// with the event work's halfword at 358 set, Func_080eaeb4's otherwise. If
// it is in the list (ids until 0xffff), its cell's shape takes
// Func_080eab98's tag four half-units lower, the cell entry's byte 3 is
// restored from the actor's word at 76, layer 2's slot is unblocked and the
// event work's word at 0xcbc records the actor.
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

struct Actor *Actor_GetById(u32 actor);
struct Actor *Func_080eaeb4(void);
s32 Func_080eab70(struct Actor *object);
u32 Func_080202f0(s32 terrain, s32 x, s32 z);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 Func_080eab98(s32 unused, s32 key, s32 value);

void Func_080eace8(u16 *actors) {
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *effect = Ram_HeapSlots->effectWork;
  struct Actor *target;

  if (effect != NULL && *(s16 *)(event + 358) != 0)
    target = *(struct Actor **)(effect + 20);
  else
    target = Func_080eaeb4();
  *(struct Actor **)(event + 0xcbc) = NULL;

  for (; *actors != 0xffff; actors++) {
    if (target == Actor_GetById(*actors)) {
      struct RestoreMap *map = (struct RestoreMap *)Ram_HeapSlots->mapWork;
      s32 zero = 0;
      u8 *grid = gMapShapeGrid;
      struct SwitchCell *cells = map->layers[target->terrainId].cells;
      struct SwitchCell *blocks = map->layers[2].cells;
      u8 *shapes = &grid[((s32)cells - 0x02010000) >> 2];
      u32 key = Func_080202f0(target->terrainId, target->x, target->z);
      s32 level =
          Map_GetTerrainHeightFar(target->terrainId, target->x, target->z);
      s32 slot = Func_080eab70(target);
      s32 tag;

      shapes += slot;
      tag = Func_080eab98(target->terrainId, key, (level >> 19) - 4);
      cells += slot;
      blocks += slot;
      ((struct ShapeCell *)shapes)->shape = tag;
      cells->flags = target->unk4c;
      blocks->block = zero;
      *(struct Actor **)(event + 0xcbc) = target;
    }
  }
}
