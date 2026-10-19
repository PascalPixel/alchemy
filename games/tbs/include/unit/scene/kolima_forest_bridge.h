#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_BRIDGE_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_BRIDGE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *KolimaForestBridge_GetEntrances(void);
const struct SceneRegion *KolimaForestBridge_GetRegions(void);
const u32 *KolimaForestBridge_GetExits(void);
const struct ScenePlacement *KolimaForestBridge_GetPlacements(void);
const struct SceneEvent *KolimaForestBridge_GetEvents(void);
s32 KolimaForestBridge_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gKolimaForestBridge_GaretAction[];
extern s32 gKolimaForestBridge_IvanAction[];
extern s32 gKolimaForestBridge_MiaAction[];
extern s32 gKolimaForestBridge_SquashAction[];
extern s32 gKolimaForestBridge_RestoreScaleAction[];
extern s32 gKolimaForestBridge_BarrierGrowAction[];
extern s32 gKolimaForestBridge_BarrierShrinkAction[];
extern s32 gKolimaForestBridge_GroundSparkleAction[];
extern struct SceneEntrance gKolimaForestBridge_Entrances[];
extern u32 gKolimaForestBridge_Exits[];
extern struct ScenePlacement gKolimaForestBridge_Placements[];
extern struct SceneEvent gKolimaForestBridge_Events[];
extern s32 gKolimaForestBridge_SparkleScript[];
extern s32 gKolimaForestBridge_SparkleBurstScript[];
extern s32 gFallingEffectScript[];

#endif
