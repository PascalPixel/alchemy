// Draft, differ 24 of 151: overlay 67b's copy of overlay 679's Func_020011a8
// (the same listing), whose draft holds its notes.
#include "../../../../games/tla/src/scene/apojii_islands/local.h"
#include "scene_ids.h"

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct ObjectRuntime *object;
};

void Func_02001e00(void) {
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
    result = Func_02002970(leader, position);
    if ((gFrameCount & 4) == 0)
      Func_02001d30(leader);
    if (result < 0) {
      Func_020029f8(owner, 0x102);
      Func_02002930(leader, leader->x, leader->y, leader->z + 0x80000);
      Func_02002910(leader, 49);
      Func_02002938(leader);
      do {
        WaitTicks(1);
      } while (leader->y != leader->terrainHeight);
      Func_02001d30(leader);
      Func_02002910(leader, 49);
      WaitTicks(3);
    } else {
      position[0] = leader->x;
      position[1] = leader->y;
      position[2] = leader->z + 0x80000;
      result = Func_02002970(leader, position);
      if (result <= 0) {
        position[0] = leader->x + 0x5b333;
        position[1] = leader->y;
        position[2] = leader->z + 0x5b333;
        result = Func_02002970(leader, position);
        if (result <= 0) {
          position[0] = leader->x - 0x5b333;
          position[1] = leader->y;
          position[2] = leader->z + 0x5b333;
          result = Func_02002970(leader, position);
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
