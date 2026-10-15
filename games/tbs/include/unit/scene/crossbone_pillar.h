#ifndef ALCHEMY_UNIT_SCENE_CROSSBONE_PILLAR_H
#define ALCHEMY_UNIT_SCENE_CROSSBONE_PILLAR_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance  *Scene_GetEntrances(void);
const struct SceneRegion    *Scene_GetRegions(void);
const u32                   *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          CrossbonePillar_SetupArea(void);
extern s32                   gEffectScript0[];
extern s32                   gEffectScript1[];
extern s32                   gEffectScript2[];
extern s32                   gStagedActor_DirectionSteps[];
extern u32                   gStagedActor_FootprintKinds[];
extern s32                   gStagedActor_FootprintBounds[];
extern const s32            *gEffectScripts[];
extern s32                   gCrossbonePillar_ActionTable[];
extern u16                   gCrossbonePillar_ShiftSteps1[];
extern u16                   gCrossbonePillar_ShiftSteps2[];
extern u16                   gCrossbonePillar_ShiftSteps3[];
extern u16                   gCrossbonePillar_ShiftSteps4[];
extern u16                   gCrossbonePillar_ShiftSteps5[];
extern struct SceneEntrance  gCrossbonePillarEntrancesOther[];
extern struct SceneEntrance  gCrossbonePillarEntrances1[];
extern struct SceneEntrance  gCrossbonePillarEntrances2[];
extern struct SceneEntrance  gCrossbonePillarEntrances3[];
extern struct SceneEntrance  gCrossbonePillarEntrances4[];
extern struct SceneEntrance  gCrossbonePillarEntrances5[];
extern u32                   gCrossbonePillarExits[];
extern struct ScenePlacement gCrossbonePillarNoPlacements[];
extern struct ScenePlacement gCrossbonePillarPlacements1[];
extern struct ScenePlacement gCrossbonePillarPlacements2[];
extern struct ScenePlacement gCrossbonePillarPlacements3[];
extern struct ScenePlacement gCrossbonePillarPlacementsOther[];
extern struct ScenePlacement gCrossbonePillarPlacements5[];
extern struct SceneEvent     gCrossbonePillarEventsOther[];
extern struct SceneEvent     gCrossbonePillarEvents1[];
extern struct SceneEvent     gCrossbonePillarEvents2[];
extern struct SceneEvent     gCrossbonePillarEvents3[];
extern struct SceneEvent     gCrossbonePillarEvents4[];
extern struct SceneEvent     gCrossbonePillarEvents5[];

#endif
