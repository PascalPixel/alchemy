#include "trial_road/trial_road.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

void Func_02000038(void) { Func_080d2b4c(8, 75); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_0200646c[];
extern const u8 gData_02006ea4[];
extern const u8 gData_02006ed4[];
extern const u8 gData_02006f04[];
extern const u8 *const gData_02007024[];

s32 Func_02000058(struct Actor *object) {
  s32 height = Map_GetTerrainHeight(0, object->x, object->z) + 0x20000;

  object->terrainHeight = height;
  object->y = height;
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->scaleY = 0x18000;
  return 0;
}

const u8 *Scene_GetPlacements(void) {
  s32 pattern = gTrialRoadState->pattern;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room1)
    return gData_02007024[pattern & 7];
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room3)
    return gData_02006ea4;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room4)
    return gData_02006ed4;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room2)
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

#include "event_inline.h"

s32 Func_02000330(s32 value);
void ObjectEffect_StartContextEffect26(void);

void Func_02000138(s32 direction) {
  struct TrialRoadState *state = gTrialRoadState;
  s16 *order = (s16 *)state;

  if ((u32)(direction - 1) <= 1) {
    if (state->field16[0] <= 0) {
      if (GameFlag_Test(2559)) {
        if (Func_02000330(0) == 0)
          /* FAKEMATCH(wrapper): Call3 kept; as a direct call, 2 of 93
           * instructions differ in tla-en. */
          Actor_WalkByAndWait(gMainRoutineWork.currentOwner, 0, -16);
        return;
      }
      Import_EventSetExitRequest(direction + 4);
    } else {
      state->field16[0]--;
      Import_EventSetExitRequest(direction);
    }
    Sound_Play(123);
  }
  if ((u32)(direction - 3) <= 1) {
    state->field16[0]++;
    if (state->field16[0] > 3) {
      if (GameFlag_Test(2559) && Func_020000fc(0) == 0)
        GameFlag_SetBit(901);
      Sound_Play(128);
      ObjectEffect_StartContextEffect26();
      Import_EventSetExitRequest(direction + 4);
    } else {
      Import_EventSetExitRequest(direction);
      Sound_Play(123);
    }
    if (state->field16[0] > state->field1a)
      state->field1a = state->field16[0];
  }
  {
    /* FAKEMATCH(temporary): the byte offset kept in a local; as order[index], 2
     * of 93 instructions differ in tla-en. */
    s32 offset = (state->field16[0] * 2 + state->mirrored) * 2;

    state->pattern = *(s16 *)((u8 *)order + offset);
  }
}
