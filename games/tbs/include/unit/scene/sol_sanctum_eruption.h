#ifndef ALCHEMY_UNIT_SCENE_SOL_SANCTUM_ERUPTION_H
#define ALCHEMY_UNIT_SCENE_SOL_SANCTUM_ERUPTION_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *SolSanctumEruption_GetEntrances(void);
const struct SceneRegion *SolSanctumEruption_GetRegions(void);
const u32 *SolSanctumEruption_GetExits(void);
const struct ScenePlacement *SolSanctumEruption_GetPlacements(void);
const struct SceneEvent *SolSanctumEruption_GetEvents(void);
s32 SolSanctumEruption_Initialize(void);
extern s32 gEruption_ArcOriginsHead[];
extern s32 gEruption_ArcOrigins[];
extern struct SceneEntrance gSolSanctumEruption_Entrances[];
extern u32 gSolSanctumEruption_Exits[];
extern struct ScenePlacement gSolSanctumEruption_Placements[];
extern struct SceneEvent gSolSanctumEruption_Events[];
extern u16 gSolSanctumEruption_UnusedStarCells[];
extern s32 gEruption_EmberScript[];
extern u8 gSolSanctum_RingDrift[][3];
extern u8 gSolSanctum_RingSwing[][3];
extern s8 gSolSanctum_RingDirection[][3];
extern s32 gSolSanctumRing_MaxScales[];
extern s32 gSolSanctumRing_ScaleSteps[];

#endif
