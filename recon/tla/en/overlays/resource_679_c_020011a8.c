// Near miss of Func_020011a8 (twin: overlay 67C Func_02002ca4), differ 24, all
// instruction order: the heap, owner and anchor loads at the top interleave
// differently, and in each of the four position blocks the native starts the z
// offset or the object argument one step earlier than y is copied. Seven spots;
// no device tried yet.
#include "../../../../games/tla/src/scene/apojii_islands/local.h"
#include "scene_ids.h"

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct ObjectRuntime *object;
};

void Func_020011a8(void) {
  struct EventWork1cc *work = Ram_HeapSlots->eventWork;
  s32 owner = gPartyState.currentOwner;
  struct ObjectRuntime *anchor = work->object;
  struct ObjectRuntime *leader = Object_GetById(owner);
  s32 position[3];
  s32 result;

  if (gPartyState.scene != (s32)&SceneId_Ov67bRoom1 ||
      leader->terrainHeight != 0) {
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z + 0x20000;
    result = Func_02001c9c(leader, position);
    if ((gFrameCount & 4) == 0)
      Func_020010d8(leader);
    if (result < 0) {
      Func_02001d74(owner, 0x102);
      Func_02001c84(leader, leader->x, leader->y, leader->z + 0x80000);
      Func_02001c6c(leader, 49);
      Func_02001c8c(leader);
      do {
        WaitFrames(1);
      } while (leader->y != leader->terrainHeight);
      Func_020010d8(leader);
      Func_02001c6c(leader, 49);
      WaitFrames(3);
    } else {
      position[0] = leader->x;
      position[1] = leader->y;
      position[2] = leader->z + 0x80000;
      result = Func_02001c9c(leader, position);
      if (result <= 0) {
        position[0] = leader->x + 0x5b333;
        position[1] = leader->y;
        position[2] = leader->z + 0x5b333;
        result = Func_02001c9c(leader, position);
        if (result <= 0) {
          position[0] = leader->x - 0x5b333;
          position[1] = leader->y;
          position[2] = leader->z + 0x5b333;
          result = Func_02001c9c(leader, position);
          if (result <= 0) {
            anchor->z += 0x20000;
            leader->z += 0x20000;
            leader->angle = 0x4000;
          }
        }
      }
    }
  }
}
