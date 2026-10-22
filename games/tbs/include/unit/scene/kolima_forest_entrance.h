#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_ENTRANCE_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_ENTRANCE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *KolimaForestEntrance_GetEntrances(void);
const struct SceneRegion *KolimaForestEntrance_GetRegions(void);
const u32 *KolimaForestEntrance_GetExits(void);
const struct ScenePlacement *KolimaForestEntrance_GetPlacements(void);
const struct SceneEvent *KolimaForestEntrance_GetEvents(void);
s32 KolimaForestEntrance_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gKolimaForestEntrance_GaretAction[];
extern s32 gKolimaForestEntrance_IvanAction[];
extern s32 gKolimaForestEntrance_MiaAction[];
extern s32 gKolimaForestEntrance_SquashAction[];
extern s32 gKolimaForestEntrance_RestoreScaleAction[];
extern s32 gKolimaForestEntrance_PsynergyFieldGrowAction[];
extern s32 gKolimaForestEntrance_PsynergyFieldShrinkAction[];
extern s32 gKolimaForestEntrance_GroundSparkleAction[];
extern struct SceneEntrance gKolimaForestEntrance_Entrances[];
extern u32 gKolimaForestEntrance_Exits[];
extern struct ScenePlacement gKolimaForestEntrance_Placements[];
extern struct SceneEvent gKolimaForestEntrance_Events[];
extern s32 gKolimaForestEntrance_SparkleScript[];
extern s32 gKolimaForestEntrance_SparkleBurstScript[];
extern s32 gFallingEffectScript[];

#endif
