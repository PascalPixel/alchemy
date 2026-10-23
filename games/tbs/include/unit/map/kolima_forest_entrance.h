#ifndef ALCHEMY_UNIT_MAP_KOLIMA_FOREST_ENTRANCE_H
#define ALCHEMY_UNIT_MAP_KOLIMA_FOREST_ENTRANCE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *KolimaForestEntrance_GetWarpDests(void);
const struct MapWarpRegion *KolimaForestEntrance_GetRegions(void);
const u32 *KolimaForestEntrance_GetWarps(void);
const struct ObjectEventTemplate *
KolimaForestEntrance_GetObjectEventTemplates(void);
const struct MapEvent *KolimaForestEntrance_GetEvents(void);
s32 KolimaForestEntrance_Initialize(void);
extern s32 gStagedObjectEvent_DirectionSteps[];
extern u32 gStagedObjectEvent_FootprintKinds[];
extern s32 gStagedObjectEvent_FootprintBounds[];
extern s32 gKolimaForestEntrance_GaretAction[];
extern s32 gKolimaForestEntrance_IvanAction[];
extern s32 gKolimaForestEntrance_MiaAction[];
extern s32 gKolimaForestEntrance_SquashAction[];
extern s32 gKolimaForestEntrance_RestoreScaleAction[];
extern s32 gKolimaForestEntrance_PsynergyFieldGrowAction[];
extern s32 gKolimaForestEntrance_PsynergyFieldShrinkAction[];
extern s32 gKolimaForestEntrance_GroundSparkleAction[];
extern struct MapWarpDest gKolimaForestEntrance_WarpDests[];
extern u32 gKolimaForestEntrance_Warps[];
extern struct ObjectEventTemplate gKolimaForestEntrance_ObjectEventTemplates[];
extern struct MapEvent gKolimaForestEntrance_Events[];
extern s32 gKolimaForestEntrance_SparkleScript[];
extern s32 gKolimaForestEntrance_SparkleBurstScript[];
extern s32 gFallingEffectScript[];

#endif
