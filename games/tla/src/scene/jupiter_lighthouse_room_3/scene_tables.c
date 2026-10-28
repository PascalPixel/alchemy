#include "local.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  return gScenePlacements;
}

void Func_02000054(struct Actor *object) {
  if (object->x >> 20 == 14 && object->z >> 20 == 12)
    Func_02002360(object, 2);
}

void Func_02000070(void) {
  struct Actor *object = Actor_GetById(10);
  struct PushedBlock block;
  s32 i;
  s32 x;

  object->update = (void (*)(void))Func_02000054;
  if (Func_02000dcc(&block))
    Func_02001050(block);
  if (object->x >> 20 == 16 && object->z >> 20 == 12 &&
      GameFlag_Test_(0x200) == 0) {
    GameFlag_SetBit_(0x200);
    Actor_SetSpritePriority(10, 3);
    object->motionFlags = 3;
    WaitFrames(5);
    Func_02002470(134);
    object->motionFlags = 2;
    for (i = 0, x = 0xb00000; i <= 3; i++, x += 0x100000) {
      Lift_(0x1000000, x, 0, 0);
      Func_02002458_(0x1000000, x, 0, 0);
      if (i <= 1)
        Func_02002460_(0x1000000, x, 0, 128);
    }
  }
  object->update = 0;
}

void Data_02000130(void) {}
