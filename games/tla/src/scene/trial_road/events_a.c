#include "trial_road.h"

extern const u8 gData_0200646c[];
extern const u8 gData_02006ea4[];
extern const u8 gData_02006ed4[];
extern const u8 gData_02006f04[];
extern const u8 *const gData_02007024[];

s32 Func_02000058(struct ObjectRuntime *object) {
  s32 height = Map_GetTerrainHeight(0, object->x, object->z) + 0x20000;

  object->terrainHeight = height;
  object->y = height;
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->scaleY = 0x18000;
  return 0;
}

const u8 *Func_02000088(void) {
  s32 pattern = gTrialRoadState->pattern;

  if (gAdventure.scene == (s32)&SceneId_Ov697Room1)
    return gData_02007024[pattern & 7];
  if (gAdventure.scene == (s32)&SceneId_Ov697Room3)
    return gData_02006ea4;
  if (gAdventure.scene == (s32)&SceneId_Ov697Room4)
    return gData_02006ed4;
  if (gAdventure.scene == (s32)&SceneId_Ov697Room2)
    return gData_02006f04;
  return gData_0200646c;
}

s32 Func_020000fc(s32 fallback) {
  struct TrialRoadState *state = gTrialRoadState;

  if (state->field1a < state->field1c)
    return 1;
  if (state->field1a > state->field1c)
    return 0;
  if (GameFlag_Test(0x284))
    return 0;
  if (state->field14 != 0)
    return 1;
  return fallback;
}
