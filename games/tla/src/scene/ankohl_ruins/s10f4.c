#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "map.h"

struct AnkohlRuinsEventWork {
  u8 unk000[0x154];
  s16 cue;
  u8 unk156[0x76];
  struct Actor *companion;
};

extern struct MapCell gMapCellBuffer[];

s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(s32 id);
s32 Func_02002e0c(struct Actor *object, s32 *position);

void Func_020010f4(void) {
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  struct AnkohlRuinsEventWork *work =
      (struct AnkohlRuinsEventWork *)Ram_HeapSlots->eventWork;
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapWork;
  struct Actor *companion = work->companion;
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
  if (gAdventure.renderMode == 5)
    return;
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z + 0x80000;
  if (Func_02002e0c(leader, position) < 0)
    work->cue = 66;
  leader->z += 0x8000;
  companion->z += 0x8000;
}
