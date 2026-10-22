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
extern s32 gValeCave_ShortEffectScript[];
extern s32 gValeCave_MediumEffectScript[];
extern s32 gValeCave_LongEffectScript[];
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gValeCave_WalkTargets[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gValeCave_EntrancesOther[];
extern struct SceneEntrance gValeCave_MouthEntrances[];
extern struct SceneEntrance gValeCave_KiteEntrances[];
extern struct SceneEntrance gValeCave_LogsEntrances[];
extern u32 gValeCave_Exits[];
extern struct ScenePlacement gValeCave_MouthPlacements[];
extern struct ScenePlacement gValeCave_KitePlacements[];
extern struct ScenePlacement gValeCave_LogsPlacements[];
extern struct SceneEvent gValeCave_EventsOther[];
extern struct SceneEvent gValeCave_MouthEvents[];
extern struct SceneEvent gValeCave_KiteEvents[];
extern struct SceneEvent gValeCave_LogsEvents[];

#endif
