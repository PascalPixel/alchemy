#ifndef ALCHEMY_UNIT_MAP_KOLIMA_FOREST_ENTRANCE_H
#define ALCHEMY_UNIT_MAP_KOLIMA_FOREST_ENTRANCE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *KolimaForestEntrance_GetEntrances(void);
const struct SceneExitRegion *KolimaForestEntrance_GetRegions(void);
const u32 *KolimaForestEntrance_GetExits(void);
const struct ActorTemplate *KolimaForestEntrance_GetActorTemplates(void);
const struct MapEvent *KolimaForestEntrance_GetEvents(void);
s32 KolimaForestEntrance_Initialize(void);
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
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
extern struct ActorTemplate gKolimaForestEntrance_ActorTemplates[];
extern struct MapEvent gKolimaForestEntrance_Events[];
extern s32 gKolimaForestEntrance_SparkleScript[];
extern s32 gKolimaForestEntrance_SparkleBurstScript[];
extern s32 gFallingEffectScript[];

#endif
