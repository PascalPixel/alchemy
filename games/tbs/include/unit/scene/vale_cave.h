#ifndef ALCHEMY_UNIT_SCENE_VALE_CAVE_H
#define ALCHEMY_UNIT_SCENE_VALE_CAVE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *ValeCave_GetEntrances(void);
const struct SceneRegion *ValeCave_GetRegions(void);
const u32 *ValeCave_GetExits(void);
const struct ScenePlacement *ValeCave_GetPlacements(void);
const struct SceneEvent *ValeCave_GetEvents(void);
s32 ValeCave_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gValeCave_WalkTargets[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gValeCave_EntrancesOther[];
extern struct SceneEntrance gValeCave_Entrances1[];
extern struct SceneEntrance gValeCave_Entrances2[];
extern struct SceneEntrance gValeCave_Entrances3[];
extern u32 gValeCave_Exits[];
extern struct ScenePlacement gValeCave_Placements1[];
extern struct ScenePlacement gValeCave_Placements2[];
extern struct ScenePlacement gValeCave_Placements3[];
extern struct SceneEvent gValeCave_EventsOther[];
extern struct SceneEvent gValeCave_Events1[];
extern struct SceneEvent gValeCave_Events2[];
extern struct SceneEvent gValeCave_Events3[];

#endif
