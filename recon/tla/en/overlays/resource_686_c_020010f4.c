// Draft, differ 6 of 95: while flag 0x104 is clear and the leader stands on a
// cell whose ground and upper layers read 236, pushes the leader and the event
// work's companion object down by half a pixel, raising cue 66 when
// Func_02002e0c refuses the step. Left: the native builds 0x80000 around the
// load of the leader's y and puts the argument copy between the next store and
// load (the known store-then-load order); barriers after either store and a
// four-minute permute do not reproduce it.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "map.h"

struct AnkohlRuinsEventWork {
  u8 unk000[0x154];
  s16 cue;
  u8 unk156[0x76];
  struct ObjectRuntime *companion;
};

extern struct MapCell gMapCellBuffer[];

s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_02002e0c(struct ObjectRuntime *object, s32 *position);

void Func_020010f4(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct AnkohlRuinsEventWork *work =
      (struct AnkohlRuinsEventWork *)Ram_HeapSlots->eventWork;
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapWork;
  struct ObjectRuntime *companion = work->companion;
  struct MapCell *ground;
  struct MapCell *upper;
  s32 index;
  s32 position[3];

  if (GameFlag_Test(0x104))
    return;
  if (map == NULL) {
    upper = gMapCellBuffer;
    ground = gMapCellBuffer;
  } else {
    ground = map->layers[0].cells;
    upper = map->layers[2].cells;
  }
  index = (leader->x >> 20) + ((leader->z >> 20) << 7);
  ground += index;
  upper += index;
  if (((ground->attributeB & 4) | (upper->collisionCode & 232)) != 236)
    return;
  if (gPartyState.renderMode == 5)
    return;
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z + 0x80000;
  if (Func_02002e0c(leader, position) < 0)
    work->cue = 66;
  leader->z += 0x8000;
  companion->z += 0x8000;
}
