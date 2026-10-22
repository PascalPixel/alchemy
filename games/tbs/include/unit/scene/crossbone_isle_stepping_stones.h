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
extern u16 gCrossboneIsleSteppingStones_SwitchCloseSteps[];
extern u16 gCrossboneIsleSteppingStones_SwitchOpenSteps[];
extern u16 gCrossboneIsleSteppingStones_FloodgateCloseSteps[];
extern u16 gCrossboneIsleSteppingStones_FloodgateOpenSteps[];
extern u16 gCrossboneIsleSteppingStones_FloodgateFootSteps[];
extern struct SceneEntrance gCrossboneIsleSteppingStonesEntrancesOther[];
extern struct SceneEntrance gCrossboneIsleSteppingStonesEntrances[];
extern struct SceneEntrance gCrossboneIsleIcePillarsEntrances[];
extern struct SceneEntrance gCrossboneIsleFloodgateEntrances[];
extern struct SceneEntrance gCrossboneIsleStackedPillarsEntrances[];
extern struct SceneEntrance gCrossboneIsleDragonStatueEntrances[];
extern u32 gCrossboneIsleSteppingStonesExits[];
extern struct ScenePlacement gCrossboneIsleSteppingStonesNoPlacements[];
extern struct ScenePlacement gCrossboneIsleSteppingStonesPlacements[];
extern struct ScenePlacement gCrossboneIsleIcePillarsPlacements[];
extern struct ScenePlacement gCrossboneIsleFloodgatePlacements[];
extern struct ScenePlacement gCrossboneIsleSteppingStonesPlacementsOther[];
extern struct ScenePlacement gCrossboneIsleDragonStatuePlacements[];
extern struct SceneEvent gCrossboneIsleSteppingStonesEventsOther[];
extern struct SceneEvent gCrossboneIsleSteppingStonesEvents[];
extern struct SceneEvent gCrossboneIsleIcePillarsEvents[];
extern struct SceneEvent gCrossboneIsleFloodgateEvents[];
extern struct SceneEvent gCrossboneIsleStackedPillarsEvents[];
extern struct SceneEvent gCrossboneIsleDragonStatueEvents[];

#endif
