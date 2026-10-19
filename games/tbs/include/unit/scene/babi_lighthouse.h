#ifndef ALCHEMY_UNIT_SCENE_BABI_LIGHTHOUSE_H
#define ALCHEMY_UNIT_SCENE_BABI_LIGHTHOUSE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
u8 *BabiLighthouse_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 BabiLighthouse_SetupScene(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gBabiLighthouse_TrembleScript[];
extern struct SceneEntrance gBabiLighthouse_Entrances3[];
extern struct SceneEntrance gBabiLighthouse_Entrances2[];
extern struct SceneEntrance gBabiLighthouse_Entrances1[];
extern struct SceneEntrance gBabiLighthouse_EntrancesOther[];
extern s16 gBabiLighthouse_Regions3[];
extern u32 gBabiLighthouse_Exits[];
extern struct ScenePlacement gBabiLighthouse_NoPlacements[];
extern struct ScenePlacement gBabiLighthouse_Placements3[];
extern struct ScenePlacement gBabiLighthouse_Placements2[];
extern struct ScenePlacement gBabiLighthouse_Placements1[];
extern struct ScenePlacement gBabiLighthouse_PlacementsOther[];
extern struct SceneEvent gBabiLighthouse_Events3[];
extern struct SceneEvent gBabiLighthouse_Events2[];
extern struct SceneEvent gBabiLighthouse_Events1[];
extern struct SceneEvent gBabiLighthouse_EventsOther[];

#endif
