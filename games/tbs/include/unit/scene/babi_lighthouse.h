#ifndef ALCHEMY_UNIT_SCENE_BABI_LIGHTHOUSE_H
#define ALCHEMY_UNIT_SCENE_BABI_LIGHTHOUSE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance  *Scene_GetEntrances(void);
const struct SceneRegion    *Scene_GetRegions(void);
u8                          *BabiLighthouse_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          BabiLighthouse_SetupScene(void);
extern s32                   gStagedActor_DirectionSteps[];
extern u32                   gStagedActor_FootprintKinds[];
extern s32                   gStagedActor_FootprintBounds[];
extern s32                   gEffectScript0[];
extern s32                   gEffectScript1[];
extern s32                   gEffectScript2[];
extern const s32            *gEffectScripts[];
extern s32                   gBabiLighthousePlacements3_Data[];
extern struct SceneEntrance  gBabiLighthouseEntrances3[];
extern struct SceneEntrance  gBabiLighthouseEntrances2[];
extern struct SceneEntrance  gBabiLighthouseEntrances1[];
extern struct SceneEntrance  gBabiLighthouseEntrancesOther[];
extern s16                   gBabiLighthouseRegions3[];
extern u32                   gBabiLighthouseExits[];
extern struct ScenePlacement gBabiLighthouseNoPlacements[];
extern struct ScenePlacement gBabiLighthousePlacements3[];
extern struct ScenePlacement gBabiLighthousePlacements2[];
extern struct ScenePlacement gBabiLighthousePlacements1[];
extern struct ScenePlacement gBabiLighthousePlacementsOther[];
extern struct SceneEvent     gBabiLighthouseEvents3[];
extern struct SceneEvent     gBabiLighthouseEvents2[];
extern struct SceneEvent     gBabiLighthouseEvents1[];
extern struct SceneEvent     gBabiLighthouseEventsOther[];

#endif
