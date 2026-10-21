#ifndef ALCHEMY_UNIT_SCENE_CROSSBONE_ISLE_STEPPING_STONES_H
#define ALCHEMY_UNIT_SCENE_CROSSBONE_ISLE_STEPPING_STONES_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern const s32 *gEffectScripts[];
extern s32 gCrossboneIsleSteppingStones_ActionTable[];
extern u16 gCrossboneIsleSteppingStones_ShiftSteps1[];
extern u16 gCrossboneIsleSteppingStones_ShiftSteps2[];
extern u16 gCrossboneIsleSteppingStones_ShiftSteps3[];
extern u16 gCrossboneIsleSteppingStones_ShiftSteps4[];
extern u16 gCrossboneIsleSteppingStones_ShiftSteps5[];
extern struct SceneEntrance gCrossboneIsleSteppingStonesEntrancesOther[];
extern struct SceneEntrance gCrossboneIsleSteppingStonesEntrances1[];
extern struct SceneEntrance gCrossboneIsleSteppingStonesEntrances2[];
extern struct SceneEntrance gCrossboneIsleSteppingStonesEntrances3[];
extern struct SceneEntrance gCrossboneIsleSteppingStonesEntrances4[];
extern struct SceneEntrance gCrossboneIsleSteppingStonesEntrances5[];
extern u32 gCrossboneIsleSteppingStonesExits[];
extern struct ScenePlacement gCrossboneIsleSteppingStonesNoPlacements[];
extern struct ScenePlacement gCrossboneIsleSteppingStonesPlacements1[];
extern struct ScenePlacement gCrossboneIsleSteppingStonesPlacements2[];
extern struct ScenePlacement gCrossboneIsleSteppingStonesPlacements3[];
extern struct ScenePlacement gCrossboneIsleSteppingStonesPlacementsOther[];
extern struct ScenePlacement gCrossboneIsleSteppingStonesPlacements5[];
extern struct SceneEvent gCrossboneIsleSteppingStonesEventsOther[];
extern struct SceneEvent gCrossboneIsleSteppingStonesEvents1[];
extern struct SceneEvent gCrossboneIsleSteppingStonesEvents2[];
extern struct SceneEvent gCrossboneIsleSteppingStonesEvents3[];
extern struct SceneEvent gCrossboneIsleSteppingStonesEvents4[];
extern struct SceneEvent gCrossboneIsleSteppingStonesEvents5[];

#endif
