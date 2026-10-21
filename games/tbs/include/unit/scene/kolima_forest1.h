#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_1_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_1_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *KolimaForest1_GetEntrances(void);
const struct SceneRegion *KolimaForest1_GetRegions(void);
const u32 *KolimaForest1_GetExits(void);
const struct ScenePlacement *KolimaForest1_GetPlacements(void);
const struct SceneEvent *KolimaForest1_GetEvents(void);
s32 KolimaForest1_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gKolimaForest1_GaretAction[];
extern s32 gKolimaForest1_IvanAction[];
extern s32 gKolimaForest1_MiaAction[];
extern s32 gKolimaForest1_SquashAction[];
extern s32 gKolimaForest1_RestoreScaleAction[];
extern s32 gKolimaForest1_BarrierGrowAction[];
extern s32 gKolimaForest1_BarrierShrinkAction[];
extern s32 gKolimaForest1_GroundSparkleAction[];
extern struct SceneEntrance gKolimaForest1_Entrances[];
extern u32 gKolimaForest1_Exits[];
extern struct ScenePlacement gKolimaForest1_Placements[];
extern struct SceneEvent gKolimaForest1_Events[];
extern s32 gKolimaForest1_SparkleScript[];
extern s32 gKolimaForest1_SparkleBurstScript[];
extern s32 gFallingEffectScript[];

#endif
