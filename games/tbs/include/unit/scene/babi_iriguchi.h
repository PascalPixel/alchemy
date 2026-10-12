#ifndef ALCHEMY_UNIT_SCENE_BABI_IRIGUCHI_H
#define ALCHEMY_UNIT_SCENE_BABI_IRIGUCHI_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
u8 *BabiIriguchi_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 BabiIriguchi_SetupScene(void);
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gBabiIriguchiPlacements3_Data[];
extern struct SceneEntrance gBabiIriguchiEntrances3[];
extern struct SceneEntrance gBabiIriguchiEntrances2[];
extern struct SceneEntrance gBabiIriguchiEntrances1[];
extern struct SceneEntrance gBabiIriguchiEntrancesOther[];
extern s16 gBabiIriguchiRegions3[];
extern u32 gBabiIriguchiExits[];
extern struct ScenePlacement gBabiIriguchiNoPlacements[];
extern struct ScenePlacement gBabiIriguchiPlacements3[];
extern struct ScenePlacement gBabiIriguchiPlacements2[];
extern struct ScenePlacement gBabiIriguchiPlacements1[];
extern struct ScenePlacement gBabiIriguchiPlacementsOther[];
extern struct SceneEvent gBabiIriguchiEvents3[];
extern struct SceneEvent gBabiIriguchiEvents2[];
extern struct SceneEvent gBabiIriguchiEvents1[];
extern struct SceneEvent gBabiIriguchiEventsOther[];

#endif
