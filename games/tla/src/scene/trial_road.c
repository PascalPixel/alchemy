/// @file
/// Trial Road, the mountain path above the Shaman Village where the party races
/// Moapa to the summit: four stages dealt from eight puzzle rooms, a left and a
/// right path through each, the blue chests that open each stage's door for
/// the weight of the party's equipment, and the battle at the peak; after the
/// race, the summit's cave and pools and Gasp the Djinni.

#include "shared/trial_road.h"
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
#include "module/field/object.h"

void TrialRoad_FightGasp(void) { Field_RequestDjinnBattle(8, 75); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gTrialRoad_Placements[];
extern const u8 gTrialRoad_Room3Placements[];
extern const u8 gTrialRoad_Room4Placements[];
extern const u8 gTrialRoad_Room2Placements[];
extern const u8 *const gTrialRoad_Room1Placements[];

s32 TrialRoad_SettleOnGround(struct Actor *object) {
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
    return gTrialRoad_Room1Placements[pattern & 7];
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room3)
    return gTrialRoad_Room3Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room4)
    return gTrialRoad_Room4Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room2)
    return gTrialRoad_Room2Placements;
  return gTrialRoad_Placements;
}

s32 TrialRoad_RivalIsAhead(s32 fallback) {
  struct TrialRoadState *state = gTrialRoadState;

  if (state->field1a < state->field1c)
    return 1;
  if (state->field1a > state->field1c)
    return 0;
  if (Event_GetFlag(0x284))
    return 0;
  if (state->field14 != 0)
    return 1;
  return fallback;
}

#include "event_inline.h"

s32 TrialRoad_OfferToGiveUp(s32 value);

void TrialRoad_TakeStairs(s32 direction) {
  struct TrialRoadState *state = gTrialRoadState;
  s16 *order = (s16 *)state;

  if ((u32)(direction - 1) <= 1) {
    if (state->field16[0] <= 0) {
      if (Event_GetFlag(2559)) {
        if (TrialRoad_OfferToGiveUp(0) == 0)
          Actor_WalkByAndWait(gMainRoutineWork.currentOwner, 0, -16);
        return;
      }
      Import_EventSetExitRequest(direction + 4);
    } else {
      state->field16[0]--;
      Import_EventSetExitRequest(direction);
    }
    Event_PlaySound(123);
  }
  if ((u32)(direction - 3) <= 1) {
    state->field16[0]++;
    if (state->field16[0] > 3) {
      if (Event_GetFlag(2559) && TrialRoad_RivalIsAhead(0) == 0)
        Event_SetFlag(901);
      Event_PlaySound(128);
      Exit_StartStairsUp();
      Import_EventSetExitRequest(direction + 4);
    } else {
      Import_EventSetExitRequest(direction);
      Event_PlaySound(123);
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
