#ifndef ALCHEMY_UNIT_SCENE_CROSSBONE_ISLE_H
#define ALCHEMY_UNIT_SCENE_CROSSBONE_ISLE_H

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
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern u16 gCrossboneIsle_EntranceCells[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gCrossboneIsleEntrances1[];
extern struct SceneEntrance gCrossboneIsleEntrances2[];
extern struct SceneEntrance gCrossboneIsleEntrances3[];
extern struct SceneEntrance gCrossboneIsleEntrances4[];
extern struct SceneEntrance gCrossboneIsleEntrances5[];
extern u32 gCrossboneIsleExits[];
extern struct ScenePlacement gCrossboneIslePlacementsOther[];
extern struct ScenePlacement gCrossboneIslePlacements1[];
extern struct ScenePlacement gCrossboneIslePlacements3[];
extern struct ScenePlacement gCrossboneIslePlacements6To14[];
extern struct SceneEvent gCrossboneIsleEventsOther[];
extern struct SceneEvent gCrossboneIsleEvents2[];
extern struct SceneEvent gCrossboneIsleEvents1[];
extern struct SceneEvent gCrossboneIsleEvents3[];
extern struct SceneEvent gCrossboneIsleEvents4[];
extern struct SceneEvent gCrossboneIsleEvents5[];

#endif
