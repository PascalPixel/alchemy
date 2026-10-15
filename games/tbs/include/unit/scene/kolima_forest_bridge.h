#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_BRIDGE_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_BRIDGE_H

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
extern s32 gKolimaForestBridge_GaretAction[];
extern s32 gKolimaForestBridge_IvanAction[];
extern s32 gKolimaForestBridge_MiaAction[];
extern s32 gKolimaForestBridge_PartyAction[];
extern s32 gKolimaForestBridge_ResetAction[];
extern s32 gKolimaForestBridge_FormationAction[];
extern s32 gKolimaForestBridge_FinishAction[];
extern s32 gKolimaForestBridge_EntryAction[];
extern struct SceneEntrance gKolimaForestBridge_Entrances[];
extern struct SceneEntrance gKolimaForestBridge_Exits[];
extern struct SceneEntrance gKolimaForestBridge_Placements[];
extern struct SceneEvent gKolimaForestBridge_Events[];
extern s32 gKolimaForestBridge_Object26Script[];
extern s32 gKolimaForestBridge_TriggerScript[];
extern s32 gFallingEffectScript[];

#endif
