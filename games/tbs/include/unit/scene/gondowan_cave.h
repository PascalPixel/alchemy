#ifndef ALCHEMY_UNIT_SCENE_GONDOWAN_CAVE_H
#define ALCHEMY_UNIT_SCENE_GONDOWAN_CAVE_H

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
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gKaragoruDouEntrancesOther[];
extern struct SceneEntrance gKaragoruDouEntrances1[];
extern struct SceneEntrance gKaragoruDouEntrances2[];
extern struct SceneEntrance gKaragoruDouEntrances3[];
extern u32 gKaragoruDouExits[];
extern struct ScenePlacement gKaragoruDouPlacementsOther[];
extern struct ScenePlacement gKaragoruDouPlacements1[];
extern struct ScenePlacement gKaragoruDouPlacements1Flag96f[];
extern struct ScenePlacement gKaragoruDouPlacements2[];
extern struct ScenePlacement gKaragoruDouPlacements3[];
extern struct SceneEvent gKaragoruDouEventsOther[];
extern struct SceneEvent gKaragoruDouEvents1[];
extern struct SceneEvent gKaragoruDouEvents1Flag96f[];
extern struct SceneEvent gKaragoruDouEvents2[];
extern struct SceneEvent gKaragoruDouEvents3[];

#endif
