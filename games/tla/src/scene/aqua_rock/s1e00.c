#include "../../../../games/tla/src/scene/apojii_islands/local.h"
#include "scene_ids.h"

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct Actor *object;
};

void Func_02001e00(void) {
  struct Actor *anchor;
  s32 owner;
  struct Actor *leader;
  s32 position[3];
  s32 result;

  {
    struct EventWork1cc *work = Ram_HeapSlots->eventWork;

    owner = gMainRoutineWork.currentOwner;
    anchor = work->object;
  }
  leader = Actor_GetById(owner);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67bRoom1 &&
      leader->terrainHeight == 0)
    return;
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z + 0x20000;
  result = Func_02002970(leader, position);
  if ((gFrameCount & 4) == 0)
    Func_02001d30(leader);
  if (result < 0) {
    Actor_SetAttachedEffect(owner, 0x102);
    Actor_SetMoveTarget(leader, leader->x, leader->y, leader->z + 0x80000);
    Func_02002910(leader, 49);
    Script_WaitForEventTimeout(leader);
    do {
      wait(1);
    } while (leader->y != leader->terrainHeight);
    Func_02001d30(leader);
    Func_02002910(leader, 49);
    wait(3);
    return;
  }
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z + 0x80000;
  result = Func_02002970(leader, position);
  if (result > 0)
    return;
  position[0] = leader->x + 0x5b333;
  position[1] = leader->y;
  position[2] = leader->z + 0x5b333;
  result = Func_02002970(leader, position);
  if (result > 0)
    return;
  position[0] = leader->x - 0x5b333;
  position[1] = leader->y;
  position[2] = leader->z + 0x5b333;
  result = Func_02002970(leader, position);
  if (result > 0)
    return;
  anchor->z += 0x20000;
  leader->z += 0x20000;
  leader->facing = 0x4000;
}
