#ifndef ALCHEMY_UNIT_SCENE_SOL_SANCTUM_ERUPTION_H
#define ALCHEMY_UNIT_SCENE_SOL_SANCTUM_ERUPTION_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance *SolSanctumEruption_GetEntrances(void);
struct SceneRegion *SolSanctumEruption_GetRegions(void);
u32 *SolSanctumEruption_GetExits(void);
struct ScenePlacement *SolSanctumEruption_GetPlacements(void);
struct SceneEvent *SolSanctumEruption_GetEvents(void);
s32 SolSanctumEruption_Initialize(void);
extern s32 gEruption_ArcOriginsHead[];
extern s32 gEruption_ArcOrigins[];
extern struct SceneEntrance gSolSanctumEruption_Entrances[];
extern u32 gSolSanctumEruption_Exits[];
extern struct ScenePlacement gSolSanctumEruption_Placements[];
extern struct SceneEvent gSolSanctumEruption_Events[];
extern u16 gSolSanctumEruption_StarCells[];
extern s32 gEruption_EmberScript[];
extern u8 gSolSanctum_RingDrift[][3];
extern u8 gSolSanctum_RingSwing[][3];
extern s8 gSolSanctum_RingDirection[][3];
extern s32 gSolSanctumRing_MaxScales[];
extern s32 gSolSanctumRing_ScaleSteps[];

#endif
