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
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 KolimaForestBridge_GaretAction[];
extern s32 KolimaForestBridge_IvanAction[];
extern s32 KolimaForestBridge_MiaAction[];
extern s32 KolimaForestBridge_PartyAction[];
extern s32 KolimaForestBridge_ResetAction[];
extern s32 KolimaForestBridge_FormationAction[];
extern s32 KolimaForestBridge_FinishAction[];
extern s32 KolimaForestBridge_EntryAction[];
extern struct SceneEntrance KolimaForestBridge_Entrances[];
extern struct SceneEntrance KolimaForestBridge_Exits[];
extern struct SceneEntrance KolimaForestBridge_Placements[];
extern struct SceneEvent KolimaForestBridge_Events[];
extern s32 KolimaForestBridge_Object26Script[];
extern s32 KolimaForestBridge_TriggerScript[];
extern s32 gFallingEffectScript[];

#endif
