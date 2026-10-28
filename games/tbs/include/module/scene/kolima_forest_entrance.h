#ifndef ALCHEMY_MODULE_MAP_KOLIMA_FOREST_ENTRANCE_H
#define ALCHEMY_MODULE_MAP_KOLIMA_FOREST_ENTRANCE_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *KolimaForestEntrance_GetEntrances(void);
const struct SceneExitRegion *KolimaForestEntrance_GetRegions(void);
const u32 *KolimaForestEntrance_GetExits(void);
const struct ActorPlacement *KolimaForestEntrance_GetActorPlacements(void);
const struct Trigger *KolimaForestEntrance_GetTriggers(void);
s32 KolimaForestEntrance_Initialize(void);
extern s32 gPushedActor_DirectionSteps[];
extern u32 gPushedActor_FootprintKinds[];
extern s32 gPushedActor_FootprintBounds[];
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
extern struct ActorPlacement gKolimaForestEntrance_ActorPlacements[];
extern struct Trigger gKolimaForestEntrance_Triggers[];
extern s32 gKolimaForestEntrance_SparkleScript[];
extern s32 gKolimaForestEntrance_SparkleBurstScript[];
extern s32 gFallingEffectScript[];

#endif
